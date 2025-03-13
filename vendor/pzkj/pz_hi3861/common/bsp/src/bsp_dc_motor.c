#include "bsp_dc_motor.h"
#include "hi_pwm.h"
#include "bsp_uart.h"
//直流电机初始化
// void airConditioner_init(void)
// {
//     hi_gpio_init();                                            // GPIO初始化
//     hi_io_set_func(DC_MOTOR_PIN1, DC_MOTOR_GPIO2_FUN);           // 设置IO为pwm模式
//     hi_gpio_set_dir(DC_MOTOR_PIN1, HI_GPIO_DIR_OUT);            // 设置GPIO为输出模式
//     hi_pwm_init(HI_PWM_PORT_PWM2);

//     hi_io_set_func(DC_MOTOR_PIN2, DC_MOTOR_GPIO6_FUN);           // 设置IO为pwm模式
//     hi_gpio_set_dir(DC_MOTOR_PIN2, HI_GPIO_DIR_OUT);            // 设置GPIO为输出模式
//     hi_pwm_init(HI_PWM_PORT_PWM3);
// }

void airConditioner_send_cmd(uint8_t switch_state, uint8_t speed, uint8_t mode)
{
    uint32_t cmd = ((uint32_t)0xB << 28) |   // 帧头 `0xB`
                   ((uint32_t)0x3 << 0)  |   // 设备 ID（空调 = 0x3）
                   ((uint32_t)switch_state << 4) |  // 开关状态
                   ((uint32_t)speed << 8) |  // 风速
                   ((uint32_t)mode << 12);   // 制冷/制热模式

    uint8_t cmd_buffer[4];
    cmd_buffer[0] = (cmd >> 24) & 0xFF;
    cmd_buffer[1] = (cmd >> 16) & 0xFF;
    cmd_buffer[2] = (cmd >> 8)  & 0xFF;
    cmd_buffer[3] = cmd & 0xFF;

    printf("Sending Air Conditioner Command: %02X %02X %02X %02X\n", 
           cmd_buffer[0], cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);

    uart2_send_data(cmd_buffer, 4);
}

//pwm占空比输出调节
void airConditioner_heat(uint8_t speed)
{
    if (speed < 1 || speed > 3) {
        printf("Invalid speed level for heating!\n");
        return;
    }
    printf("Sending command: Heat, speed=%d\n", speed);
    airConditioner_send_cmd(1, speed, 1);  // 开启空调，模式=1（制热）
}


void airConditioner_cool(uint8_t speed)
{
    if (speed < 1 || speed > 3) {
        printf("Invalid speed level for cooling!\n");
        return;
    }
    printf("Sending command: Cool, speed=%d\n", speed);
    airConditioner_send_cmd(1, speed, 0);  // 开启空调，模式=0（制冷）
}
void airConditioner_stop(void)
{
    printf("Sending command: Stop Air Conditioner\n");
    airConditioner_send_cmd(0, 0, 0);  // 关闭空调
}


uint8_t cur_state;
void airConditioner_work(uint8_t airConditioner_state)
{
    cur_state = airConditioner_state;
    uint8_t func_type = airConditioner_state & 0x1;
    uint8_t speed_type = (airConditioner_state >> 1) & 0x3;

    printf("Processing airConditioner_work: func_type=%d, speed_type=%d\n", func_type, speed_type);

    // 确保 speed_type 合法（范围 1~3）
    if (speed_type < 1 || speed_type > 3) {
        airConditioner_stop();
        printf("Invalid speed! Stopping air conditioner.\n");
        return;
    }

    if (func_type == 0) {
        airConditioner_heat(speed_type);
    } else {
        airConditioner_cool(speed_type);
    }
}

uint8_t airConditioner_getState(void)
{
    return cur_state;
}