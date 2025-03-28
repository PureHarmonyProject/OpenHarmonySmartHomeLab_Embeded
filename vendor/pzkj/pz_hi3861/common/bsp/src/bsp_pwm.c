#include "bsp_pwm.h"
#include <unistd.h>
#include "bsp_led.h"
#include "hi_pwm.h"


//PWM初始化
void pwm_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_func(LED_PIN, HI_IO_FUNC_GPIO_2_PWM2_OUT);       // 设置IO为GPIO功能
    hi_gpio_set_dir(LED_PIN, HI_GPIO_DIR_OUT);                 // 设置GPIO为输出模式
    hi_pwm_init(HI_PWM_PORT_PWM2);
}

//pwm占空比输出调节
void pwm_set_duty(uint16_t duty)
{
    hi_pwm_start(HI_PWM_PORT_PWM2,duty,40000);//指定分频系数为40000 可认为重装载值就是40000 duty为ccr
}

