#ifndef BSP_WIFI_H
#define BSP_WIFI_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"
#include "wifi_error_code.h"
#include "wifi_device.h"



//函数声明
WifiErrorCode WiFi_createHotspots(const char *ssid, const char *psk);
WifiErrorCode WiFi_connectHotspots(const char *ssid, const char *psk);
char* WiFi_GetLocalIP(void);

#endif
