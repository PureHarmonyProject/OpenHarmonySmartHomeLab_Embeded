#include "bsp_sg90.h"
#include <unistd.h>
#include "hi_time.h"
#include "bsp_uart.h"


//SG90初始化
void sg90_init(void)
{
    hi_gpio_init();                                 // GPIO初始化
    hi_io_set_func(SG90_PIN, SG90_GPIO_FUN);       // 设置IO为GPIO功能
    hi_gpio_set_dir(SG90_PIN, HI_GPIO_DIR_OUT);    // 设置GPIO为输出模式
}

//设置SG90角度
void set_sg90_angle(uint16_t angle)
{
    uint8_t i=20;
    uint16_t time=0;

    time=(angle-0)*(2500-500)/(180-0)+500;// (占空比 = (angle - 0) / 180 * (12.5%  - 2.5%) + 2.5%)
                                          // 0°：2.5%   180°：12.5%  频率50hz
    while(i--)
    {
        IoTGpioSetOutputVal(SG90_PIN,HI_GPIO_VALUE1);
        hi_udelay(time);
        IoTGpioSetOutputVal(SG90_PIN,HI_GPIO_VALUE0);
        hi_udelay(20000-time);
    }
}


uint8_t door_state = 0;  // 0 = 关闭, 1 = 打开

uint8_t door_get_curstate(void)
{
    return door_state;
}

void door_set_curstate(uint8_t cur_door_state)
{
    door_state = cur_door_state;
}

void door_send_cmd(uint8_t switch_state)
{
    uint32_t cmd = ((uint32_t)0xB << 28) |   // 帧头 `0xB`
                   ((uint32_t)0x1 << 0)  |   // 设备 ID（门 = 0x1）
                   ((uint32_t)switch_state << 4) |  // 开关状态（1 = 开门，0 = 关门）
                   ((uint32_t)0x0 << 8) |  // param1（无）
                   ((uint32_t)0x0 << 12);  // param2（无）

    uint8_t cmd_buffer[4];
    cmd_buffer[0] = (cmd >> 24) & 0xFF;
    cmd_buffer[1] = (cmd >> 16) & 0xFF;
    cmd_buffer[2] = (cmd >> 8)  & 0xFF;
    cmd_buffer[3] = cmd & 0xFF;

    printf("Sending Door Command: %02X %02X %02X %02X\n", 
           cmd_buffer[0], cmd_buffer[1], cmd_buffer[2], cmd_buffer[3]);

    uart2_send_data(cmd_buffer, 4);
}

int door_open(void) 
{
    if (door_state == 1) {
        printf("[ERROR] 门已处于打开状态，不可再被打开！！！\n");
        return 0;
    }
    printf("发送开门指令\n");
    door_state = 1;
    door_send_cmd(1);  // 发送开门指令
    return 1;
}


int door_close(void)
{
    if (door_state == 0) {
        printf("[ERROR] 门已处于关闭状态，不可再被关闭！！！\n");
        return 0;
    }
    printf("发送关门指令\n");
    door_state = 0;
    door_send_cmd(0);  // 发送关门指令
    return 1;
}