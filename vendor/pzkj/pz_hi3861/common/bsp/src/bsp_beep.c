#include "bsp_beep.h"


//蜂鸣器初始化
void beep_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(BEEP_PIN, HI_IO_PULL_UP);                   // 设置GPIO上拉
    hi_io_set_func(BEEP_PIN, BEEP_GPIO_FUN);                   // 设置IO为GPIO功能
    hi_gpio_set_dir(BEEP_PIN, HI_GPIO_DIR_OUT);                // 设置GPIO为输出模式
}

//蜂鸣器报警
void beep_alarm(uint16_t cnt,uint16_t time)
{
    //占空比为0.5
    //cnt决定频率 1.5k到5k最优 cnt决定响的时间，time决定频率 2khz说明500us一个周期 各一半则为250us
    while(cnt--)
    {
        BEEP(1); 
        usleep(time); 
        BEEP(0);
        usleep(time); 
    }
}

void beep_warning(void)
{
    beep_alarm(BEEP_WARNING_COUNT,BEEP_WARNING_TIME);
}

uint8_t beep_state = 0;
void beep_alarm_by_pcf8575(uint16_t cnt,uint16_t time)
{
    
    //占空比为0.5
    //cnt决定频率 1.5k到5k最优 cnt决定响的时间，time决定频率 2khz说明500us一个周期 各一半则为250us
    while(cnt--)
    {
        PF_BEEP(1); 
        usleep(time); 
        PF_BEEP(0);
        usleep(time); 
    }
}

void beep_warning_by_pcf8575(void)
{
    beep_state = 1;
    beep_alarm_by_pcf8575(BEEP_WARNING_COUNT,BEEP_WARNING_TIME);
}

hi_bool beep_get_state(void)
{
    return beep_state;
}