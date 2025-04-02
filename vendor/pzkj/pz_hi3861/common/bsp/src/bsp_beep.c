#include "bsp_beep.h"
#include "hi_pwm.h"

uint8_t beep_state = 0;
//蜂鸣器初始化
void beep_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_func(BEEP_PIN, HI_IO_FUNC_GPIO_8_PWM1_OUT);      // 设置IO为PWM输出
    hi_gpio_set_dir(BEEP_PIN, HI_GPIO_DIR_OUT);               // 设置GPIO为输出模式
    hi_pwm_init(HI_PWM_PORT_PWM1);                            // 初始化PWM端口
}

// 设置蜂鸣器 PWM 输出的占空比
void beep_alarm(uint16_t cnt, uint16_t time)
{
    while (cnt--)
    {
        beep_set_duty(20000); // 50% 占空比 (20000/40000)
        usleep(time);         // 持续 time 时间
        beep_stop();          // 关闭蜂鸣器
        usleep(time);         // 停止 time 时间
    }
}

// 设置蜂鸣器 PWM 输出的占空比
void beep_set_duty(uint16_t duty)
{
    hi_pwm_start(HI_PWM_PORT_PWM1, duty, 40000);  // 40000 作为分频系数
}

// 关闭蜂鸣器
void beep_stop(void)
{
    beep_state = 0;
    hi_pwm_stop(HI_PWM_PORT_PWM1);
}

void beep_warning(void)
{
    beep_state = 1;
    beep_alarm(BEEP_WARNING_COUNT,BEEP_WARNING_TIME);
}


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