#include "bsp_key.h"
#include <unistd.h>

//按键初始化
void key_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(KEY1_PIN, HI_IO_PULL_UP);                   // 设置GPIO上拉
    hi_io_set_func(KEY1_PIN, KEY1_GPIO_FUN);                   // 设置IO为GPIO功能
    hi_gpio_set_dir(KEY1_PIN, HI_GPIO_DIR_IN);                // 设置GPIO为输入模式

    hi_io_set_pull(KEY2_PIN, HI_IO_PULL_UP);                   // 设置GPIO上拉
    hi_io_set_func(KEY2_PIN, KEY2_GPIO_FUN);                   // 设置IO为GPIO功能
    hi_gpio_set_dir(KEY2_PIN, HI_GPIO_DIR_IN);                // 设置GPIO为输入模式
}

/*******************************************************************************
* 函 数 名       : key_scan
* 函数功能        : 检测独立按键是否按下，按下则返回对应键值
* 输    入       : mode=0：单次扫描按键
                  mode=1：连续扫描按键
* 输    出       : KEY1_PRESS：K1按下
                  KEY2_PRESS：K2按下
                  KEY_UNPRESS：未有按键按下
*******************************************************************************/
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key1=0;
    static uint8_t key2=0;

    hi_gpio_get_input_val(KEY1_PIN,&key1);
    hi_gpio_get_input_val(KEY2_PIN,&key2);

    if(key1==0 || key2==0)
    {
        usleep(10*1000);//消抖
        hi_gpio_get_input_val(KEY1_PIN,&key1);
        hi_gpio_get_input_val(KEY2_PIN,&key2);
        if(key1==0 || key2==0)
        {
            if(key1==0)return KEY1_PRESS;
            else if(key2==0)return KEY2_PRESS;
        }
    }
    return KEY_UNPRESS;   
}
