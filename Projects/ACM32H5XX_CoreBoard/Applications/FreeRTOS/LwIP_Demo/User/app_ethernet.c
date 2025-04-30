/**
  ******************************************************************************
  * @file    app_ethernet.c 
  * @author  
  * @brief   Ethernet specific module
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "main.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif
#include "app_ethernet.h"
#include "ethernetif.h"
#include "lwip/netifapi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if LWIP_DHCP
#define MAX_DHCP_TRIES  4
__IO uint8_t DHCP_state = DHCP_OFF;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Notify the User about the network interface config status
  * @param  netif: the network interface
  * @retval None
  */
void ethernet_link_status_updated(struct netif *netif)
{
    if (netif_is_up(netif))
    {
#if LWIP_DHCP
        /* Update DHCP state machine */
        DHCP_state = DHCP_START;
        uint8_t iptxt[20];
        sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
        printf("Static IP address: %s\n", iptxt);
#endif /* LWIP_DHCP */
    }
    else
    {
#if LWIP_DHCP
        /* Update DHCP state machine */
        DHCP_state = DHCP_LINK_DOWN;
        printf("The network cable is not connected \n");
#endif /* LWIP_DHCP */
    }
}

#if LWIP_DHCP
/**
  * @brief  DHCP Process
  * @param  argument: network interface
  * @retval None
  */
void DHCP_Thread(void const * argument)
{
    struct netif *netif = (struct netif *) argument;
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    struct dhcp *dhcp;
    uint8_t iptxt[20];

    for (;;)
    {
        switch (DHCP_state)
        {
        case DHCP_START:
            ip_addr_set_zero_ip4(&netif->ip_addr);
            ip_addr_set_zero_ip4(&netif->netmask);
            ip_addr_set_zero_ip4(&netif->gw);
            DHCP_state = DHCP_WAIT_ADDRESS;
            printf("  State: Looking for DHCP server ...\n");

            netifapi_dhcp_start(netif);
            break;
        
        case DHCP_WAIT_ADDRESS:
            if (dhcp_supplied_address(netif))
            {
                DHCP_state = DHCP_ADDRESS_ASSIGNED;
                sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
                printf("IP address assigned by a DHCP server: %s\n", iptxt);
            }
            else
            {
                dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

                /* DHCP timeout */
                if (dhcp->tries > MAX_DHCP_TRIES)
                {
                    DHCP_state = DHCP_TIMEOUT;

                    /* Static address used */
                    IP_ADDR4(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
                    IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
                    IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
                    netifapi_netif_set_addr(netif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));

                    sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
                    printf ("DHCP Timeout !! \n");
                    printf ("Static IP address: %s\n", iptxt);
                }
            }
            break;
            
        case DHCP_LINK_DOWN:
            DHCP_state = DHCP_OFF;
            printf("The network cable is not connected \n");
            break;
        
        default: break;
        }

        /* wait 500 ms */
        osDelay(500);
    }
}
#endif  /* LWIP_DHCP */


