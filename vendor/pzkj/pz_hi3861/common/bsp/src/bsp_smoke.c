#include "bsp_smoke.h"
#include "bsp_adc.h"

void smoke_init(void)
{
    adc5_init();
}

int smoke_check(void)
{
    if(get_adc5_value() > 1000) {
        return 1;
    }else {
        return 0;
    }
}