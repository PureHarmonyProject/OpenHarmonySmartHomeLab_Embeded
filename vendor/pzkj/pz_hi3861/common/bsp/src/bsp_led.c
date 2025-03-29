#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_smoke.h"

//LED初始化
void led_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(LED_PIN, HI_IO_PULL_DOWN);                  // 设置GPIO下拉
    hi_io_set_func(LED_PIN, LED_GPIO_FUN);                      // 设置IO为GPIO功能
    hi_gpio_set_dir(LED_PIN, HI_GPIO_DIR_OUT);                 // 设置GPIO为输出模式
}

void led_on() 
{
    LED(1);
}

void led_off(void) 
{
    LED(0);
}

void led_warning(void)
{
    printf("准备开始闪烁\n");
    uint32_t count = LED_WARNING_COUNT;
    while(count--)
    {
        led_on();
        usleep(LED_DELAY_TIME_MS * 1000);
        led_off();
        usleep(LED_DELAY_TIME_MS * 1000);
    }
    printf("闪烁完毕\n");
}

void led_on_by_pcf8575() 
{
    LED(1);
}

void led_off_by_pcf8575(void) 
{
    LED(0);
}

uint32_t cur_color_light_state;
void led_set_color_and_brightness(uint32_t led_color_and_brightness)
{
    uint8_t buffer[4] = {
        (led_color_and_brightness >> 24) & 0xff,  // 高字节 (帧头 + 亮度)
        (led_color_and_brightness >> 16) & 0xff,  // R 分量
        (led_color_and_brightness >> 8) & 0xff,   // G 分量
        (led_color_and_brightness >> 0) & 0xff    // B 分量
    };

    cur_color_light_state = led_color_and_brightness;
    uart1_send_data(buffer, sizeof(buffer));  // 发送 4 字节数据
}

uint32_t led_get_color_and_brightness(void)
{
    return cur_color_light_state;
}

void led_update_color_and_brightness(uint32_t new_color_brightness)
{
    cur_color_light_state = new_color_brightness;
}

void adjust_led_brightness_by_light_sensor(void)
{
    uint32_t light_value = light_get_value();  // 获取光敏传感器数值
    printf("[LIGHT] 光敏传感器数值 = %d\n", light_value);

    // 映射亮度值，确保在 0~15 范围内
    uint32_t brightness = (light_value * 15) / 2200;

    if (brightness > 15) {
        brightness = 15;  // 限制最大亮度
    }

    if (brightness < 0) {
        brightness = 0;  // 限制最小亮度
    }
    // 获取当前 LED 颜色和亮度
    uint32_t current_led_state = led_get_color_and_brightness();
    
    // 只修改亮度部分，保留 RGB 颜色
    uint32_t new_led_state = (0xA << 28) | (brightness << 24) | (current_led_state & 0x00FFFFFF);

    // printf("[DEBUG] Light Value = %u\n", light_value);
    // printf("[DEBUG] Brightness = %u\n", brightness);
    // printf("[DEBUG] Current LED State = 0x%X\n", current_led_state);
    printf("[DEBUG] New LED State = 0x%X\n", new_led_state);
    // 发送 LED 颜色和亮度调整命令
    led_set_color_and_brightness(new_led_state);
}
