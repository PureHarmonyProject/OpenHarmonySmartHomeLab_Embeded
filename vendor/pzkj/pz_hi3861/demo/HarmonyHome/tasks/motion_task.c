#include "motion_task.h"
#include "template.h"

osThreadId_t MOTION_Task_ID; //任务ID

void motion_sensor_task(void)
{
    int flag = 0;
    int flag1 = 0;
    int value = 0, prevalue = 0;
    // sr501_init();
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