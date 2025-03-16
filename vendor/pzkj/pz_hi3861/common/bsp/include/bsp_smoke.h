#ifndef BSP_smoke_H
#define BSP_smoke_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

void smoke_init(void);
int smoke_check(void);
uint16_t smoke_get_value(void);
void MQ5_init(void);
uint16_t MQ5_get_value(void);
void light_init(void);
uint16_t light_get_value(void);
#endif
