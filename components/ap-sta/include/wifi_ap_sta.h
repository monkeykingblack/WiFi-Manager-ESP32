#ifndef __WIFI_AP_STA_H__
#define __WIFI_AP_STA_H__

#ifdef __cplusplus
extern "C" {
#endif


// STA
#define WIFI_SSID               "IOTMAKER.VN"
#define WIFI_PASS               "@iotmaker.vn134"

// AP
#define ESP32_AP_SSID           "espwifi256"
#define ESP32_AP_PASS           "esp123456"
#define ESP32_AP_CHANNEL        3
#define ESP32_MAX_CONN          3


void user_wifi_config_ap_sta(void);


#ifdef __cplusplus
}
#endif

#endif /*#ifndef __USER_WIFI_APSTA_H__*/