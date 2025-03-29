#include "mqtt_task.h"
#include "template.h"

typedef struct message_sensorData {
    uint32_t led_lightness_color;
    uint8_t curtain_percent;
    hi_bool curtain_openstate;
    hi_bool door_state;
    uint8_t temperature_indoor;
    uint8_t humidity_indoor;  
    uint32_t smoke;           // 烟雾传感器数据
    uint32_t comb;           // 可燃气体数据
    uint32_t light;           // 可燃气体数据
    hi_bool beep_state; // 蜂鸣器当前的状态
    uint8_t airConditioner_state; // 空调当前的状态
    float current; // 当前电流 ma
    float voltage; // 当前电压 mv
    float power; // 当前功率 mw
} msg_sensorData_t;

msg_sensorData_t sensorData = {0}; // 传感器的数据
osThreadId_t mqtt_send_task_id; // mqtt 发布数据任务ID
osThreadId_t mqtt_recv_task_id; // mqtt 接收数据任务ID

uint8_t publish_topic[MQTT_DATA_MAX] = {0};
uint8_t mqtt_data[MQTT_DATA_MAX] = {0};

int Packaged_json_data(void);
int Parsing_json_data(const char *payload);
void send_cloud_request_code(const char *request_id, int ret_code, int request_len);

extern uint8_t temp, humi;

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
    // 确保所有数据正确填充
    cJSON_AddNumberToObject(properties, "led_lightness_color", sensorData.led_lightness_color);
    cJSON_AddNumberToObject(properties, "curtain_percent", sensorData.curtain_percent);
    // cJSON_AddNumberToObject(properties, "curtain_openstate", sensorData.curtain_openstate);
    cJSON_AddNumberToObject(properties, "door_state", sensorData.door_state);
    cJSON_AddNumberToObject(properties, "temperature_indoor", sensorData.temperature_indoor);
    cJSON_AddNumberToObject(properties, "humidity_indoor", sensorData.humidity_indoor);
    cJSON_AddNumberToObject(properties, "smoke", sensorData.smoke);
    cJSON_AddNumberToObject(properties, "comb", sensorData.comb);
    cJSON_AddNumberToObject(properties, "light", sensorData.light);
    cJSON_AddNumberToObject(properties, "beep_state", sensorData.beep_state);
    cJSON_AddNumberToObject(properties, "airConditioner_state", sensorData.airConditioner_state);  // ✅ 修正
    cJSON_AddNumberToObject(properties, "current", sensorData.current);
    cJSON_AddNumberToObject(properties, "voltage", sensorData.voltage);
    cJSON_AddNumberToObject(properties, "power", sensorData.power);
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

// 解析 LED 颜色值
int led_setColor_get_jsonData_value(const cJSON *const object, uint32_t *value) {
    if (object == NULL || value == NULL) {
        printf("[ERROR] led_setColor_get_jsonData_value: 参数为空\n");
        return -1;
    }

    cJSON *json_value = cJSON_GetObjectItem(object, "color_RGB");//命令有误，实际该数据包含了亮度和rgb
    if (!json_value) {
        printf("[ERROR] led_setColor_get_jsonData_value: color_RGB 未找到\n");
        return -1;
    }

    if (!cJSON_IsNumber(json_value)) {
        printf("[ERROR] led_setColor_get_jsonData_value: color_RGB 类型错误\n");
        return -1;
    }

    *value = (uint32_t)json_value->valuedouble;  // 使用 `valuedouble` 防止 `int32_t` 溢出

    printf("✅ 提取的 color_RGB 值: %u (HEX: %08X)\n", *value, *value);
    return 0;
}

// 解析门状态值
int door_setState_get_jsonData_value(const cJSON *const object, hi_bool *value) {
    if (object == NULL || value == NULL) {
        printf("[ERROR] door_setState_get_jsonData_value: 参数为空\n");
        return -1;
    }

    cJSON *json_value = cJSON_GetObjectItem(object, "door_state");
    if (!json_value) {
        printf("[ERROR] door_setState_get_jsonData_value: door_state 未找到\n");
        return -1;
    }

    printf("🔍 JSON解析的door_state: %s\n", cJSON_Print(json_value));

    if (cJSON_IsBool(json_value)) {
        *value = cJSON_IsTrue(json_value) ? 1 : 0;
        printf("✅ door_state 解析成功: %d\n", *value);
        return 0;
    }

    printf("[ERROR] door_setState_get_jsonData_value: 类型错误 (期望: 布尔值)\n");
    return -1;
}

// 解析窗帘开合百分比
int curtain_openPercent_get_jsonData_value(const cJSON *const object, uint8_t *value) {
    if (object == NULL || value == NULL) {
        printf("[ERROR] curtain_openPercent_get_jsonData_value: 参数为空\n");
        return -1;
    }

    cJSON *json_value = cJSON_GetObjectItem(object, "open_percent");
    if (!json_value) {
        printf("[ERROR] curtain_openPercent_get_jsonData_value: open_percent 未找到\n");
        return -1;
    }

    if (!cJSON_IsNumber(json_value)) {
        printf("[ERROR] curtain_openPercent_get_jsonData_value: open_percent 类型错误\n");
        return -1;
    }

    *value = (uint8_t)json_value->valueint;
    printf("✅ 提取的 open_percent 值: %d\n", *value);
    return 0;
}

int curtain_closePercent_get_jsonData_value(const cJSON *const object, uint8_t *value) {
    if (object == NULL || value == NULL) {
        printf("[ERROR] curtain_closePercent_get_jsonData_value: 参数为空\n");
        return -1;
    }

    cJSON *json_value = cJSON_GetObjectItem(object, "close_percent");
    if (!json_value) {
        printf("[ERROR] curtain_closePercent_get_jsonData_value: close_percent 未找到\n");
        return -1;
    }

    if (!cJSON_IsNumber(json_value)) {
        printf("[ERROR] curtain_closePercent_get_jsonData_value: close_percent 类型错误\n");
        return -1;
    }

    *value = (uint8_t)json_value->valueint;
    printf("✅ 提取的 close_percent 值: %d\n", *value);
    return 0;
}

// 解析空调状态
int airConditioner_setState_get_jsonData_value(const cJSON *const object, uint8_t *value) {
    if (object == NULL || value == NULL) {
        printf("[ERROR] airConditioner_setState_get_jsonData_value: 参数为空\n");
        return -1;
    }

    cJSON *json_value = cJSON_GetObjectItem(object, "airConditioner_state");
    if (!json_value) {
        printf("[ERROR] airConditioner_setState_get_jsonData_value: airConditioner_state 未找到\n");
        return -1;
    }

    if (!cJSON_IsNumber(json_value)) {
        printf("[ERROR] airConditioner_setState_get_jsonData_value: airConditioner_state 类型错误\n");
        return -1;
    }

    *value = (uint8_t)json_value->valueint;
    printf("✅ 提取的 airConditioner_state 值: %d\n", *value);
    return 0;
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
            // if (!strcmp(command_name->valuestring, "led")) 
            // {
            //     ret_code = get_jsonData_value(paras, &sensorData.led);
            // } 
            // if (!strcmp(command_name->valuestring, "door")) 
            // {
            //     ret_code = get_jsonData_value(paras, &sensorData.door);
            // }
            // //窗帘的需求是什么
            // if (!strcmp(command_name->valuestring, "curtain")) 
            // {
            //     ret_code = curtain_get_jsonData_value(paras, &sensorData.curtain_curState);
            // }

            /////////////////////////////////////////////////////////////////////////////////
            // if (!strcmp(command_name->valuestring, "led_setLightness")) 
            // {
            //     ret_code = get_jsonData_value(paras, &sensorData.led);
            // } 
            //目前还没改过来,需要将亮度和color合起来
            if (!strcmp(command_name->valuestring, "led_setLightness_color")) 
            {
                ret_code = led_setColor_get_jsonData_value(paras, &sensorData.led_lightness_color);
                led_set_color_and_brightness(sensorData.led_lightness_color);
            }
            if (!strcmp(command_name->valuestring, "curtain_openPercent")) 
            {
                ret_code = curtain_openPercent_get_jsonData_value(paras, &sensorData.curtain_percent);
                curtain_open_angle(sensorData.curtain_percent); //待修改 应该是关闭到指定的角度，而不是关闭角度
            }
            if (!strcmp(command_name->valuestring, "curtain_closePercent")) 
            {
                ret_code = curtain_closePercent_get_jsonData_value(paras, &sensorData.curtain_percent);
                curtain_close_angle(sensorData.curtain_percent);
            } 
            if (!strcmp(command_name->valuestring, "door_setState")) 
            {
                ret_code = door_setState_get_jsonData_value(paras, &sensorData.door_state);
                (sensorData.door_state == 1) ? door_open() : door_close();
            }
            if (!strcmp(command_name->valuestring, "airConditioner_setState")) 
            {
                ret_code = airConditioner_setState_get_jsonData_value(paras, &sensorData.airConditioner_state);
                airConditioner_work(sensorData.airConditioner_state);
            }
            // if (!strcmp(command_name->valuestring, "led_getState")) 
            // {
            //     ret_code = curtain_get_jsonData_value(paras, &sensorData.curtain_curState);
            // }
            // if (!strcmp(command_name->valuestring, "curtain_getState")) 
            // {
            //     ret_code = get_jsonData_value(paras, &sensorData.led);
            // } 
            // if (!strcmp(command_name->valuestring, "door_getState")) 
            // {
            //     ret_code = get_jsonData_value(paras, &sensorData.door);
            // }
            // if (!strcmp(command_name->valuestring, "dth11_getState")) 
            // {
            //     ret_code = curtain_get_jsonData_value(paras, &sensorData.curtain_curState);
            // }
            // if (!strcmp(command_name->valuestring, "smoke_getState")) 
            // {
            //     ret_code = curtain_get_jsonData_value(paras, &sensorData.curtain_curState);
            // }
            // if (!strcmp(command_name->valuestring, "beep_getState")) 
            // {
            //     ret_code = curtain_get_jsonData_value(paras, &sensorData.curtain_curState);
            // }
        }
    }
    cJSON_Delete(root);
    root = command_name = paras = value = red = green = blue = NULL;
    // (sensorData.led == 1) ? LED(1) : LED(0);
    
    
    // curtain_open_angle_by_pcf8575(360 * sensorData.curtain_percent / 100);
    return ret_code;
}


/**
 * @brief MQTT  发布消息任务
 */
void mqtt_send_task(void)
{
    // printf("IOTDA is initing !!!\r\n");
    // p_MQTTClient_sub_callback = &mqttClient_sub_callback;

    // // 连接WiFi
    // if (WiFi_connectHotspots(WIFI_SSID, WIFI_PAWD) != WIFI_SUCCESS) {
    //     printf("[error] connectWiFiHotspots\r\n");
    // }
    // sleep(TASK_INIT_TIME);

    // // 连接MQTT服务器
    // if (MQTTClient_connectServer(SERVER_IP_ADDR, SERVER_IP_PORT) != WIFI_SUCCESS) {
    //     printf("[error] mqttClient_connectServer\r\n");
    // }
    // sleep(TASK_INIT_TIME);

    // // 初始化MQTT客户端
    // if (MQTTClient_init(MQTT_CLIENT_ID, MQTT_USER_NAME, MQTT_PASS_WORD) != WIFI_SUCCESS) {
    //     printf("[error] mqttClient_init\r\n");
    // }
    // sleep(TASK_INIT_TIME); 

    // // 订阅主题
    // if (MQTTClient_subscribe(MQTT_TOPIC_SUB_COMMANDS) != WIFI_SUCCESS) {
    //     printf("[error] mqttClient_subscribe\r\n");
    // }
    // printf("IOTDA init success !!!\r\n");
    // sleep(TASK_INIT_TIME);
    while (1) 
    {
        // 获取传感器的数据
        dht11_read_data(&temp, &humi);
        sensorData.temperature_indoor = temp;
        sensorData.humidity_indoor = humi;
        sensorData.curtain_percent = curtain_get_curangle();
        sensorData.door_state = door_get_curstate();
        sensorData.airConditioner_state = airConditioner_get_curstate();
        sensorData.smoke = smoke_get_value();
        sensorData.comb = MQ5_get_value();
        sensorData.light = light_get_value();
        sensorData.beep_state = beep_get_state();
        sensorData.current = ina226_get_current();
        sensorData.voltage = ina226_get_bus_voltage();
        sensorData.power = sensorData.current * sensorData.voltage / 1000;
        // 同时也给asrpro发送心跳

        char buf[64];
        sprintf(buf, "C1%d,%d,%d,%d,%d\n", sensorData.temperature_indoor, sensorData.humidity_indoor, sensorData.door_state, sensorData.smoke, sensorData.comb);
        uart1_send_data(buf, sizeof(buf));  // 发送数据

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
    char request_topic[128];
    snprintf(request_topic, sizeof(request_topic), MQTT_TOPIC_PUB_COMMANDS_REQ, DEVICE_ID, request_id);

    char response[64];
    snprintf(response, sizeof(response), "{\"result_code\":%d}", ret_code);

    printf("\n📤 发送 ACK 到 IoTDA\n");
    printf("📤 ACK 主题 (request_topic): %s\n", request_topic);
    printf("📤 ACK 数据 (response): %s\n", response);

    int result = MQTTClient_pub(request_topic, response, strlen(response));

    // **新增详细错误代码检查**
    if (result == 0) {
        printf("✅ ACK 发送成功\n");
    } else {
        printf("❌ ACK 发送失败，错误代码: %d\n", result);
        if (result == -1) {
            printf("   📌 可能的原因: MQTTClient_pub() 内部错误\n");
        } else if (result == -2) {
            printf("   📌 可能的原因: MQTT 连接未建立 (但你说不是这个问题)\n");
        } else if (result == -3) {
            printf("   📌 可能的原因: 发送数据过大\n");
        } else if (result == -4) {
            printf("   📌 可能的原因: MQTT 主题格式错误\n");
        } else {
            printf("   📌 可能的原因: 未知 MQTT 发送失败\n");
        }
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

        // 提取 request_id
        char request_id[50] = {0};
        char *request_start = strstr(topic, "request_id="); // 找到 request_id= 位置
        if (request_start) {
            strcpy_s(request_id, sizeof(request_id), request_start + 11);  // 复制 ID 部分
        }

        printf("✅ 正确提取的 request_id: %s\r\n", request_id);

        // 解析 JSON 数据
        int ret_code = Parsing_json_data(payload);

        // 发送 ACK 到 IoTDA
        send_cloud_request_code(request_id, ret_code, strlen(request_id));
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