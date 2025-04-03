#include "dht11_oled_task.h"
#include "template.h"



uint8_t temp, humi;

#include "bsp_ina226.h"
#include "bsp_smoke.h"
#include "bsp_sg90.h"

uint8_t current_page = 0;
uint8_t page_mode = 0;  // 0:常规模式，正常进行三个页面的切换 1: 紧急模式，展示情况特征，禁止页面切换，一段时间后恢复：2: 此外，当存在语音命令下发时，增加oled显示
uint8_t emergency_count = 0, order_count = 0;
// 紧急模式在以下情况触发
// 1. 烟雾传感器检测到烟雾、液化石油气传感器监测到气体泄漏
// 2. 陌生人强制闯入
// 3. 电流过流预警
void display_sensor_data(void)
{
    char line1[32];
    char line2[32];
    char line3[32];

    oled_clear();
    if(page_mode == 0)
    {
        // 顶部标题栏
        oled_fill_rectangle(0, 0, 128, 12, 1);
        oled_showstring(24, 2, (uint8_t *)" Smart Panel ", 12);

        switch (current_page) {
            case 0:  // Page 0: 温湿度
                dht11_read_data(&temp, &humi);
                snprintf(line1, sizeof(line1), "Temp     : %2d C", temp);
                snprintf(line2, sizeof(line2), "Humidity : %2d %%", humi);
                oled_showstring(0, 16, (uint8_t *)line1, 12);
                oled_showstring(0, 28, (uint8_t *)line2, 12);
                break;

            case 1:  // Page 1: 电流/电压/功率
            {
                float current = ina226_get_current();
                float voltage = ina226_get_bus_voltage();
                float power = current * voltage / 1000.0; // mW to W

                snprintf(line1, sizeof(line1), "Voltage  : %.1f mV", voltage);
                snprintf(line2, sizeof(line2), "Current  : %.1f mA", current);
                snprintf(line3, sizeof(line3), "Power    : %.2f mW", power);

                oled_showstring(0, 16, (uint8_t *)line1, 12);
                oled_showstring(0, 28, (uint8_t *)line2, 12);
                oled_showstring(0, 40, (uint8_t *)line3, 12);
                break;
            }

            case 2:  // Page 2: 门锁 + 烟雾 + 液化气体
            {
                uint16_t smoke = smoke_get_value();
                uint16_t gas = MQ5_get_value();
                uint8_t door = door_get_curstate();  // 1=开, 0=关

                snprintf(line1, sizeof(line1), "Door     : %s", door ? "Open" : "Closed");
                snprintf(line2, sizeof(line2), "Smoke    : %u", smoke);
                snprintf(line3, sizeof(line3), "Gas(MQ5) : %u", gas);

                oled_showstring(0, 16, (uint8_t *)line1, 12);
                oled_showstring(0, 28, (uint8_t *)line2, 12);
                oled_showstring(0, 40, (uint8_t *)line3, 12);
                break;
            }
        }

        // 分割线 + 状态栏
        oled_draw_hline(0, 51, 128, 1);

        char status[32];
        snprintf(status, sizeof(status), "Page: %d", current_page + 1);
        oled_showstring(0, 52, (uint8_t *)status, 12);

        oled_refresh_gram();

        // 切换页码：0 -> 1 -> 2 -> 0 ...
        current_page = (current_page + 1) % 3;
    }
    else if(page_mode == 1)
    {
        // 紧急模式
        // TODO: 在这里添加紧急模式下的显示内容
        emergency_count++;
        if(emergency_count > 10)
        {
            page_mode = 0;
            emergency_count = 0;
        }
    }
    else if(page_mode == 2)
    {
        // TODO: 在这里添加语音命令下的显示内容
        order_count++;
        if(order_count >= 1)
        {
            page_mode = 0;
            order_count = 0;
        }
    }
    
}

osThreadId_t Sensor_Task_ID;
void sensor_task(void)
{
    while (1) 
    {
        display_sensor_data();
        printf("更新传感器数据\n");
        osDelay(TASK_DELAY_3000MS);
    }
}

void sensor_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "SensorTask";
    taskOptions.attr_bits = 0;
    taskOptions.cb_mem = NULL;
    taskOptions.cb_size = 1024;
    taskOptions.stack_mem = NULL;
    taskOptions.stack_size = 4096;
    taskOptions.priority = osPriorityAboveNormal;

    Sensor_Task_ID = osThreadNew((osThreadFunc_t)sensor_task, NULL, &taskOptions);
    if (Sensor_Task_ID != NULL)
    {
        printf("ID = %d, Create Sensor_Task_ID is OK!\n", Sensor_Task_ID);
    }
}

void oled_showstring_center(uint8_t y, uint8_t *str, uint8_t font_size)
{
    uint8_t char_width = (font_size == 16) ? 8 : 6;  // 常见字体宽度
    uint8_t len = strlen((const char *)str);
    uint8_t x = (128 - (len * char_width)) / 2;
    oled_showstring(x, y, str, font_size);
}
