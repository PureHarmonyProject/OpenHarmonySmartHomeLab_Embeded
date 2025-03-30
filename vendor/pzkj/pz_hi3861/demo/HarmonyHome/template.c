#include "template.h"

extern void sensor_task_create(void);
extern void motion_sensor_task_create(void);
extern void smoke_sensor_task_create(void);
extern void uart1_task_create(void);
extern void uart2_task_create(void);
extern void wifi_iotda_task_create(void);
extern void sensor_task_create(void);
extern void automation_task_create(void);
extern void bsp_init(void);

extern bsp_oled_init(void);
static void template_demo(void)
{
    printf("极个别组-基于openharmony的智能家居系统\r\n");

    bsp_init();
    // // test_task_create();

    // motion_sensor_task_create();//貌似要等一分钟才会正常
    // oled_init();
    sensor_task_create();
    smoke_sensor_task_create();
    uart1_task_create();
    uart2_task_create();
    wifi_iotda_task_create();//任务创建
    automation_task_create();
}
SYS_RUN(template_demo);
