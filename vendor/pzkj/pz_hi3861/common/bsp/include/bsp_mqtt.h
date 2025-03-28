#ifndef BSP_MQTT_H
#define BSP_MQTT_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"


//函数声明
int MQTTClient_connectServer(const char *ip_addr, int ip_port);
int MQTTClient_unConnectServer(void);
int MQTTClient_subscribe(char *subTopic);
int MQTTClient_init(char *clientID, char *userName, char *password);
int MQTTClient_pub(char *pub_Topic, unsigned char *payloadData, int payloadLen);
int MQTTClient_sub(void);
extern int8_t(*p_MQTTClient_sub_callback)(unsigned char *topic, unsigned char *payload);

#endif
