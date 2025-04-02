#ifndef __TEMPLATE_H_
#define __TEMPLATE_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "cmsis_os2.h"
#include <unistd.h>
#include "ohos_init.h"

#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_oled.h"
#include "bsp_step_motor.h"
#include "bsp_sr501.h"
#include "bsp_dc_motor.h"
#include "bsp_sg90.h"
#include "bsp_smoke.h"
#include "bsp_ina226.h"
#include "bsp_uart.h"
// #include "bsp_ds18b20.h"
#include "bsp_pcf8575.h"
#include "bsp_nfc.h"
#include "bsp_rc522.h"

#include "bsp_wifi.h"
#include "bsp_mqtt.h"

#include "bsp_pwm.h"

#include "lwip/netifapi.h"
#include "lwip/sockets.h"
#include "lwip/api_shell.h"

#include "hi_pwm.h" //test
#include "cJSON.h"
#define TASKDLYTICK    500
#define TASK_DELAY_5000MS 500    //500 = 5000ms 1tick = 10ms
#define TASK_DELAY_1000MS  100    //100 = 1000ms 1tick = 10ms
#define TASK_DELAY_100MS 10    //10 = 100ms 1tick = 10ms
#define TASKDLYTICK    500

#define TASK_DELAY_100MS 10    //10 = 100ms 1tick = 10ms
#define TASK_DELAY_3000MS 300
#define TASK_DELAY_5000MS 500    //500 = 5000ms 1tick = 10ms
#define TASK_DELAY_1000MS  100    //100 = 1000ms 1tick = 10ms


#endif // __TEST_TASK_H__
