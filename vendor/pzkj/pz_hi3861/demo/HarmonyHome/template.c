#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_oled.h"
#include "bsp_step_motor.h"
#include "bsp_sr501.h"
#include "bsp_key.h"
#include "bsp_dc_motor.h"
#include "bsp_sg90.h"
#include "bsp_smoke.h"
#include "bsp_ina219.h"
#include "bsp_uart.h"
#define TASKDLYTICK    500

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

// 传感器、电流电压更新
void display_sensor_data(void)
{
    char display_buffer[128];
    uint8_t temp, humi;
    int16_t current;
    // uint16_t bus_voltage, shunt_voltage;
    
    // 获取温湿度数据
    dht11_read_data(&temp, &humi);
    
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
        osDelay(1000);  // 每1秒更新一次数据
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
    taskOptions.priority = osPriorityNormal;   // 任务的优先级

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
		osDelay(500);; //10ms
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
}
static void template_demo(void)
{
    printf("极个别组-基于openharmony的智能家居系统\r\n");

    // bsp_init();
    // uart0_init(115200);

    // test_task_create();

    led_init();
    sr501_init();
    motion_sensor_task_create();//貌似要等一分钟才会正常
    sensor_task_create();
    smoke_sensor_task_create();
    // uart_task_create();

}
SYS_RUN(template_demo);
