#include "./include/wifi_ap_sta.h"

#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	wifi_ap_record_t my_wifi_info;
	
    switch(event->event_id) 
	{
		case SYSTEM_EVENT_STA_START:
			printf("[WIFI AP+STA] STA start event!\r\n");
			ESP_ERROR_CHECK(esp_wifi_connect());
			break;
		case SYSTEM_EVENT_STA_GOT_IP:
			printf("[WIFI AP+STA] Get IP event!\r\n");
			printf("[WIFI AP+STA] ESP32 IP: %s !\r\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
			break;
		case SYSTEM_EVENT_STA_CONNECTED:
			printf("[WIFI AP+STA] Wifi STA connect event!\r\n");
			esp_wifi_sta_get_ap_info(&my_wifi_info);
			printf("[WIFI AP+STA] Connect to : %s!\r\n", my_wifi_info.ssid);
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
			printf("[WIFI AP+STA] Wifi STA disconnect event, reconnect!\r\n");
			ESP_ERROR_CHECK( esp_wifi_connect() );
			break;
		case SYSTEM_EVENT_AP_START:
			printf("[WIFI AP+STA] AP start event!\r\n");

			//ESP_LOGI(TAG, "station:"MACSTR" join,AID=%d\n",
			//MAC2STR(event->event_info.sta_connected.mac),
			//event->event_info.sta_connected.aid);
			//xEventGroupSetBits(udp_event_group, WIFI_CONNECTED_BIT);
			break;
		case SYSTEM_EVENT_AP_STACONNECTED:
			printf("[WIFI AP+STA] A station connected to ESP32 soft-AP!\r\n");
			//ESP_LOGI(TAG, "station:"MACSTR" join,AID=%d\n",
			//MAC2STR(event->event_info.sta_connected.mac),
			//event->event_info.sta_connected.aid);
			//xEventGroupSetBits(udp_event_group, WIFI_CONNECTED_BIT);
			break;
		case SYSTEM_EVENT_AP_STADISCONNECTED:
			printf("[WIFI AP+STA] A station disconnected from ESP32 soft-AP!\r\n");
			//ESP_LOGI(TAG, "station:"MACSTR"leave,AID=%d\n",
			//MAC2STR(event->event_info.sta_disconnected.mac),
			//event->event_info.sta_disconnected.aid);
			//xEventGroupClearBits(udp_event_group, WIFI_CONNECTED_BIT);
			break;
		default:
			break;
    }
	
    return ESP_OK;	
}


void user_wifi_config_ap_sta(void)
{
	printf("[WIFI AP+STA] softAP+station config start!\r\n");
	printf("[WIFI AP+STA] connect to Wifi : %s \r\n", WIFI_SSID);
	printf("[WIFI AP+STA] ESP32 AP SSID: %s  PASSWORD: %s \r\n", ESP32_AP_SSID, ESP32_AP_PASS);
	
    tcpip_adapter_init();
	
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
	
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	
	// STA
    wifi_config_t wifi_sta_config = 
	{
		.sta = 
		{
			.ssid = WIFI_SSID,
			.password = WIFI_PASS
		},
    };
	
	// AP
	wifi_config_t wifi_ap_config = 
	{
		.ap = 
		{
            .ssid = ESP32_AP_SSID,                 // ESP32 wifi soft-AP
			.password = ESP32_AP_PASS,
            .ssid_len = strlen(ESP32_AP_SSID),
			.channel = ESP32_AP_CHANNEL,
            .max_connection = ESP32_MAX_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
	
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_APSTA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_sta_config) );
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_ap_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );	
}