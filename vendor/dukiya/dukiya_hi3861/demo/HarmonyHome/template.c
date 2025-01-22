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
#include "bsp_adc.h"
#include "bsp_ina219.h"

#define TASKDLYTICK    1000
//LED任务 io2
osThreadId_t LED_Task_ID; //led任务ID

void LED_Task(void)
{
    led_init();//LED初始化

    while (1) 
    {       
        LED(1); 
        printf("[LED PRI = %d]\n",osPriorityBelowNormal);
        osDelay(TASKDLYTICK); //500ms
    }
}
//LED任务创建
void led_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "LEDTask";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal; // 任务的优先级

    LED_Task_ID = osThreadNew((osThreadFunc_t)LED_Task, NULL, &taskOptions); // 创建led任务
    if (LED_Task_ID != NULL)
    {
        printf("ID = %d, Create LED_Task_ID is OK!\n", LED_Task_ID);
    }
}

//蜂鸣器任务 io8
osThreadId_t BEEP_Task_ID; //beep任务ID

void Beep_Task(void)
{
    beep_init();

    while (1) 
    {       
        beep_alarm(100, 250);
        printf("[BEEP PRI = %d]\n",osPriorityBelowNormal1);
        osDelay(TASKDLYTICK); //500ms
    }
    
}
//蜂鸣器任务创建
void beep_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "BeepTask";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal1; // 任务的优先级

    BEEP_Task_ID = osThreadNew((osThreadFunc_t)Beep_Task, NULL, &taskOptions); // 创建Beep任务
    if (BEEP_Task_ID != NULL)
    {
        printf("ID = %d, Create Beep_Task_ID is OK!\n", BEEP_Task_ID);
    }
}

//oled任务 scl:io9 sda:io10
osThreadId_t OLED_Task_ID; //beep任务ID

void Oled_Task(void)
{
    oled_init();
    
    oled_clear();
	oled_showstring(0,0,"Hello i am Dukiya",12);
	oled_refresh_gram();//更新显存

    while (1) 
    {       
        printf("[OLED PRI = %d]\n",osPriorityBelowNormal2);
        osDelay(TASKDLYTICK); //500ms
    }
}
//oled任务创建
void oledtask_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "OledTask";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal2; // 任务的优先级

    OLED_Task_ID = osThreadNew((osThreadFunc_t)Oled_Task, NULL, &taskOptions); // 创建Oled任务
    if (OLED_Task_ID != NULL)
    {
        printf("ID = %d, Create Oled_Task_ID is OK!\n", OLED_Task_ID);
    }
}

//温湿度传感器任务 io0 每次更改初始化口后都要rebuild才行
osThreadId_t DHT11_Task_ID; //任务ID

void DHT11_Task(void)
{
    
    uint8_t temp;  	    
	uint8_t humi;

    while(dht11_init())
	{
		printf("DHT11检测失败,请插好!\r\n");
		osDelay(500);; //10ms
	}
	printf("DHT11检测成功!\r\n");

    while (1) 
    {
        dht11_read_data(&temp,&humi);
        printf("[DHT11 PRI = %d]温度=%d°C  湿度=%d%%RH\r\n",osPriorityBelowNormal3,temp,humi);
        osDelay(TASKDLYTICK); //500ms
    }

}
//任务创建
void dht11_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "dht11Task";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal3; // 任务的优先级

    DHT11_Task_ID = osThreadNew((osThreadFunc_t)DHT11_Task, NULL, &taskOptions); // 创建任务
    if (DHT11_Task_ID != NULL)
    {
        printf("ID = %d, DHT11_Task_ID Create OK!\n", DHT11_Task_ID);
    }
}


//步进电机控制任务 io1,5,6,7
osThreadId_t STEP_MOTOR_Task_ID; //任务ID

void STEP_MOTOR_Task(void)
{
    step_motor_init(); // 步进电机初始化

    while (1) 
    {
        // printf("Start: Clockwise rotation, 180 degrees\n");
        // step_motor_run(4, 1, 5, 180, 1);

        // // 停止电机运行
        // printf("Stop: Motor stopped\n");
        // step_motor_run(4, 1, 5, 180, 0);
        curtain_open();
        osDelay(500); // 延迟500ms观察停止状态
    }
}

//任务创建
void step_motor_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "step_motorTask";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal4; // 任务的优先级

    STEP_MOTOR_Task_ID = osThreadNew((osThreadFunc_t)STEP_MOTOR_Task, NULL, &taskOptions); // 创建任务
    if (STEP_MOTOR_Task_ID != NULL)
    {
        printf("ID = %d, STEP_MOTOR_Task Create OK!\n", STEP_MOTOR_Task_ID);
    }
}

//人体传感器任务 io11
osThreadId_t HC_SR501_Task_ID; //任务ID

void HC_SR501_Task(void)
{
    sr501_init();
    
    while (1) 
    {       
        printf("[SR501 PRI = %d]",osPriorityBelowNormal5);
        if(1 == sr501_read())
        {
            printf("有人来了\n");
        }else {
            printf("没有检测到人\n");
        }
        osDelay(TASKDLYTICK); //500ms
    }
}
//任务创建
void sr501_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "sr501Task";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal5; // 任务的优先级

    HC_SR501_Task_ID = osThreadNew((osThreadFunc_t)HC_SR501_Task, NULL, &taskOptions); // 创建任务
    if (HC_SR501_Task_ID != NULL)
    {
        printf("ID = %d, HC_SR501_Task Create OK!\n", HC_SR501_Task_ID);
    }
}

//直流电机任务 io12
osThreadId_t DC_MOTOR_Task_ID; //任务ID

void DC_MOTOR_Task(void)
{
    dc_motor_init();//直流电机初始化
    DC_MOTOR(1);
    sleep(3);
    DC_MOTOR(0);

    while (1) 
    {
        DC_MOTOR(1);
        osDelay(TASKDLYTICK); //500ms
    }
}
//任务创建
void dc_motor_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "dc_motorTask";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal6; // 任务的优先级

    DC_MOTOR_Task_ID = osThreadNew((osThreadFunc_t)DC_MOTOR_Task, NULL, &taskOptions); // 创建任务1
    if (DC_MOTOR_Task_ID != NULL)
    {
        printf("ID = %d, DC_MOTOR_Task Create OK!\n", DC_MOTOR_Task_ID);
    }
}

//舵机控制 io5
osThreadId_t SG90_Task_ID; //任务ID

void SG90_Task(void)
{
    int16_t ang=0;
    uint8_t dir=0;

    sg90_init();

    while (1) 
    {       
        if(dir==0)
        {
            ang+=10;
            if(ang>180)dir=1;
        }
        else
        {
            ang-=10;
            if(ang<0)dir=0;            
        }
        printf("[SG90 PRI = %d]\n",osPriorityBelowNormal7);
        set_sg90_angle(ang);
        osDelay(TASKDLYTICK); //500ms
    }
}
//任务创建
void sg90_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "sg90Task";       // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityBelowNormal7; // 任务的优先级

    SG90_Task_ID = osThreadNew((osThreadFunc_t)SG90_Task, NULL, &taskOptions); // 创建任务
    if (SG90_Task_ID != NULL)
    {
        printf("ID = %d, SG90_Task_ID Create OK!\n", SG90_Task_ID);
    }
}

//烟雾传感器 io12
osThreadId_t SMOKE_Task_ID; //任务ID

void SMOKE_Task(void)
{
    
    uint16_t adc_value=0;

    led_init();//LED初始化
    adc5_init();

    while (1) 
    {
        adc_value=get_adc5_value();
        printf("[SMOKE PRI = %d]adc_value=%d\r\n",osPriorityNormal,adc_value);
        osDelay(TASKDLYTICK); //500ms
    }
}
//任务创建
void smoke_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "ModuleTask";            // 任务的名字
    taskOptions.attr_bits = 0;               // 属性位
    taskOptions.cb_mem = NULL;               // 堆空间地址
    taskOptions.cb_size = 0;                 // 堆空间大小
    taskOptions.stack_mem = NULL;            // 栈空间地址
    taskOptions.stack_size = 1024;           // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal; // 任务的优先级

    SMOKE_Task_ID = osThreadNew((osThreadFunc_t)SMOKE_Task, NULL, &taskOptions); // 创建任务1
    if (SMOKE_Task_ID != NULL)
    {
        printf("ID = %d, Create SMOKE_Task_ID is OK!\n", SMOKE_Task_ID);
    }
}

// INA219任务 scl:io9 sda:io10
osThreadId_t INA219_Task_ID; // INA219任务ID

// INA219任务
void INA219_Task(void)
{
    int16_t current;
    uint16_t bus_voltage;
    uint16_t shunt_voltage;
    uint32_t power;

    ina219_init(); // 初始化INA219传感器

    while (1) 
    {
        // 读取电流
        current = ina219_read_current();
        if (current != -1) 
        {
            printf("Current: %d mA\n", current);
        } 
        else 
        {
            printf("Failed to read current\n");
        }

        // 读取总线电压
        bus_voltage = ina219_read_bus_voltage();
        if (bus_voltage != 0xFFFF) 
        {
            printf("Bus Voltage: %d mV\n", bus_voltage);
        } 
        else 
        {
            printf("Failed to read bus voltage\n");
        }

        // 读取分流电压
        shunt_voltage = ina219_read_shunt_voltage();
        if (shunt_voltage != -1) 
        {
            printf("Shunt Voltage: %d mV\n", shunt_voltage);
        } 
        else 
        {
            printf("Failed to read shunt voltage\n");
        }

        // 计算功率
        power = ina219_calculate_power(current, bus_voltage);
        printf("Power: %d mW\n", power);

        osDelay(TASKDLYTICK); //500ms
    }
}

// INA219任务创建
void ina219_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "INA219Task";           // 任务的名字
    taskOptions.attr_bits = 0;                 // 属性位
    taskOptions.cb_mem = NULL;                 // 堆空间地址
    taskOptions.cb_size = 0;                   // 堆空间大小
    taskOptions.stack_mem = NULL;              // 栈空间地址
    taskOptions.stack_size = 1024;             // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityNormal1;   // 任务的优先级

    INA219_Task_ID = osThreadNew((osThreadFunc_t)INA219_Task, NULL, &taskOptions); // 创建INA219任务
    if (INA219_Task_ID != NULL)
    {
        printf("ID = %d, Create INA219_Task_ID is OK!\n", INA219_Task_ID);
    }
}

// 空闲任务
osThreadId_t IDLE_Task_ID; // IDLE任务ID

// IDLE任务
void IDLE_Task(void)
{
    uint64_t i=0;
    while (1) 
    {
        i++;
        if(i%50==0)
        {
            i = 0;
            printf("空闲任务运行中\n");
        }
    }
}

// IDLE任务创建
void idle_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "idleTask";           // 任务的名字
    taskOptions.attr_bits = 0;                 // 属性位
    taskOptions.cb_mem = NULL;                 // 堆空间地址
    taskOptions.cb_size = 0;                   // 堆空间大小
    taskOptions.stack_mem = NULL;              // 栈空间地址
    taskOptions.stack_size = 1024;             // 栈空间大小 单位:字节
    taskOptions.priority = osPriorityIdle;   // 任务的优先级

    IDLE_Task_ID = osThreadNew((osThreadFunc_t)IDLE_Task, NULL, &taskOptions); // 创建INA219任务
    if (INA219_Task_ID != NULL)
    {
        printf("ID = %d, Create IDLE_Task_ID is OK!\n", IDLE_Task_ID);
    }else {
        printf("Create IDLE_Task_ID Falied!\n");
    }
}

/**
 * @description: 初始化并创建任务
 * @param {*}
 * @return {*}
 */
static void template_demo(void)
{
    printf("极个别组-基于openharmony的智能家居系统\r\n");
    // led_task_create();// LED任务
    // beep_task_create();// 蜂鸣器任务
    // oledtask_create();// oled任务
    // dht11_task_create();// 温湿度传感器任务
    step_motor_task_create();// 步进电机任务
    // sr501_task_create();// 人体传感器任务
    // dc_motor_task_create();// 直流电机任务
    // sg90_task_create();// 舵机控制任务
    // smoke_task_create();// 烟雾传感器任务
    // // ina219_task_create();// INA219传感器任务
    // idle_task_create();
}
SYS_RUN(template_demo);
