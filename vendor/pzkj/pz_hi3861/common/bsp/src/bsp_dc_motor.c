#include "bsp_dc_motor.h"
#include "hi_pwm.h"

//直流电机初始化
void airConditioner_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_func(DC_MOTOR_PIN1, DC_MOTOR_GPIO2_FUN);           // 设置IO为pwm模式
    hi_gpio_set_dir(DC_MOTOR_PIN1, HI_GPIO_DIR_OUT);            // 设置GPIO为输出模式
    hi_pwm_init(HI_PWM_PORT_PWM2);

    hi_io_set_func(DC_MOTOR_PIN2, DC_MOTOR_GPIO6_FUN);           // 设置IO为pwm模式
    hi_gpio_set_dir(DC_MOTOR_PIN2, HI_GPIO_DIR_OUT);            // 设置GPIO为输出模式
    hi_pwm_init(HI_PWM_PORT_PWM3);

    // hi_u32 ret = hi_pwm_start(HI_PWM_PORT_PWM3,40000,40000);
    // if(ret != HI_ERR_SUCCESS) {
    //     printf("PWM3 start failed\n");
    // }else {
    //     printf("PWM3 start success\n");
    // }
}

//pwm占空比输出调节
static void dc_motor_pwm_set_duty(uint16_t duty)
{
    hi_pwm_start(HI_PWM_PORT_PWM3,duty,40000);//指定分频系数为40000 可认为重装载值就是40000 duty为ccr
}

//用pwm实现电机调速
//只分三个档 低 中 高速
int32_t speed_array[] = {20000,30000,40000}; //速度数组
void dc_motor_run(int type)
{
    if(type < DC_MOTOR_LOW || type > DC_MOTOR_HIGH) {
        return;
    }
    pwm_set_duty(speed_array[type]);
}

void airConditioner_heat(int type)
{
    if(type < DC_MOTOR_LOW || type > DC_MOTOR_HIGH) {
        return;
    }
    hi_pwm_stop(HI_PWM_PORT_PWM2);
    hi_pwm_stop(HI_PWM_PORT_PWM3);
    osDelay(10);  // 确保PWM已完全停止

    printf("空调制热\n,速度为%d\n", type);
    hi_pwm_start(HI_PWM_PORT_PWM3,speed_array[type],40000);
    hi_pwm_start(HI_PWM_PORT_PWM2,0,40000);
}

void airConditioner_cool(int type)
{
    if(type < DC_MOTOR_LOW || type > DC_MOTOR_HIGH) {
        return;
    }
    hi_pwm_stop(HI_PWM_PORT_PWM2);
    hi_pwm_stop(HI_PWM_PORT_PWM3);
    osDelay(10);  // 确保PWM已完全停止

    printf("空调制冷,速度为%d\n", type);
    hi_pwm_start(HI_PWM_PORT_PWM2,speed_array[type],40000);
    hi_pwm_start(HI_PWM_PORT_PWM3,0,40000);
}

void airConditioner_stop(void)
{
    printf("空调停转\n");
    hi_pwm_stop(HI_PWM_PORT_PWM2);
    hi_pwm_stop(HI_PWM_PORT_PWM3);
    osDelay(10);  // 确保PWM已完全停止
}

uint8_t cur_state;
void airConditioner_work(uint8_t airConditioner_state)
{
    cur_state = airConditioner_state;
    int func_type = airConditioner_state & 0x1;
    int speed_type = (airConditioner_state >> 1) & 0x3;

    printf("func_typr = %d, speed_type = %d\n",func_type, speed_type);
    // 确保 speed_type 在合法范围内
    if (speed_type == 0 || speed_type > 3) {
        airConditioner_stop();
        printf("airConditioner stop\n");
        return;
    }

    // // 避免重复执行相同状态
    // if (cur_state == airConditioner_state) {
    //     printf("airConditioner state unchanged, skipping PWM update.\n");
    //     return;
    // }

    if (func_type == 0) {
        airConditioner_heat(speed_type - 1);
        printf("airConditioner heat, type = %d\n", speed_type);
    } else {
        airConditioner_cool(speed_type - 1);
        printf("airConditioner cool, type = %d\n", speed_type);
    }

    
}


uint8_t airConditioner_getState(void)
{
    return cur_state;
}