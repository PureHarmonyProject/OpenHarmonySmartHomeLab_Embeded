/**
 ****************************************************************************************************
 * @file        bsp_sr501.c
 * @author      Dukiya
 * @version     V1.0
 * @date        2025/1/16
 * @brief       HC-SR501人体红外感应模块
 ****************************************************************************************************
*/

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
int sr501_read(void) {
    hi_gpio_value gpio_value;
    hi_gpio_get_input_val(SR_501_PIN, &gpio_value);

    // 返回传感器状态
    return (gpio_value == HI_GPIO_VALUE1); // 检测到人体时返回 HI_TRUE
}