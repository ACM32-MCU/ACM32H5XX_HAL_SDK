/******************************************************************************
*@file  : main.c
*@brief : main program
******************************************************************************/
#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "tcpecho.h"
#include "udpecho.h"
#include "tcpclient.h"
#include "udpclient.h"
#include "app_ethernet.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif gnetif; /* network interface structure */

/* Private function prototypes -----------------------------------------------*/
static void StartThread(void const * argument);
static void Netif_Config(void);

/******************************************************************************
*@brief : main program
*@param : none
*@return: none
******************************************************************************/
int main(void)
{
    HAL_Init();

    SystemClock_Config(SYSCLK_SELECT, PCLK1_DIV_SELECT, PCLK2_DIV_SELECT, PCLK3_DIV_SELECT, PCLK4_DIV_SELECT);

    BSP_UART_Init(USART1, 115200);

    /* Init thread */
    osThreadDef(Start, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 4);
    osThreadCreate (osThread(Start), NULL);

    printfS("\r\n\r\n");
    printfS("************************************************************\r\n\r\n");
    printfS("system startup\r\n");

    get_reset_source();

    printfS("HCK: %u\r\n", HAL_RCC_GetHCLKFreq());
    printfS("PCLK1: %u\r\n", HAL_RCC_GetPCLK1Freq());
    printfS("PCLK2: %u\r\n", HAL_RCC_GetPCLK2Freq());
    printfS("PCLK3: %u\r\n", HAL_RCC_GetPCLK3Freq());
    printfS("PCLK4: %u\r\n", HAL_RCC_GetPCLK4Freq());
    printfS("\r\n");
    printfS("************************************************************\r\n\r\n");

    /* Start scheduler */
    osKernelStart();
     
    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    while(1)
    {
    };
}

/**
  * @brief  Start Thread
  * @param  argument not used
  * @retval None
  */
static void StartThread(void const * argument)
{
    /* Create tcp_ip stack thread */
    tcpip_init(NULL, NULL);

    /* Initialize the LwIP stack */
    Netif_Config();

    /* Initialize the TCP echo server thread */
    tcpecho_init();
    /* Initialize the UDP echo server thread */
    udpecho_init();
    /* Initialize the TCP client thread */
    tcpclient_init();
    /* Initialize the UDP client thread */
    udpclient_init();
    
    for( ;; )
    {
        /* Delete the Init Thread */
        osThreadTerminate(NULL);
    }
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    uint32_t PHYLinkState = 0;
    uint32_t auto_negotiation;
      
#if LWIP_DHCP
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
#else
    IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
    IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
    IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* LWIP_DHCP */

    /* add the network interface */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

    /* Registers the default network interface */
    netif_set_default(&gnetif);

    ethernet_link_status_updated(&gnetif);

#if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status*/
    netif_set_link_callback(&gnetif, ethernet_link_status_updated);

    /* Create the Ethernet link handler thread */
    osThreadDef(EthLink, ethernet_link_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE *2);
    osThreadCreate (osThread(EthLink), &gnetif);
#endif /* LWIP_NETIF_LINK_CALLBACK */
    
#if LWIP_DHCP
    /* Start DHCPClient */
    osThreadDef(DHCP, DHCP_Thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
    osThreadCreate (osThread(DHCP), &gnetif);
#endif /* LWIP_DHCP */
}

