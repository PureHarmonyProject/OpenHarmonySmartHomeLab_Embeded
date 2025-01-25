#include "bsp_adc.h"
#include <unistd.h>
#include "hi_adc.h"

//ADC初始化
void adc5_init(void)
{
    hi_gpio_init();                                            // GPIO初始化
    hi_io_set_pull(ADC5_PIN, HI_IO_PULL_UP);                   // 设置GPIO上拉
}

//读取AD值
uint16_t get_adc5_value(void)
{
    uint16_t data;
    hi_adc_read(HI_ADC_CHANNEL_5,&data,HI_ADC_EQU_MODEL_8,HI_ADC_CUR_BAIS_DEFAULT,0xff);
    return data;
}

//读取电压
float get_adc5_voltage(void)
{
    return hi_adc_convert_to_voltage(get_adc5_value());
}

