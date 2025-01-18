/**
 ****************************************************************************************************
 * @file        template.c
 * @author      普中科技
 * @version     V1.0
 * @date        2024-06-05
 * @brief       步进电机实验
 * @license     Copyright (c) 2024-2034, 深圳市普中科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:普中-Hi3861
 * 在线视频:https://space.bilibili.com/2146492485
 * 公司网址:www.prechin.cn
 * 购买地址:
 *
 ****************************************************************************************************
 * 实验现象：步进电机运行，K1控制方向，K2调速
 *
 ****************************************************************************************************
 */

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

//LED任务 io2
osThreadId_t LED_Task_ID; //led任务ID

void LED_Task(void)
{
    led_init();//LED初始化

    while (1) 
    {
        LED(1); 
        usleep(200*1000); //200ms
        LED(0);
        usleep(200*1000); //200ms
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
    taskOptions.priority = osPriorityNormal; // 任务的优先级

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
        usleep(10*1000);
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
    taskOptions.priority = osPriorityNormal; // 任务的优先级

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
        usleep(10*1000); //10ms
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
    taskOptions.priority = osPriorityNormal; // 任务的优先级

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
    static uint8_t i=0;
    uint8_t temp;  	    
	uint8_t humi;

    while(dht11_init())
	{
		printf("DHT11检测失败,请插好!\r\n");
		usleep(500*1000); //500ms
	}
	printf("DHT11检测成功!\r\n");

    while (1) 
    {
        i++;
        if(i%50==0)
        {
            dht11_read_data(&temp,&humi);
			printf("温度=%d°C  湿度=%d%%RH\r\n",temp,humi);
        }

        usleep(10*1000); //10ms
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
    taskOptions.priority = osPriorityNormal1; // 任务的优先级

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
    uint8_t key=0;
    uint8_t dir=0;
	uint8_t speed=STEPMOTOR_MAXSPEED;

    step_motor_init();//步进电机初始化
    key_init();//按键初始化

    while (1) 
    {
        key=key_scan(0);//按键扫描
        if(key==KEY1_PRESS)//方向
            dir=!dir;
        else if(key==KEY2_PRESS)//加速
        {
			speed--;
			if(speed<STEPMOTOR_MAXSPEED)speed=STEPMOTOR_MINSPEED;
		}
        
        step_motor_run(8,dir,speed,45,1);
        printf("步进电机前进中\n");
        
        usleep(10*1000);
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
    taskOptions.priority = osPriorityNormal; // 任务的优先级

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
        if(1 == sr501_read())
        {
            printf("有人来了\n");
        }else {
            printf("没有检测到人\n");
        }
        
        usleep(10*1000);
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
    taskOptions.priority = osPriorityNormal; // 任务的优先级

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
        usleep(10*1000);
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
    taskOptions.priority = osPriorityNormal; // 任务的优先级

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
        set_sg90_angle(ang);
        usleep(200*1000); //20ms
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
    taskOptions.priority = osPriorityNormal1; // 任务的优先级

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
    uint8_t i=0;
    uint16_t adc_value=0;

    led_init();//LED初始化
    adc5_init();

    while (1) 
    {
        i++;
        if(i%50==0)
        {
            adc_value=get_adc5_value();
            printf("adc_value=%d\r\n",adc_value);
        }
        usleep(10*1000);
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

        usleep(1000 * 1000); // 每秒读取一次数据
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
    taskOptions.priority = osPriorityNormal;   // 任务的优先级

    INA219_Task_ID = osThreadNew((osThreadFunc_t)INA219_Task, NULL, &taskOptions); // 创建INA219任务
    if (INA219_Task_ID != NULL)
    {
        printf("ID = %d, Create INA219_Task_ID is OK!\n", INA219_Task_ID);
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
    led_task_create();// LED任务
    beep_task_create();// 蜂鸣器任务
    oledtask_create();// oled任务
    dht11_task_create();// 温湿度传感器任务
    step_motor_task_create();// 步进电机任务
    // sr501_task_create();// 人体传感器任务
    // dc_motor_task_create();// 直流电机任务
    sg90_task_create();// 舵机控制任务
    smoke_task_create();// 烟雾传感器任务
    // ina219_task_create();// INA219传感器任务
}
SYS_RUN(template_demo);
