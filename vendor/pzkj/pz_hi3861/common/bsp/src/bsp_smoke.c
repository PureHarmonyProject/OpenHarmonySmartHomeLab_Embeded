#include "bsp_smoke.h"
#include "bsp_adc.h"

void smoke_init(void)
{
    adc6_init();
}

int smoke_check(void)
{
    uint16_t smoke_val = get_adc5_value();
    printf("smoke_val = %d\n",smoke_val);
    if(smoke_val > 10000) {
        return 1;
    }else {
        return 0;
    }
}

uint16_t smoke_get_value(void)
{
    return get_adc5_value();
}

void MQ5_init(void)
{
    adc2_init();
}

uint16_t MQ5_get_value(void)
{
    return get_adc2_value();
}