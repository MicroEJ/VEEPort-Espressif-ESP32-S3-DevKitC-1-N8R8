/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief lwip_util implementation over LWIP and FreeRTOS.
 * @author MicroEJ Developer Team
 * @version 0.2.0
 * @date 1 February 2018
 */

#include "lwip_util.h"

#include "LLNET_CONF.h"
#include "LLNET_Common.h"

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "netif/etharp.h"
#include "lwip/esp_netif_net_stack.h"
#include <netdb.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Sanity check. Synchronization has to be activated in LWIP configuration file.
#if (SYS_LIGHTWEIGHT_PROT != 1)
	#error LWIP synchronization not activated in lwipopts.h file. Please add "#define SYS_LIGHTWEIGHT_PROT	1" in lwipopts.h header file.
#endif

#define MAX_DHCP_TRIES  4
#define LWIP_LINK_TASK_PRIORITY 9
#define LWIP_DHCP_TASK_PRIORITY 8

#define LWIP_DHCP_POLLING_INTERVAL 250

/* DHCP process states */
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5

struct netif gnetif;
uint8_t DHCP_state;

#if LWIP_NETIF_LINK_CALLBACK
static TaskHandle_t	link_task_handle;
#endif
static TaskHandle_t	dhcp_task_handle;

static uint8_t dhcp_sleeping = 1;

/* variable used to notify that DNS servers list has changed */
uint8_t dns_servers_list_updated = 1;

char lwip_netif[NUMB_OF_NETIF_TO_STORE][MAX_SIZE_OF_NETIF_NAME];

/**
  * @brief  Reset the network interface ip, netmask and gateway addresses to zero.
  * @param  netif: the network interface
  * @retval None
  */
static void netif_addr_set_zero_ip4(struct netif* netif){
	ip_addr_set_zero_ip4(&netif->ip_addr);
	ip_addr_set_zero_ip4(&netif->netmask);
	ip_addr_set_zero_ip4(&netif->gw);
}

/**
  * @brief  This function is called when the network interface is disconnected.
  * @param  netif: the network interface
  * @retval None
  */
void netif_not_connected(struct netif *netif){
	netif_addr_set_zero_ip4(netif);
	LLNET_DEBUG_TRACE("[INFO] The network cable is not connected \n");
}

/**
  * @brief  DHCP Process
* @param  argument: network interface
  * @retval None
  */
static void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  uint32_t IPaddress;

  for (;;)
  {
  	// check if DHCP thread has to suspend
	if(dhcp_sleeping == 1){
		vTaskSuspend(NULL);
	}

    switch (DHCP_state)
    {
    case DHCP_START:
      {
        netif_addr_set_zero_ip4(netif);
        IPaddress = 0;
        dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;

        LLNET_DEBUG_TRACE("[INFO] DHCP started\n");
      }
      break;

    case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = ip_2_ip4(&(netif->ip_addr))->addr;

        if (IPaddress!=0)
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;

          /* Stop DHCP */
          dhcp_stop(netif);
		  dhcp_sleeping = 1;

		  LLNET_DEBUG_TRACE("[INFO] DHCP address assigned: %s\n", inet_ntoa(IPaddress));

					// set static DNS configuration if required by user
					if(LLNET_CONF_is_dns_dhcp_enabled() != LLNET_CONF_TRUE)
					{
						ip_addr_t dnsaddr;
						if(DNS_MAX_SERVERS > 0)
						{
							char * static_dns_ip_addr = LLNET_CONF_get_dns_ip_address();
							if(static_dns_ip_addr != NULL)
							{
								dnsaddr.type = IPADDR_TYPE_V4;
								ip_2_ip4(&dnsaddr)->addr = inet_addr(static_dns_ip_addr);
								dns_setserver(0, &dnsaddr);
							}
						}
					}

					// notify DNS servers IP address updated
					dns_servers_list_updated = 1;
        }
        else
        {
#if LWIP_VERSION_MAJOR == 1

        	struct dhcp *dhcp = netif->dhcp;

#elif LWIP_VERSION_MAJOR == 2

			struct dhcp *dhcp = (struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

#else

	#error "Invalid LWIP version (LWIP_VERSION_MAJOR)."

#endif

			/* DHCP timeout */
			if (dhcp->tries > MAX_DHCP_TRIES)
			{
				DHCP_state = DHCP_TIMEOUT;

				/* Stop DHCP */
				dhcp_stop(netif);
				dhcp_sleeping = 1;

				LLNET_DEBUG_TRACE("[INFO] DHCP timeout\n");
			}
        }
      }
      break;

    default: break;
    }

    /* wait 250 ms */
	TickType_t ticks = LWIP_DHCP_POLLING_INTERVAL / portTICK_PERIOD_MS;
    vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
  }
}

/**
 * Network initialization. Start network interfaces and configure it.
 * @return 0 if no error occurred, error code otherwise.
 */
int32_t llnet_lwip_init(void)
{
	int32_t dhcpConfEnabled =  LLNET_CONF_is_ip_dhcp_enabled() == LLNET_CONF_TRUE;

	/* Initialize the LwIP TCP/IP stack */
	/* Configure the Network interface */
	/* done by LLECOM_WIFI_initialize() */

	if(dhcpConfEnabled)
	{
		/* Start DHCPClient */
		dhcp_sleeping = 0;
#if defined(__GNUC__)
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE * 5, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#else
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE * 2, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#endif
	}

	return 0;
}

void set_lwip_netif_name(int32_t id, char *netif_name)
{
	strcpy(lwip_netif[id], netif_name);
}

char *get_lwip_netif_name(int32_t id)
{
	return lwip_netif[id];
}

struct netif* getNetworkInterface(int8_t* name)
{
	char *llnet_netif[] = LLNET_NETIF_NAMES;
	for (int32_t netif_idx = 0; netif_idx < (sizeof(llnet_netif) / sizeof(llnet_netif[0])); netif_idx++) {
		if (strcmp((char *)name, llnet_netif[netif_idx]) == 0) {
			// the same interface name at LLNET level can have different names at platform level
			// so, keep trying to find the lwip level interface until one is found or until we exhaust the list
			struct netif *ret = netif_find(lwip_netif[netif_idx]);
			if (ret) {
				return ret;
			}
		}
	}
	return NULL;
}
