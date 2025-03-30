#include "bsp_sr501.h"
#include <unistd.h>
#include "hi_time.h"

void sr501_init(void)
{
	hi_gpio_init();                                            // GPIO初始化
	hi_io_set_pull(SR_501_PIN, HI_IO_PULL_DOWN);             // 设置GPIO下拉
	hi_io_set_func(SR_501_PIN, SR_501_GPIO_FUN);              // 设置IO为GPIO功能
    hi_gpio_set_dir(SR_501_PIN, HI_GPIO_DIR_IN);             // 设置GPIO为输入模式
}

// 读取 HC-SR501 状态
static int sr501_read(void) {
    hi_gpio_value gpio_value;
    hi_gpio_get_input_val(SR_501_PIN, &gpio_value);
    printf("sr501_read: %d\n",gpio_value);
    // 返回传感器状态
    return (gpio_value == HI_GPIO_VALUE1); // 检测到人体时返回 HI_TRUE
}

int sr501_check(void) {
    return sr501_read();
}

adjust_led_brightness_by_motion_sensor()
{
    if (sr501_check()) {
        // uint32_t current_led_state = led_get_color_and_brightness();
    
        // // 只修改亮度部分，保留 RGB 颜色
        // uint32_t new_led_state = (0xA << 28) | (0xA << 24) | (current_led_state & 0x00FFFFFF);

        // printf("[DEBUG][SR501] New LED State = 0x%X\n", new_led_state);
        // 发送 LED 颜色和亮度调整命令
        led_set_color_and_brightness(0xAFFFFFFF);
    } else {
        // uint32_t current_led_state = led_get_color_and_brightness();
    
        // // 只修改亮度部分，保留 RGB 颜色
        // uint32_t new_led_state = (0xA << 28) | (0x0 << 24) | (current_led_state & 0x00FFFFFF);

        // printf("[DEBUG] New LED State = 0x%X\n", new_led_state);
        // 发送 LED 颜色和亮度调整命令
        led_set_color_and_brightness(0xA0000000);
    }
}