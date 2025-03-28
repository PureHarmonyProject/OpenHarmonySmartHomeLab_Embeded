#include "bsp_smoke.h"
#include "bsp_adc.h"
#include <unistd.h>  // for usleep

#define FILTER_WINDOW_SIZE 5  // 滑动窗口大小

void smoke_init(void)
{
    adc6_init();
}

int smoke_check(void)
{
    uint16_t smoke_val = smoke_get_value();  // 用滤波后的值判断
    printf("smoke_val = %d\n", smoke_val);
    return smoke_val > 10000 ? 1 : 0;
}

// 加入滑动平均滤波
uint16_t smoke_get_value(void)
{
    uint32_t sum = 0;
    for (int i = 0; i < FILTER_WINDOW_SIZE; i++) {
        sum += get_adc6_value();
        usleep(3000);  // 小延时，防止ADC抖动重复
    }
    return sum / FILTER_WINDOW_SIZE;
}

// MQ5 燃气传感器部分同样加入滤波
void MQ5_init(void)
{
    adc2_init();
}

uint16_t MQ5_get_value(void)
{
    uint32_t sum = 0;
    for (int i = 0; i < FILTER_WINDOW_SIZE; i++) {
        sum += get_adc2_value();
        usleep(3000);
    }
    return sum / FILTER_WINDOW_SIZE;
}

// 光照传感器保留原始（如需也加可复制模板）
void light_init(void)
{
    adc3_init();
}

uint16_t light_get_value(void)
{
    return get_adc3_value();
}
