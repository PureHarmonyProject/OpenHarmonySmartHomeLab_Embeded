#include "bsp_init.h"
#include "template.h"
#include "mqtt_task.h"

extern int8_t mqttClient_sub_callback(unsigned char *topic, unsigned char *payload); // mqtt 回调声明

void bsp_init(void)
{
    // //LED初始化
    // led_init();

    // //蜂鸣器初始化
    beep_init();

    //oled初始化
    printf("OLED is initing !!!\r\n");
    oled_init();
    printf("OLED init success !!!\r\n");

    const char *word = "HOme";
    uint8_t font_size = 24;     // 可改为 16 或 12
    uint8_t char_width = 12;    // 每个字宽度（与字体大小对应）
    uint8_t word_len = strlen(word);
    uint8_t total_width = word_len * char_width;

    uint8_t x = (128 - total_width) / 2;
    uint8_t y = (64 - font_size) / 2;

    oled_clear();
    oled_showstring(x, y, (uint8_t *)word, font_size);
	char status[32];
	snprintf(status, sizeof(status), "System Is Initing!");
    oled_showstring_center(52, (uint8_t *)status, 12);
    oled_refresh_gram();

    // // 温湿度传感器初始化
    printf("DHT11 is initing !!!\r\n");
    while(dht11_init())
	{
		printf("DHT11检测失败,请插好!\r\n");
		osDelay(TASK_DELAY_100MS); //100ms
	}
	printf("DHT11检测成功!\r\n");

    // //电流电压传感器初始化 也需要做初始化检测处理
    ina226_init();


    // //人体感应初始化
    printf("SR501 is initing !!!\r\n");
    sr501_init();
    printf("SR501 init success !!!\r\n");
    // //烟雾传感器初始化
    printf("SMOKE is initing !!!\r\n");
    smoke_init();
    printf("SMOKE init success !!!\r\n");

    printf("MQ5 is initing !!!\r\n");
    MQ5_init();
    printf("MQ5 init success !!!\r\n");
    
    //iotda初始化
    // 初始化MQTT回调函数
    printf("IOTDA is initing !!!\r\n");
    p_MQTTClient_sub_callback = &mqttClient_sub_callback;

    // 连接WiFi
    if (WiFi_connectHotspots(WIFI_SSID, WIFI_PAWD) != WIFI_SUCCESS) {
        printf("[error] connectWiFiHotspots\r\n");
    }
    sleep(TASK_INIT_TIME);

    // 连接MQTT服务器
    if (MQTTClient_connectServer(SERVER_IP_ADDR, SERVER_IP_PORT) != WIFI_SUCCESS) {
        printf("[error] mqttClient_connectServer\r\n");
    }
    sleep(TASK_INIT_TIME);

    // 初始化MQTT客户端
    if (MQTTClient_init(MQTT_CLIENT_ID, MQTT_USER_NAME, MQTT_PASS_WORD) != WIFI_SUCCESS) {
        printf("[error] mqttClient_init\r\n");
    }
    sleep(TASK_INIT_TIME); 

    // 订阅主题
    if (MQTTClient_subscribe(MQTT_TOPIC_SUB_COMMANDS) != WIFI_SUCCESS) {
        printf("[error] mqttClient_subscribe\r\n");
    }
    printf("IOTDA init success !!!\r\n");
    sleep(TASK_INIT_TIME);

    printf("all bsp init success !!!\r\n");

}
