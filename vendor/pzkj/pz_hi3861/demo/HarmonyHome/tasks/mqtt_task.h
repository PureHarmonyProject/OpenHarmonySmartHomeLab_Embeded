#ifndef __MQTT_TASK_H__
#define __MQTT_TASK_H__

#include <stdint.h>
#include <hi_types_base.h>

typedef struct message_sensorData {
    uint32_t led_lightness_color;
    uint8_t curtain_percent;
    hi_bool curtain_openstate;
    uint8_t door_state;
    uint8_t temperature_indoor;
    uint8_t humidity_indoor;  
    uint32_t smoke;           // 烟雾传感器数据
    uint32_t comb;           // 可燃气体数据
    uint32_t light;           // 亮度数据
    hi_bool beep_state; // 蜂鸣器当前的状态
    uint8_t airConditioner_state; // 空调当前的状态
    uint32_t current; // 当前电流 ua
    uint32_t voltage; // 当前电压 uv
    uint32_t power; // 当前功率 uw
    uint32_t automation_mode_scene;
} msg_sensorData_t;

//WIFI连接热点和密码
#define WIFI_SSID "CMCC-92B9"
#define WIFI_PAWD "12345678@kd"

//注册华为云后需要更新的信息
// 产品ID
#define DEVICE_ID "67cc67268a63120625169e01"
// MQTT客户端ID
#define MQTT_CLIENT_ID "67cc67268a63120625169e01_Home_hi3861_0_0_2025030908" 
// MQTT用户名
#define MQTT_USER_NAME "67cc67268a63120625169e01_Home_hi3861"
// MQTT密码
#define MQTT_PASS_WORD "a5e4a9d80674a8d28007eedf518d3ce92ba56b0581cfb2e39eb40c0648784ab3"
// 华为云平台的IP地址
#define SERVER_IP_ADDR "124.70.218.131"
// 华为云平台的IP端口号
#define SERVER_IP_PORT 1883
// 订阅 接收控制命令的主题
#define MQTT_TOPIC_SUB_COMMANDS "$oc/devices/%s/sys/commands/#"
// 发布 成功接收到控制命令后的主题
#define MQTT_TOPIC_PUB_COMMANDS_REQ "$oc/devices/%s/sys/commands/response/request_id=%s"
#define MALLOC_MQTT_TOPIC_PUB_COMMANDS_REQ "$oc/devices//sys/commands/response/request_id="

// 发布 设备属性数据的主题
#define MQTT_TOPIC_PUB_PROPERTIES "$oc/devices/%s/sys/properties/report"
#define MALLOC_MQTT_TOPIC_PUB_PROPERTIES "$oc/devices//sys/properties/report"

#define TASK_STACK_SIZE (1024 * 10)
#define MsgQueueObjectNumber 16 // 定义消息队列对象的个数

#define MQTT_SEND_TASK_TIME 3 // s
#define MQTT_RECV_TASK_TIME 1 // s
#define TASK_INIT_TIME 2 // s
#define DISPLAY_BUFF_MAX 64
#define MQTT_DATA_MAX 256

void wifi_iotda_task_create(void);
int8_t mqttClient_sub_callback(unsigned char *topic, unsigned char *payload);

#endif // __MQTT_TASK_H__
