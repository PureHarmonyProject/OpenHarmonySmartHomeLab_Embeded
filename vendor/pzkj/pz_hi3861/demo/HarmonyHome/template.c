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
#include "bsp_ina226.h"
#include "bsp_uart.h"
// #include "bsp_ds18b20.h"
#include "bsp_pcf8575.h"
#include "bsp_nfc.h"
#include "bsp_rc522.h"

#include "bsp_wifi.h"
#include "bsp_mqtt.h"

#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "lwip/api_shell.h"

#include "cJSON.h"
#define TASKDLYTICK    500
#define TASK_DELAY_5000MS 500    //500 = 5000ms 1tick = 10ms
#define TASK_DELAY_1000MS  100    //100 = 1000ms 1tick = 10ms
#define TASK_DELAY_100MS 10    //10 = 100ms 1tick = 10ms

osThreadId_t Test_Task_ID; //任务ID
#define VALID_UID  {0x12, 0x34, 0x56, 0x78, 0x9A}  // 允许访问的 UID
void rc522_test_spi(void) {
    printf("[DEBUG] SPI 读写测试...\n");

    // **写入 0x55 到 RC522 寄存器 0x0C (CommandReg)**
    rc522_write_register(0x0C, 0x12);

    // **读取 0x0C 看看是否正确**
    hi_u8 test_value = rc522_read_register(0x0C);

    printf("[DEBUG] 读取 CommandReg (0x0C): 0x%02X\n", test_value);
}

void test_task(void) {
    // pcf8575_init();
    // step_motor_init();
    // ina219_init();

    if(rc522_init() != HI_ERR_SUCCESS)  // 初始化 RC522
    {
        printf("RC522 初始化失败\n");
        return;
    }
    printf("RC522 初始化完成\n");

    hi_u8 addr = 0x07;
    hi_u8 write_data = 0x12;
    hi_u8 read_data[1] = {0};

    // printf("\n=== 写入寄存器 0x%02X ===\n", addr);
    // rc522_write_read_register(addr, write_data, NULL, 0, 0);
    printf("\n=== 读取寄存器 0x%02X ===\n", addr);
    rc522_write_read_register(addr, 0x00, read_data, 1, 1);

    printf("\n最终读取到的值: 0x%02X\n", read_data[0]);

    // if(rc522_write_register(0x0c,0x12) != HI_ERR_SUCCESS)  // 初始化 RC522
    // {
    //     printf("RC522 写值失败\n");
    //     return;
    // }
    // printf("RC522 写值成功\n");
    // rc522_reset();
    // hi_u8 test_reg = rc522_read_register(0x0c);
    // printf("[DEBUG] 读取 test_reg (0x0c): 0x%02X\n",test_reg);
    // rc522_test_spi();
    // hi_u8 status2 = rc522_read_register(0x07);
    // printf("[DEBUG] 读取 Status1Reg (0x07): 0x%02X\n", status2);

    // // // 读取 RC522 版本号，检查 SPI 是否正常
    // hi_u8 version = rc522_read_register(0x37);
    // printf("RC522 版本号: 0x%02X\n", version);
    // if (version != 0x91 && version != 0x92) {
    //     printf("⚠️  RC522 版本号错误，检查 SPI 连接！\n");
    //     return;
    // }

    // // 确保 RC522 天线打开
    // rc522_write_register(0x14, 0x03);  // 打开天线
    // hi_u8 tx_control = rc522_read_register(0x14);
    // printf("RC522 天线状态: 0x%02X\n", tx_control);
    // if (tx_control != 0x03) {
    //     printf("⚠️  天线未正确打开，请检查 `rc522_antenna_on()`\n");
    // }

    // hi_u8 uid[5];
    // hi_u8 valid_uid[5] = VALID_UID;
    // 初始化NFC模块
    // result = nfc_init();

    while (1) {

        printf("\n=== 读取寄存器 0x%02X ===\n", addr);
        rc522_write_read_register(addr, 0x00, read_data, 1, 1);

        printf("\n最终读取到的值: 0x%02X\n", read_data[0]);
        // hi_u8 test_reg = rc522_read_register(0x0c);
        // printf("[DEBUG] 读取 test_reg (0x0c): 0x%02X\n",test_reg);
        // // rc522_test_spi();
        // hi_u8 status2 = rc522_read_register(0x07);
        // printf("[DEBUG] 读取 Status1Reg (0x07): 0x%02X\n", status2);
    
        // // // 读取 RC522 版本号，检查 SPI 是否正常
        // hi_u8 version = rc522_read_register(0x37);
        // printf("RC522 版本号: 0x%02X\n", version);
        // hi_u8 status1 = rc522_read_register(0x07);
        // printf("[DEBUG] 读取 Status1Reg (0x07): 0x%02X\n", status1);
        // printf("\n=== NFC 读卡检测中... ===\n");

        // // **先检测 NFC 卡是否存在**
        // hi_u8 status = rc522_check_card();
        // if (!status) {
        //     printf("❌ 未检测到 NFC 卡\n");
        // } else {
        //     printf("✅ NFC 卡检测成功！开始读取 UID...\n");

        //     // **读取 UID**
        //     if (rc522_read_card_uid(uid)) {
        //         printf("📟 NFC UID: %02X %02X %02X %02X %02X\n",
        //                uid[0], uid[1], uid[2], uid[3], uid[4]);

        //         // **检查 UID 是否匹配**
        //         if (memcmp(uid, valid_uid, 5) == 0) {
        //             printf("🔓 访问授权成功！\n");
        //             // 这里可以添加开门/开灯的 GPIO 逻辑
        //         } else {
        //             printf("❌ 访问被拒绝！\n");
        //         }
        //     } else {
        //         printf("⚠️ UID 读取失败！\n");
        //     }
        // }

        // hi_sleep(1000);
        // ina226_get_current();
        // led_on();
        // printf("窗帘打开\n");
        // curtain_open_angle(150);
        
        // printf("门操作\n");
        // if(door_get_curstate()) 
        // {
        //     door_close();
        // }else {
        //     door_open();
        // }
        
        // usleep(500 * 1000);
        // printf("门关闭\n");
        // door_close();
        // usleep(500 * 1000);
        // printf("蜂鸣器响\n");
        // beep_warning_by_pcf8575();
        // usleep(500 * 1000);
        
        
        // printf("cur i = %.2f ma\n",ina226_get_current());
        // printf("cur u = %.2f mv\n",ina219_get_bus_voltage());
        // printf("cur p = %.2f mw\n",ina219_get_power());

        // 读取NFC卡片数据
        // uint8_t readBuffer[100]; // 假设读取的最大数据长度为100字节
        // result = get_NDEFDataPackage(readBuffer, sizeof(readBuffer));
        // printf("读取到的NFC卡片数据：");
        // for (int i = 0; i < 9; i++) {
        //     printf("%02X ", readBuffer[i]);
        // }
        // printf("\r\n");

        // pcf8575_write_bit(1,1);
        // pcf8575_write_bit(2,1);
        // pcf8575_write_bit(3,1);

        // // pcf8575_write(0xff);
        // uint16_t data;
        // pcf8575_read(&data);
        // printf("data = %04x\r\n",data);  
        // printf("bit = %d\r\n", pcf8575_read_bit(1));
        // printf("bit = %d\r\n", pcf8575_read_bit(2));
        // printf("bit = %d\r\n", pcf8575_read_bit(3));
        // curtain_test();
        // door_open();
        // curtain_open_by_pcf8575();
        // beep_warning_by_pcf8575();

        osDelay(TASK_DELAY_100MS);  // 每 5 秒更新一次数据
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
    taskOptions.stack_size = 4096;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal2; // 任务的优先级

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
                // led_on();  // 打开LED
                // beep_warning_by_pcf8575();
            }else {
                printf("[MOTION] 人已离开\n");
                // led_off();
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
        osDelay(TASK_DELAY_5000MS);  // 每5秒更新一次数据
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
void handle_uart_command(char *command)
{
    /*重新规定指令*/
    /*
    0x01 : 开门
    0x02: 关门
    0x03: 开窗帘
    0x04: 关窗帘
    0x11: 门已被打开，不可重复开
    0x12: 门已被关闭，不可重复关
    0x21: 门已开好
    0x22: 门已关好
    0x23: 窗帘已开好
    0x24: 窗帘已关好
    */
   uint16_t response = 0;  // 定义 16 位返回值
   printf("处理指令中\n");

   // 去除换行符，防止匹配失败
   char *trimmed_command = strtok(command, "\r\n");

   // 解析 16 进制指令（字符串 -> uint16_t）
   uint16_t cmd = (uint16_t)strtol(trimmed_command, NULL, 16);

   switch (cmd) {
       case 0x03:  // 开窗帘
           curtain_open_by_pcf8575();
           response = 0x23;
           break;
       case 0x04:  // 关窗帘
           printf("窗帘关闭\n");
           response = 0x24;
           break;
       case 0x01:  // 开门
           response = door_open() ? 0x21 : 0x11;
           break;
       case 0x02:  // 关门
           response = door_close() ? 0x22 : 0x12;
           break;
       default:
           printf("未知指令: %s\n", trimmed_command);
           return;
   }

   // 发送 `uint16_t` 数据（发送 2 个字节）
   uart1_send_data(&response, sizeof(response));
}

osThreadId_t Uart_Task_ID;
void uart_task(void)
{
    char buffer[128];
    int len;
    uart1_init(115200);
    while (1)
    {
        len = uart1_read_data(buffer, sizeof(buffer));  // 接收串口数据
        if (len > 0) {
            buffer[len] = '\0';
            handle_uart_command(buffer);  // 处理串口指令
        }

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
    taskOptions.stack_size = 4096;             // 栈空间大小 单位:字节
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
    double curtain_curState; // 窗帘的开合百分比
    uint8_t door; //门的开闭状态
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
    cJSON_AddNumberToObject(properties, "curtain_curState", sensorData.curtain_curState);
    cJSON_AddStringToObject(properties, "door", sensorData.door ? "ON" : "OFF");
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

//处理on,off类命令
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

//处理窗帘开合比命令
int curtain_get_jsonData_value(const cJSON *const object, double *value)
{
    cJSON *json_value = NULL;
    int ret = -1;
    json_value = cJSON_GetObjectItem(object, "value");
    if (json_value) {
        *value = json_value->valuedouble;
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
            if (!strcmp(command_name->valuestring, "door")) 
            {
                ret_code = get_jsonData_value(paras, &sensorData.door);
            }
            //窗帘的需求是什么
            if (!strcmp(command_name->valuestring, "curtain")) 
            {
                ret_code = curtain_get_jsonData_value(paras, &sensorData.curtain_curState);
            }
        }
    }
    cJSON_Delete(root);
    root = command_name = paras = value = red = green = blue = NULL;
    (sensorData.led == 1) ? LED(1) : LED(0);
    (sensorData.door == 1) ? door_open() : door_close();
    curtain_open_angle(360 * sensorData.curtain_curState);
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
        sensorData.curtain_curState = curtain_get_curstate();
        sensorData.door = door_get_curstate();
        // sensorData.door = door_get_curstate();
        // 组Topic
        memset_s(publish_topic, MQTT_DATA_MAX, 0, MQTT_DATA_MAX);
        if (sprintf_s(publish_topic, MQTT_DATA_MAX, MQTT_TOPIC_PUB_PROPERTIES, DEVICE_ID) > 0) 
        {
            // 组JSON数据
            Packaged_json_data();
            // 发布消息
            MQTTClient_pub(publish_topic, mqtt_data, strlen((char *)mqtt_data));
        }
        osDelay(TASK_DELAY_1000MS);
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
        osDelay(TASK_DELAY_1000MS);
    }
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
    // //LED初始化
    // led_init();

    // //蜂鸣器初始化
    // beep_init();

    //oled初始化
    printf("OLED is initing !!!\r\n");
    oled_init_mutex();
    oled_init();
    printf("OLED init success !!!\r\n");


    // 温湿度传感器初始化
    printf("DHT11 is initing !!!\r\n");
    while(dht11_init())
	{
		printf("DHT11检测失败,请插好!\r\n");
		osDelay(TASK_DELAY_100MS); //100ms
	}
	printf("DHT11检测成功!\r\n");

    // //电流电压传感器初始化 也需要做初始化检测处理
    // // ina219_init();

    // //步进电机初始化
    // // step_motor_init();

    // //直流电机初始化
    // // dc_motor_init();

    printf("PCF8575 is initing !!!\r\n");
    pcf8575_init();
    // //舵机初始化

    printf("SG90 is initing !!!\r\n");
    sg90_init();
    door_open();
    door_close();
    printf("SG90 init success !!!\r\n");
    // //人体感应初始化
    printf("SR501 is initing !!!\r\n");
    sr501_init();
    printf("SR501 init success !!!\r\n");
    // //烟雾传感器初始化
    printf("SMOKE is initing !!!\r\n");
    smoke_init();
    printf("SMOKE init success !!!\r\n");


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
static void template_demo(void)
{
    printf("极个别组-基于openharmony的智能家居系统\r\n");

    // bsp_init();
    test_task_create();

    // motion_sensor_task_create();//貌似要等一分钟才会正常
    // sensor_task_create();
    // smoke_sensor_task_create();
    // uart_task_create();
    
    // wifi_iotda_task_create();//任务创建
    
}
SYS_RUN(template_demo);
