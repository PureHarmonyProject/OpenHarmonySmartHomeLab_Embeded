#include "dht11_oled_task.h"
#include "template.h"

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
    // osMutexAcquire(oled_mutex, osWaitForever);  
    // oled_clear();
    oled_showstring(0, 0, display_buffer, 12);  // 显示数据
    oled_refresh_gram();
    // osMutexRelease(oled_mutex);  // 释放OLED互斥锁
}

void sensor_task(void)
{
        //oled初始化
    // printf("OLED is initing !!!\r\n");
    // oled_init_mutex();
    // oled_init();
    // printf("OLED init success !!!\r\n");


    // // // 温湿度传感器初始化
    // printf("DHT11 is initing !!!\r\n");
    // while(dht11_init())
	// {
	// 	printf("DHT11检测失败,请插好!\r\n");
	// 	osDelay(TASK_DELAY_100MS); //100ms
	// }
	// printf("DHT11检测成功!\r\n");
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
    taskOptions.priority = osPriorityAboveNormal;   // 任务的优先级

    Sensor_Task_ID = osThreadNew((osThreadFunc_t)sensor_task, NULL, &taskOptions); // 创建INA219任务
    if (Sensor_Task_ID != NULL)
    {
        printf("ID = %d, Create Sensor_Task_ID is OK!\n", Sensor_Task_ID);
    }
}