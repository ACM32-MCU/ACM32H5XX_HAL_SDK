/* Includes ------------------------------------------------------------------*/
#include "tcpclient.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/opt.h"
#include <string.h>

#if LWIP_NETCONN
#include "lwip/sys.h"
#include "lwip/api.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TCPCLIENT_THREAD_PRIO  (osPriorityAboveNormal)

/* Private macro -------------------------------------------------------------*/
#define TCP_SV_IP_ADDR0   ((uint8_t)192U)
#define TCP_SV_IP_ADDR1   ((uint8_t)168U)
#define TCP_SV_IP_ADDR2   ((uint8_t)3U)
#define TCP_SV_IP_ADDR3   ((uint8_t)181U)

#define TCP_SV_PORT       ((uint16_t)5001U)

/* Private variables ---------------------------------------------------------*/
const char *tcpdata = "Hello TCP Server";

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void tcpclient_thread(void *arg)
{
    struct netconn *conn;
    ip_addr_t dstipaddr;  
    err_t err;
    LWIP_UNUSED_ARG(arg);
    
    while (1) {       
        /* Create a new connection identifier. */
        conn = netconn_new(NETCONN_TCP);
        LWIP_ERROR("tcpclient: invalid conn", (conn != NULL), return;);
        
        IP_ADDR4(&dstipaddr,TCP_SV_IP_ADDR0,TCP_SV_IP_ADDR1,TCP_SV_IP_ADDR2,TCP_SV_IP_ADDR3);

        while(netconn_connect(conn, &dstipaddr, TCP_SV_PORT) != ERR_OK) {
            osDelay(500);
        }

        netconn_write(conn, (void *)tcpdata, strlen(tcpdata), NETCONN_NOFLAG);

        /* Close connection and discard connection identifier. */
        netconn_close(conn);
        netconn_delete(conn);  
        
        osDelay(2000);     
    }
}
/*-----------------------------------------------------------------------------------*/
void
tcpclient_init(void)
{
    sys_thread_new("tcpclient_thread", tcpclient_thread, NULL, (configMINIMAL_STACK_SIZE*2), TCPCLIENT_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
