/**
 ******************************************************************************
  * File Name          : ethernetif.h
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  */

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"

#define NETIF_MTU								      ( 1500 )

/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);

void ethernetif_input(void const * argument);
void ethernet_link_thread(void const * argument );

void Error_Handler(void);
u32_t sys_jiffies(void);
u32_t sys_now(void);


#endif
