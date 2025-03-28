#include "bsp_relay.h"


//继电器初始化
void relay_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(RELAY_PIN, HI_IO_PULL_DOWN);                   // 设置GPIO下拉
    hi_io_set_func(RELAY_PIN, RELAY_GPIO_FUN);                   // 设置IO为GPIO功能
    hi_gpio_set_dir(RELAY_PIN, HI_GPIO_DIR_OUT);                // 设置GPIO为输出模式
}

