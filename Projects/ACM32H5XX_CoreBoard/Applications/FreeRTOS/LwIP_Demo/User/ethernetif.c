/**
  ******************************************************************************
  * File Name          : ethernetif.c
  * Description        : This file provides code for the configuration
  *                      of the ethernetif.c MiddleWare.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_eth.h"
#include "bsp_lan8720.h"
#include "main.h"
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
#include <string.h>
#include "cmsis_os.h"
#include "lwip/tcpip.h"

/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT              ( portMAX_DELAY )
/* Time to block waiting for transmissions to finish */
#define ETHIF_TX_TIMEOUT                    (2000U)

/* ETH_CODE: increase stack size, otherwise there
 * might be overflow in more advanced applications.
 * Lower optimization can increase the stack usage
 * and cause stack overflows in some cases.
 */
/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE         ( 1024 )

/* Network interface name */
#define IFNAME0 'H'
#define IFNAME1 '5'

/* Private variables ---------------------------------------------------------*/
__IO uint32_t TxPkt = 0;
__IO uint32_t RxPkt = 0;

osSemaphoreId RxPktSemaphore = NULL;   /* Semaphore to signal incoming packets */
osSemaphoreId TxPktSemaphore = NULL;   /* Semaphore to signal transmit packet complete */

__IO uint32_t desc_index = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  handlerEth: ETH handler
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *handlerEth)
{
  osSemaphoreRelease(RxPktSemaphore);
}
/**
  * @brief  Ethernet Tx Transfer completed callback
  * @param  handlerEth: ETH handler
  * @retval None
  */
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *handlerEth)
{
  osSemaphoreRelease(TxPktSemaphore);
}


/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH)
*******************************************************************************/
/**
 * @brief In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
    uint32_t LAN8720_State;
    uint32_t auto_negotiation;
    
    if(bsp_eth_init() != HAL_OK)
    {
        return;
    }
        
    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] =  ETH_Handle.Init.MACAddr[0];
    netif->hwaddr[1] =  ETH_Handle.Init.MACAddr[1];
    netif->hwaddr[2] =  ETH_Handle.Init.MACAddr[2];
    netif->hwaddr[3] =  ETH_Handle.Init.MACAddr[3];
    netif->hwaddr[4] =  ETH_Handle.Init.MACAddr[4];
    netif->hwaddr[5] =  ETH_Handle.Init.MACAddr[5];

    /* maximum transfer unit */
    netif->mtu = NETIF_MTU;

    /* Accept broadcast address and ARP traffic */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
#if LWIP_ARP
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#else
    netif->flags |= NETIF_FLAG_BROADCAST;
#endif /* LWIP_ARP */

    /* create a binary semaphore used for informing ethernetif of frame reception */
    RxPktSemaphore = xSemaphoreCreateBinary();

    /* create a binary semaphore used for informing ethernetif of frame transmission */
    TxPktSemaphore = xSemaphoreCreateBinary();
    
    /* create the task that handles the ETH_MAC */
    osThreadDef(EthIf, ethernetif_input, osPriorityRealtime, 0, INTERFACE_THREAD_STACK_SIZE);
    osThreadCreate (osThread(EthIf), netif);  

    /* Get link state */
    bsp_lan8720_get_link_state(&ETH_Handle, &auto_negotiation, &LAN8720_State);   
    if(LAN8720_State == LAN8720_LINK_DOWN)
    {
        //printf("low_level_init->Link down\r\n");
        netif_set_link_down(netif);
        netif_set_down(netif);   
    }
    else
    {
        //printf("low_level_init->Link up\r\n");
        netif_set_up(netif);
        netif_set_link_up(netif);
    }

}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t errval = ERR_OK;
    uint32_t len;
    uint8_t *buff;
    ETH_DMADescTypeDef *desc;
    static uint32_t desc_index = 0;
    
    UNUSED(netif);
        
    desc = &DMATxDscrTab[desc_index];

    while (READ_BIT(desc->DESC0, ETH_DMA_TX_DESC0_OWN));
    
    if (p->tot_len > ETH_TX_BUFF_SIZE)
        return ERR_USE;
    
    buff = (uint8_t *)desc->Buff1;
    len = 0;
    
    while (p)
    {
        if (p->len)
        {
            memcpy((void *)buff, (void *)p->payload, p->len);
            len += p->len;
            buff += len;
        }
        p = p->next;
    }
    
    SET_BIT(ETH->DMAIER , ETH_DMAIER_NISE | ETH_DMAIER_TIE);
    WRITE_REG(desc->DESC0, ETH_DMA_TX_DESC0_IC | ETH_DMA_TX_DESC0_FS | ETH_DMA_TX_DESC0_LS | ETH_DMA_TX_DESC0_CIC | ETH_DMA_TX_DESC0_TCH);
    
    WRITE_REG(desc->DESC1, len  << ETH_DMA_TX_DESC1_TBS1_Pos);
    WRITE_REG(desc->DESC2, desc->Buff1);
    
    SET_BIT(desc->DESC0, ETH_DMA_TX_DESC0_OWN);
    
    ETH->DMASR = ETH_DMASR_TBUS;
    ETH->DMATPDR = 0;
    
    desc_index++;
    if (desc_index >= ETH_TX_DESC_CNT)
        desc_index = 0;

    while(osSemaphoreWait( TxPktSemaphore, ETHIF_TX_TIMEOUT) != osOK){};
 
    return errval;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
   */
static struct pbuf * low_level_input(struct netif *netif)
{
    uint8_t *buff;
    uint32_t len;
    struct pbuf *p;
    struct pbuf *q;
    ETH_DMADescTypeDef *desc;
    //static uint32_t desc_index = 0;
    
    UNUSED(netif);
    
    p = NULL;
    desc = &DMARxDscrTab[desc_index];
    
    if (READ_BIT(desc->DESC0, ETH_DMA_RX_DESC0_OWN) == 0)
    {
        desc_index++;
        if (desc_index >= ETH_RX_DESC_CNT)
            desc_index = 0;
        
        if ((READ_REG(desc->DESC0) & (ETH_DMA_RX_DESC0_FS | ETH_DMA_RX_DESC0_LS | ETH_DMA_RX_DESC0_ES)) == (ETH_DMA_RX_DESC0_FS | ETH_DMA_RX_DESC0_LS))
        {
            buff = (uint8_t *)desc->Buff1;
            len = (READ_REG(desc->DESC0) & ETH_DMA_RX_DESC0_FL) >> ETH_DMA_RX_DESC0_FL_Pos;

            p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
            q = p;
            while (q)
            {
                if (q->len)
                {
                    memcpy((void *)q->payload, (void *)buff, q->len);
                    buff += q->len;
                }
                q = q->next;
            }
        }
        
        WRITE_REG(desc->DESC2, desc->Buff1);
        WRITE_REG(desc->DESC3, desc->Buff2);
        SET_BIT(desc->DESC0, ETH_DMA_RX_DESC0_OWN);
        
        ETH->DMASR = ETH_DMASR_RBUS;
        ETH->DMARPDR = 0;
    }
    return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void ethernetif_input(void const* argument)
{
  struct pbuf *p = NULL;
  struct netif *netif = (struct netif *) argument;

  for( ;; )
  {
    if (osSemaphoreWait( RxPktSemaphore, TIME_WAITING_FOR_INPUT) == osOK)
    {
      do
      {
        p = low_level_input( netif );
        if (p != NULL)
        {
          if (netif->input( p, netif) != ERR_OK )
          {
            pbuf_free(p);
          }
        }
      } while(p!=NULL);
    }
  }
}

#if !LWIP_ARP
/**
 * This function has to be completed by user in case of ARP OFF.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if ...
 */
static err_t low_level_output_arp_off(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{
  err_t errval;
  errval = ERR_OK;



  return errval;

}
#endif /* LWIP_ARP */

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
    struct ethernetif *ethernetif;
        
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP
    netif->output = etharp_output;
#else
    /* The user should write its own code in low_level_output_arp_off function */
    netif->output = low_level_output_arp_off;
#endif /* LWIP_ARP */
#endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */

#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Current Time value
*/
u32_t sys_now(void)
{
    return HAL_GetTick();
}

/**
  * @brief  Check the ETH link state then update ETH driver and netif link accordingly.
  * @param  argument: netif
  * @retval None
  */
void ethernet_link_thread(void const * argument)
{
    uint32_t PHYLinkState = 0;
    uint32_t auto_negotiation;
    uint32_t linkchanged = 0U, speed = 0U, duplex = 0U;

    struct netif *netif = (struct netif *) argument;

    for(;;)
    {
        bsp_lan8720_get_link_state(&ETH_Handle, &auto_negotiation, &PHYLinkState);

        if(netif_is_link_up(netif) && (PHYLinkState == LAN8720_LINK_DOWN))
        {
            HAL_ETH_Stop(&ETH_Handle);
            netif_set_down(netif);
            netif_set_link_down(netif);
        }
        else if(!netif_is_link_up(netif) && (PHYLinkState > LAN8720_LINK_DOWN))
        {
            switch (PHYLinkState)
            {
            case LAN8720_LINK_100M_FULL_DUPLEX:
                duplex = ETH_MODE_FULL_DUPLEX;
                speed = ETH_SPEED_100M;
                linkchanged = 1;
                break;
            case LAN8720_LINK_100M_HALF_DUPLEX:
                duplex = ETH_MODE_HALF_DUPLEX;
                speed = ETH_SPEED_100M;
                linkchanged = 1;
                break;
            case LAN8720_LINK_10M_FULL_DUPLEX:
                duplex = ETH_MODE_FULL_DUPLEX;
                speed = ETH_SPEED_10M;
                linkchanged = 1;
                break;
            case LAN8720_LINK_10M_HALF_DUPLEX:
                duplex = ETH_MODE_HALF_DUPLEX;
                speed = ETH_SPEED_10M;
                linkchanged = 1;
                break;
            default:
                break;
            }

            if(linkchanged)
            {         
                ETH_Handle.Init.Speed = speed;
                ETH_Handle.Init.DuplexMode = duplex;
                HAL_ETH_SpeedConfig(&ETH_Handle);
                HAL_ETH_DuplexModeConfig(&ETH_Handle);
                HAL_ETH_Start(&ETH_Handle);
                
                netif_set_up(netif);
                netif_set_link_up(netif);
            }
        }

        osDelay(100);
    }
}

