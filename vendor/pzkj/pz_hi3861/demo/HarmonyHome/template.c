#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_oled.h"
#include "bsp_step_motor.h"
#include "bsp_sr501.h"
#include "bsp_dc_motor.h"
#include "bsp_sg90.h"
#include "bsp_smoke.h"
#include "bsp_ina219.h"
#include "bsp_uart.h"
// #include "bsp_ds18b20.h"

#include "bsp_wifi.h"
#include "bsp_mqtt.h"

#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "lwip/api_shell.h"

#include "cJSON.h"
#define TASKDLYTICK    500
#define LONGDELAYTICK  100    //100 = 1000ms 1tick = 10ms
#define SHORTDELAYTICK 10    //10 = 100ms 1tick = 10ms

osThreadId_t Test_Task_ID; //任务ID

void test_task(void)
{
    step_motor_init();
    sg90_init();
    beep_init();
    // led_init();
    while (1) 
    {
        // led_on();
        printf("窗帘打开\n");
        curtain_open();
        usleep(500 * 1000);
        printf("门打开\n");
        door_open();
        usleep(500 * 1000);
        printf("门关闭\n");
        door_close();
        usleep(500 * 1000);
        printf("蜂鸣器响\n");
        beep_warning();
        usleep(500 * 1000);
    }
}

void test_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "test_task";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal5; // 任务的优先级

    Test_Task_ID = osThreadNew((osThreadFunc_t)test_task, NULL, &taskOptions); // 创建任务
    if (Test_Task_ID != NULL)
    {
        printf("ID = %d, Test_Task_ID Create OK!\n", Test_Task_ID);
    }
}

/**************************************************检测类任务******************************************************** */

//人体感应任务 检测到人后报警
osThreadId_t MOTION_Task_ID; //任务ID

void motion_sensor_task(void)
{
    int flag = 0;
    int flag1 = 0;
    int value = 0, prevalue = 0;
    while (1) 
    {
        value = sr501_check();

        if(value != prevalue) {
            printf("prevalue = %d, value = %d\n",prevalue, value);
            if(value == 1) {
                printf("[MOTION] 检测到人体\n");
                led_on();  // 打开LED
            }else {
                printf("[MOTION] 人已离开\n");
                led_off();
            }
        }

        prevalue = value;
        // if (sr501_check()) {
        //     // if(flag == 0) {
        //     //     flag1 = 1;
        //     // }else if(flag1 == 1) {
        //     //     flag1 = 0;
        //     usleep(2000 * 1000);
        //     if(sr501_check()) {
        //         printf("[MOTION] 检测到人体\n");
        //         led_on();  // 打开LED
        //     }
        //     // }
        // }else {
        //     if(flag == 0) {
        //         flag = 1;
        //     }else if(flag == 1) {
        //         flag = 2;
        //     }else if (flag == 2) {
        //         flag = 0;
        //         led_off();
        //         printf("[MOTION] 人已离开\n");
        //     }
            
        // }
        // while (sr501_check())
        // {
        //     led_on();
        // }
        // led_off();
        osDelay(100);  // 延时100ms进行下一次检查
    }
}

void motion_sensor_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "motionTask";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal3; // 任务的优先级

    MOTION_Task_ID = osThreadNew((osThreadFunc_t)motion_sensor_task, NULL, &taskOptions); // 创建任务
    if (MOTION_Task_ID != NULL)
    {
        printf("ID = %d, MOTION_Task_ID Create OK!\n", MOTION_Task_ID);
    }
}

// 9. 烟雾传感器任务 有烟时报警
osThreadId_t SMOKE_Task_ID; //任务ID

void smoke_sensor_task(void)
{
    while (1) 
    {
        if(smoke_check()) {
            printf("[SMOKE] 检测到烟雾\n");
            led_warning();   // 打开LED和蜂鸣器
            beep_warning(); 
        }else {
            printf("[SMOKE] 没有检测到烟雾\n");
        }
        osDelay(100);  // 延时100ms进行下一次检查
    }
}

//任务创建
void smoke_sensor_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "ModuleTask";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal2; // 任务的优先级

    SMOKE_Task_ID = osThreadNew((osThreadFunc_t)smoke_sensor_task, NULL, &taskOptions); // 创建任务1
    if (SMOKE_Task_ID != NULL)
    {
        printf("ID = %d, Create SMOKE_Task_ID is OK!\n", SMOKE_Task_ID);
    }
}

/**********************************************************************************************************************/

/**************************************************自动化任务******************************************************** */

// 10. sensor更新 包含温湿度传感器、电压传感器数据，在oled上更新
osMutexId_t oled_mutex;  // 用于控制OLED的互斥锁

void oled_init_mutex(void) {
    osMutexAttr_t mutex_attr = {0};
    oled_mutex = osMutexNew(&mutex_attr);  // 初始化OLED的互斥锁
}

osThreadId_t Sensor_Task_ID; // INA219任务ID

uint8_t temp, humi;
// 传感器、电流电压更新
void display_sensor_data(void)
{
    char display_buffer[128];
    
    int16_t current;
    // uint16_t bus_voltage, shunt_voltage;
    
    // 获取温湿度数据
    // dht11_read_data(&temp, &humi);
    
    // // 获取电流电压数据
    // current = ina219_read_current();
    // bus_voltage = ina219_read_bus_voltage();
    // shunt_voltage = ina219_read_shunt_voltage();

    // snprintf(display_buffer, sizeof(display_buffer), 
    //          "Temp: %dC Humidity: %d%%\nCurrent: %d mA\nBus Voltage: %d mV", 
    //          temp, humi, current, bus_voltage);

    snprintf(display_buffer, sizeof(display_buffer), 
             "Temp: %dC Humidity: %d%%\n", 
             temp, humi, current);
    
    // 使用互斥锁，防止OLED同时被多个任务操作
    osMutexAcquire(oled_mutex, osWaitForever);  
    oled_clear();
    oled_showstring(0, 0, display_buffer, 12);  // 显示数据
    oled_refresh_gram();
    osMutexRelease(oled_mutex);  // 释放OLED互斥锁
}

void sensor_task(void)
{
    while (1) 
    {
        display_sensor_data();
        printf("更新传感器数据\n");
        osDelay(LONGDELAYTICK);  // 每1秒更新一次数据
    }
}

// INA219任务创建
void sensor_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "SensorTask";           // 任务的名字
    taskOptions.attr_bits = 0;                 // 属性位
    taskOptions.cb_mem = NULL;                 // 堆空间地址
    taskOptions.cb_size = 1024;                   // 堆空间大小
    taskOptions.stack_mem = NULL;              // 栈空间地址
    taskOptions.stack_size = 4096;             // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal2;   // 任务的优先级

    Sensor_Task_ID = osThreadNew((osThreadFunc_t)sensor_task, NULL, &taskOptions); // 创建INA219任务
    if (Sensor_Task_ID != NULL)
    {
        printf("ID = %d, Create Sensor_Task_ID is OK!\n", Sensor_Task_ID);
    }
}
/**********************************************************************************************************************/

/**************************************************串口控制任务******************************************************** */
//根据串口命令控制步进电机、直流电机、舵机
osThreadId_t Uart_Task_ID;
void uart_task(void)
{
    char buffer[128];
    int len;
    
    while (1)
    {
        len = uart0_read_data(buffer, sizeof(buffer));  // 接收串口数据
        if (len > 0) {
            buffer[len] = '\0';
            handle_uart_command(buffer);  // 处理串口指令
        }
        osDelay(100);
    }
}

void handle_uart_command(char *command)
{
    if (strcmp(command, "OPEN CURTAIN") == 0) {
        printf("窗帘打开\n");
        // curtain_open();
    } else if (strcmp(command, "CLOSE CURTAIN") == 0) {
        printf("窗帘关闭\n");
        // curtain_open();
    } else if (strcmp(command, "TURN ON DC MOTOR LOW") == 0) {
        printf("低档位电机启动\n");
        // dc_motor_run(DC_MOTOR_LOW);
    } else if (strcmp(command, "TURN OFF DC MOTOR MEDIUM") == 0) {
        printf("中档位电机启动\n");
        // dc_motor_run(DC_MOTOR_MEDIUM);
    } else if (strcmp(command, "TURN OFF DC MOTOR HIGH") == 0) {
        printf("高档位电机启动\n");
        // dc_motor_run(DC_MOTOR_HIGH);
    } else if (strcmp(command, "OPEN DOOR") == 0) {
        printf("门打开\n");
        // door_open();
    } else if (strcmp(command, "CLOSE DOOR") == 0) {
        printf("门关闭\n");
        // door_close();
    } else if (strcmp(command, "TURN ON LIGHT") == 0) {
        printf("灯打开\n");
        // led_on();
    } else if (strcmp(command, "TURN OFF LIGHT") == 0) {
        printf("灯关闭\n");
        // led_off();
    } else {
        printf("未知指令: %s\n", command);
    }
}

void uart_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "UartTask";           // 任务的名字
    taskOptions.attr_bits = 0;                 // 属性位
    taskOptions.cb_mem = NULL;                 // 堆空间地址
    taskOptions.cb_size = 0;                   // 堆空间大小
    taskOptions.stack_mem = NULL;              // 栈空间地址
    taskOptions.stack_size = 1024;             // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal1;   // 任务的优先级

    Uart_Task_ID = osThreadNew((osThreadFunc_t)uart_task, NULL, &taskOptions); // 创建INA219任务
    if (Uart_Task_ID != NULL)
    {
        printf("ID = %d, Create Uart_Task_ID is OK!\n", Uart_Task_ID);
    }
}

/**********************************************************************************************************************/

/**************************************************iotda任务******************************************************** */

//WIFI连接热点和密码
#define WIFI_SSID "duki"
#define WIFI_PAWD "12345678"

//注册华为云后需要更新的信息
// 产品ID
#define DEVICE_ID "678c85feef99673c8ae15f52"
// MQTT客户端ID
#define MQTT_CLIENT_ID "678c85feef99673c8ae15f52_hi3861_led_0_0_2025021015" 
// MQTT用户名
#define MQTT_USER_NAME "678c85feef99673c8ae15f52_hi3861_led"
// MQTT密码
#define MQTT_PASS_WORD "86d7740c80ac4374cc29074fa91ecb6fb1dbd87742faad2e5f04994f3d9f9f2b"
// 华为云平台的IP地址
#define SERVER_IP_ADDR "117.78.5.125"
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
typedef struct message_sensorData {
    uint8_t led;        // LED灯当前的状态
    float temperature;  // 当前的温度值
    float humidity;   // 当前的湿度值
} msg_sensorData_t;
msg_sensorData_t sensorData = {0}; // 传感器的数据
osThreadId_t mqtt_send_task_id; // mqtt 发布数据任务ID
osThreadId_t mqtt_recv_task_id; // mqtt 接收数据任务ID
#define MQTT_SEND_TASK_TIME 3 // s
#define MQTT_RECV_TASK_TIME 1 // s
#define TASK_INIT_TIME 2 // s
#define DISPLAY_BUFF_MAX 64
#define MQTT_DATA_MAX 256
uint8_t publish_topic[MQTT_DATA_MAX] = {0};
uint8_t mqtt_data[MQTT_DATA_MAX] = {0};


/**
 * @brief 组JSON数据
 */
int Packaged_json_data(void)
{
    cJSON *root = NULL, *array = NULL, *services = NULL;
    cJSON *properties = NULL;
    int ret = 0;

    // 组JSON数据
    root = cJSON_CreateObject(); // 创建一个对象
    services = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "services", services);
    array = cJSON_CreateObject();
    cJSON_AddStringToObject(array, "service_id", "attribute");
    properties = cJSON_CreateObject();
    cJSON_AddItemToObject(array, "properties", properties);
    cJSON_AddStringToObject(properties, "led", sensorData.led ? "ON" : "OFF");
    cJSON_AddNumberToObject(properties, "temperature", sensorData.temperature);
    cJSON_AddNumberToObject(properties, "humidity", sensorData.humidity);
    cJSON_AddItemToArray(services, array);  // 将对象添加到数组中

    /* 格式化打印创建的带数组的JSON对象 */
    char *str_print = cJSON_PrintUnformatted(root);
    if (str_print != NULL) {
        // printf("%s\n", str_print);
        if (strcpy_s(mqtt_data, strlen(str_print) + 1, str_print) == 0) {
            ret = 0;
        } else {
            ret = -1;
        }
        cJSON_free(str_print);
    } else {
        ret = -1;
    }
    if (root != NULL) {
        cJSON_Delete(root);
    } else {
        ret = -1;
    }
    properties = str_print = root = array = services = NULL;

    return ret;
}

int get_jsonData_value(const cJSON *const object, uint8_t *value)
{
    cJSON *json_value = NULL;
    int ret = -1;
    json_value = cJSON_GetObjectItem(object, "value");
    if (json_value) {
        if (!strcmp(json_value->valuestring, "ON")) {
            *value = 1;
            json_value = NULL;
            ret = 0; // 0为成功
        } else if (!strcmp(json_value->valuestring, "OFF")) {
            *value = 0;
            json_value = NULL;
            ret = 0;
        }
    }
    json_value = NULL;
    return ret; // -1为失败
}

/**
 * @brief 解析JSON数据
 */
int Parsing_json_data(const char *payload)
{
    cJSON *root = NULL, *command_name = NULL, *paras = NULL, *value = NULL;
    cJSON *red = NULL, *green = NULL, *blue = NULL;
    int ret_code = 1;
    root = cJSON_Parse((const char *)payload);
    if (root) {
        // 解析JSON数据
        command_name = cJSON_GetObjectItem(root, "command_name");
        paras = cJSON_GetObjectItem(root, "paras");
        if (command_name)
        {
            if (!strcmp(command_name->valuestring, "led")) 
            {
                ret_code = get_jsonData_value(paras, &sensorData.led);
            } 
        }
    }
    cJSON_Delete(root);
    root = command_name = paras = value = red = green = blue = NULL;
    (sensorData.led == 1) ? LED(1) : LED(0);

    return ret_code;
}


/**
 * @brief MQTT  发布消息任务
 */
void mqtt_send_task(void)
{
    while (1) 
    {
        // 获取传感器的数据
        dht11_read_data(&temp, &humi);
        sensorData.temperature = temp;
        sensorData.humidity = humi;
        
        // 组Topic
        memset_s(publish_topic, MQTT_DATA_MAX, 0, MQTT_DATA_MAX);
        if (sprintf_s(publish_topic, MQTT_DATA_MAX, MQTT_TOPIC_PUB_PROPERTIES, DEVICE_ID) > 0) 
        {
            // 组JSON数据
            Packaged_json_data();
            // 发布消息
            MQTTClient_pub(publish_topic, mqtt_data, strlen((char *)mqtt_data));
        }
        osDelay(LONGDELAYTICK);
    }
    
}

// 向云端发送返回值
void send_cloud_request_code(const char *request_id, int ret_code, int request_len)
{
    char *request_topic = (char *)malloc(strlen(MALLOC_MQTT_TOPIC_PUB_COMMANDS_REQ) +
                                            strlen(DEVICE_ID) + request_len + 1);
    if (request_topic != NULL) 
    {
        memset_s(request_topic,
                 strlen(DEVICE_ID) + strlen(MALLOC_MQTT_TOPIC_PUB_COMMANDS_REQ) + request_len + 1,
                 0,
                 strlen(DEVICE_ID) + strlen(MALLOC_MQTT_TOPIC_PUB_COMMANDS_REQ) + request_len + 1);
        if (sprintf_s(request_topic,
                      strlen(DEVICE_ID) + strlen(MALLOC_MQTT_TOPIC_PUB_COMMANDS_REQ) + request_len + 1,
                      MQTT_TOPIC_PUB_COMMANDS_REQ, DEVICE_ID, request_id) > 0) 
        {
            if (ret_code == 0) 
            {
                MQTTClient_pub(request_topic, "{\"result_code\":0}", strlen("{\"result_code\":0}"));
            } 
            else if (ret_code == 1) 
            {
                MQTTClient_pub(request_topic, "{\"result_code\":1}", strlen("{\"result_code\":1}"));
            }
        }
        free(request_topic);
        request_topic = NULL;
    }
}

/**
 * @brief MQTT接收数据的回调函数
 */
int8_t mqttClient_sub_callback(unsigned char *topic, unsigned char *payload)
{
    if ((topic == NULL) || (payload == NULL)) 
    {
        return -1;
    } 
    else 
    {
        printf("topic: %s\r\n", topic);
        printf("payload: %s\r\n", payload);

        // 提取出topic中的request_id
        char request_id[50] = {0};
        int ret_code = 1; // 1为失败
        if (0 == strcpy_s(request_id, sizeof(request_id),
                          topic + strlen(DEVICE_ID) + strlen("$oc/devices//sys/commands/request_id="))) {
            printf("request_id: %s\r\n", request_id);
            // 解析JSON数据
            ret_code = Parsing_json_data(payload);
            send_cloud_request_code(request_id, ret_code, sizeof(request_id));
        }
    }
    return 0;
}

/**
 * @brief MQTT  接收消息任务
 */
void mqtt_recv_task(void)
{
    while (1) 
    {
        MQTTClient_sub();
        osDelay(LONGDELAYTICK);
    }
}

//硬件初始化
void hardware_init(void)
{
    // led_init();//LED初始化
    // ds18b20_init();//DS18B20初始化

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
    sleep(TASK_INIT_TIME);
}

//任务创建
void wifi_iotda_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "mqtt_send_task";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = TASK_STACK_SIZE;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal1; // 任务的优先级

    mqtt_send_task_id = osThreadNew((osThreadFunc_t)mqtt_send_task, NULL, &taskOptions); // 创建任务
    if (mqtt_send_task_id != NULL)
    {
        printf("ID = %d, mqtt_send_task_id Create OK!\n", mqtt_send_task_id);
    }

    taskOptions.name = "mqtt_recv_task";
    taskOptions.stack_size = TASK_STACK_SIZE;
    mqtt_recv_task_id = osThreadNew((osThreadFunc_t)mqtt_recv_task, NULL, &taskOptions);
    if (mqtt_recv_task_id != NULL) {
        printf("ID = %d, Create mqtt_recv_task_id is OK!\r\n", mqtt_recv_task_id);
    }
}
/**********************************************************************************************************************/

/**
 * @description: 初始化并创建任务
 * @param {*}
 * @return {*}
 */
static bsp_init(void)
{
    //LED初始化
    led_init();

    //蜂鸣器初始化
    beep_init();

    //oled初始化
    oled_init_mutex();
    oled_init();
    // oled_clear();
	// oled_showstring(0,0,"Hello i am Dukiya",12);
	// oled_refresh_gram();//更新显存

    //温湿度传感器初始化
    while(dht11_init())
	{
		printf("DHT11检测失败,请插好!\r\n");
		osDelay(SHORTDELAYTICK); //100ms
	}
	printf("DHT11检测成功!\r\n");

    //电流电压传感器初始化 也需要做初始化检测处理
    ina219_init();

    //步进电机初始化
    step_motor_init();

    //直流电机初始化
    dc_motor_init();

    //舵机初始化
    sg90_init();

    //人体感应初始化
    sr501_init();

    //烟雾传感器初始化
    smoke_init();

    //串口接收初始化
    uart0_init(115200);

    //iotda初始化
    hardware_init();
}
static void template_demo(void)
{
    printf("极个别组-基于openharmony的智能家居系统\r\n");

    bsp_init();
    // uart0_init(115200);

    // test_task_create();

    // led_init();
    // sr501_init();
    // motion_sensor_task_create();//貌似要等一分钟才会正常
    sensor_task_create();
    // smoke_sensor_task_create();
    // uart_task_create();
    
    wifi_iotda_task_create();//任务创建

}
SYS_RUN(template_demo);
