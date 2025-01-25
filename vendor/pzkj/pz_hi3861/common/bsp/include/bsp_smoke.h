#ifndef BSP_smoke_H
#define BSP_smoke_H

#include "cmsis_os2.h"
#include "hi_io.h"
#include "hi_gpio.h"

void smoke_init(void);
int smoke_check(void);
#endif
