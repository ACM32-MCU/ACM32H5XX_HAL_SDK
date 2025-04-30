/******************************************************************************
*@file  : app.c
*@brief : application
******************************************************************************/

#include "app.h"
#include "ethernetif.h"
#include "bsp_eth.h"
#include "bsp_lan8720.h"
#include "tcp_client.h"

#include <lwip/opt.h>
#include <lwip/arch.h>
#include "tcpip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/sio.h"
#include "ethernetif.h"
#include "netif/ethernet.h"
#include "lwip/def.h"
#include "lwip/stats.h"
#include "lwip/etharp.h"
#include "lwip/ip.h"
#include "lwip/snmp.h"
#include "lwip/timeouts.h"
#include "lwip/dhcp.h"

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;

uint8_t  LOCAL_ADDR[4] = {192, 168, 1, 20};
uint16_t LOCAL_PORT = 5001;

uint8_t  DEST_ADDR[4] = {192, 168, 1, 181};
uint16_t DEST_PORT = 5001;

uint8_t  GATEWAY_ADDR[4] = {192, 168, 1, 1};
uint8_t  NETMASK_ADDR[4] = {255, 255, 255, 0};

volatile uint32_t g_eth_recv_complete;

void LwIP_Init(void);

void APP_Test(void)
{
    LwIP_Init();  
    tcp_client_init();
    
    while (1)
    {
        if(g_eth_recv_complete)
        {
            g_eth_recv_complete = 0;
            ethernetif_input(&gnetif);
        }
        sys_check_timeouts();
    }
}

void LwIP_Init(void)
{
    /* IP addresses initialization */

    #if LWIP_DHCP
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
    #else
    IP4_ADDR(&ipaddr, LOCAL_ADDR[0], LOCAL_ADDR[1], LOCAL_ADDR[2], LOCAL_ADDR[3]);
    IP4_ADDR(&netmask, NETMASK_ADDR[0], NETMASK_ADDR[1], NETMASK_ADDR[2], NETMASK_ADDR[3]);
    IP4_ADDR(&gw, GATEWAY_ADDR[0], GATEWAY_ADDR[1], GATEWAY_ADDR[2], GATEWAY_ADDR[3]);
    #endif
    
    /* Initilialize the LwIP stack without RTOS */
    lwip_init();

    /* add the network interface (IPv4/IPv6) without RTOS */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /* Registers the default network interface */
    netif_set_default(&gnetif);
    
    while (ethernetif_check_link_state(&gnetif));
    
    if (netif_is_link_up(&gnetif))
    {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&gnetif);
    }
    else
    {
        /* When the netif link is down this function must be called */
        netif_set_down(&gnetif);
    }
    
    #if LWIP_DHCP
    //enable dhcp
    if (dhcp_start(&gnetif) == ERR_OK)
    {
        printfS("lwip dhcp init success.\r\n");
    }
    else
    {
        printfS("lwip dhcp init fail.\r\n");
    }
    while (ip_addr_cmp(&(gnetif.ip_addr), &ipaddr))   // Wait for IP to be valid.
    {
        if(g_eth_recv_complete)
        {
            g_eth_recv_complete = 0;
            // Call the network card receiving function
            ethernetif_input(&gnetif);
        }
        // Handle timed event in LwIP
        sys_check_timeouts();
    }
    #endif
    
    printfS("Local IP address:%u.%u.%u.%u\n\n",  (uint32_t)((gnetif.ip_addr.addr)&0x000000ff),           \
                                                 (uint32_t)(((gnetif.ip_addr.addr)&0x0000ff00) >> 8),    \
                                                 (uint32_t)(((gnetif.ip_addr.addr)&0x00ff0000) >> 16),   \
                                                 (uint32_t)((gnetif.ip_addr.addr)&0xff000000) >> 24);

}

void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    UNUSED(heth);
    
    g_eth_recv_complete = 1;
}
 