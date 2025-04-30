/* Includes ------------------------------------------------------------------*/
#include "udpclient.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/opt.h"
#include <string.h>

#if LWIP_NETCONN
#include "lwip/sys.h"
#include "lwip/api.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UDPCLIENT_THREAD_PRIO  (osPriorityAboveNormal)

/* Private macro -------------------------------------------------------------*/
#define UDP_SV_IP_ADDR0   ((uint8_t)192U)
#define UDP_SV_IP_ADDR1   ((uint8_t)168U)
#define UDP_SV_IP_ADDR2   ((uint8_t)3U)
#define UDP_SV_IP_ADDR3   ((uint8_t)181U)

#define UDP_SV_PORT       ((uint16_t)5001U)

/* Private variables ---------------------------------------------------------*/
const char *udpdata = "Hello UDP Server";

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void udpclient_thread(void *arg)
{
    struct netconn *conn;
    struct netbuf *tx_buf;
    ip_addr_t dstipaddr;
    err_t err;
    LWIP_UNUSED_ARG(arg);

    /* Create a new connection identifier. */
    conn = netconn_new(NETCONN_UDP);
    LWIP_ERROR("udpclient: invalid conn", (conn != NULL), return;);

    IP_ADDR4(&dstipaddr,UDP_SV_IP_ADDR0,UDP_SV_IP_ADDR1,UDP_SV_IP_ADDR2,UDP_SV_IP_ADDR3);
    
    while (1) {
        tx_buf = netbuf_new();
        netbuf_alloc(tx_buf, strlen(udpdata));
        
        pbuf_take(tx_buf->p, (const void *)udpdata, strlen(udpdata));
        
        err = netconn_sendto(conn, tx_buf, (const ip_addr_t *)&dstipaddr, UDP_SV_PORT);
        if(err != ERR_OK) {
          LWIP_DEBUGF(LWIP_DBG_ON, ("netconn_send failed: %d\n", (int)err));
        } else {
          LWIP_DEBUGF(LWIP_DBG_ON, ("got %s\n", buffer));
        }
        netbuf_delete(tx_buf);
        
        osDelay(2000);
    }
}
/*-----------------------------------------------------------------------------------*/
void
udpclient_init(void)
{
    sys_thread_new("udpclient_thread", udpclient_thread, NULL, (configMINIMAL_STACK_SIZE*2), UDPCLIENT_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
