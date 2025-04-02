#include "automation_task.h"
#include "template.h"
#include "mqtt_task.h"

AutoModeFlags auto_mode_flags = 0;  // 初始为全关
// 开启单个模块功能位
void enable_mode(AutoModeFlags mode) {
    auto_mode_flags |= mode;
}

// 关闭单个模块功能位
void disable_mode(AutoModeFlags mode) {
    auto_mode_flags &= ~mode;
}

// 判断功能是否开启
uint8_t is_mode_enabled(AutoModeFlags mode) {
    return (auto_mode_flags & mode) != 0;
}

// 开启场景组合（一次开启多个功能位）
void enable_scene(AutoModeFlags scene_mask) {
    auto_mode_flags |= scene_mask;
}

// 关闭场景组合（一次关闭多个功能位）
void disable_scene(AutoModeFlags scene_mask) {
    auto_mode_flags &= ~scene_mask;
}

AutoModeFlags get_mode_scene(void)
{
    return auto_mode_flags;
}
void print_enabled_modes() {
    printf("当前自动化标志位: 0x%08X\n", auto_mode_flags);

    if (is_mode_enabled(AUTO_MODE_MOTION_LIGHT)) {
        printf("✅ 有人自动开灯已启用\n");
    }
    if (is_mode_enabled(AUTO_MODE_TEMP_HUMI_AC)) {
        printf("✅ 空调自动温控已启用\n");
    }
    if (is_mode_enabled(AUTO_MODE_CUSTOM_SCENE1)) {
        printf("✅ 自定义场景1 已启用\n");
    }
    // ...你可以继续列出所有功能
}

/************************************家居自动化调整函数************************************************ */
extern msg_sensorData_t sensorData;
uint8_t slow_lock = 0, up_lock = 0;;
void adjust_airConditioner_by_temp_humi(void)
{
    // 根据温度和湿度调整空调
    // 1. 温度太高，开启空调制冷
    if(sensorData.temperature_indoor > SENSOR_TEMP_MAX)
    {
        airConditioner_mode_power(AIRCONDITIONER_MODE_COOL, AIRCONDITIONER_SPEED_DEFAULT);
        printf("温度太高，开启空调制冷\n");
    }
    // 2. 温度太低，开启空调制热
    else if(sensorData.temperature_indoor < SENSOR_TEMP_MIN)
    {
        airConditioner_mode_power(AIRCONDITIONER_MODE_HEAT, AIRCONDITIONER_SPEED_DEFAULT);
        printf("温度太低，开启空调制热\n");
    }
    else
    {
        airConditioner_mode_power(AIRCONDITIONER_MODE_ORIGIN, AIRCONDITIONER_SPEED_ORIGIN);
    }
    // 3. 湿度过高，降低风速
    if(sensorData.humidity_indoor > SENSOR_HUMI_MAX)
    {
        if(slow_lock == 1) return;
        airConditioner_slow();
        printf("湿度太高，降低风速\n");
        slow_lock = 1;
    }
    else
    {
        airConditioner_mode_power(AIRCONDITIONER_MODE_ORIGIN, AIRCONDITIONER_SPEED_ORIGIN);
        slow_lock = 0;
    }
}

uint8_t curtain_open_lock = 0, curtain_close_lock = 0;
void adjust_curtain_by_temp_humi(void)
{
    // 3. 湿度过高，关闭窗帘
    // printf("窗帘自动化进行中\n");
    if(sensorData.humidity_indoor > SENSOR_HUMI_MAX)
    {
        printf("湿度太高，关闭窗帘\n");
        curtain_set_angle(0);
    }

    // 根据温度和湿度调整空调
    // 1. 温度太高，窗帘打开一些
    if(sensorData.temperature_indoor > SENSOR_TEMP_MAX)
    {
        // curtain_set_angle(2);
        if(curtain_open_lock == 1) return;
        curtain_open_angle(50);
        curtain_open_lock = 1;

        curtain_close_lock = 0;
    }
    // 2. 温度太低，窗帘关小一些
    if(sensorData.temperature_indoor < SENSOR_TEMP_MIN)
    {
        // curtain_set_angle(2);
        if(curtain_close_lock == 1) return;
        curtain_close_angle(50);
        
        curtain_open_lock = 0;
        curtain_close_lock = 1;
    }
    
}

void adjust_curtain_by_lightness(void)
{
    // 根据温度和湿度调整空调
    // 1. 亮度太低，窗帘打开一些
    if(sensorData.light < SENSOR_LIGHT_MAX)
    {
        curtain_open_angle(50);
    }
}
 
void adjust_device_by_power(void)
{
    // 根据耗能调整家居设备 功率太高或者电压太低，降低功率; 电流太高，关闭电器
    if(sensorData.power > SENSOR_POWER_MAX || sensorData.voltage < SENSOR_VOLTAGE_MIN)
    {
        // 空调降低功率
        airConditioner_low_power();
        // 开启光敏调节灯光模式
        enable_mode(AUTO_MODE_LIGHT_LIGHTNESS);
    }
    else
    {
        // 关闭光敏调节
        disable_mode(AUTO_MODE_LIGHT_LIGHTNESS);
    }

    if(sensorData.current > SENSOR_CURRENT_MAX)
    {
        // 关闭电器
        airConditioner_stop();
        // 开启温湿度调节窗帘
        enable_mode(AUTO_MODE_TEMP_HUMI_CURTAIN);
    }
    else
    {
        // 关闭温湿度调节窗帘
        disable_mode(AUTO_MODE_TEMP_HUMI_CURTAIN);
    }
}
/************************************家居自动化总控函数************************************************ */
// 灯光 人体感应 + 光强
void light_task_control(void)
{
    if(is_mode_enabled(AUTO_MODE_LIGHT_LIGHTNESS))
    {
        adjust_led_brightness_by_light_sensor();
    }

    if(is_mode_enabled(AUTO_MODE_MOTION_LIGHT))
    {
        adjust_led_brightness_by_motion_sensor();
    }
}

// 空调 温度 + 湿度
void airConditioner_task_control(void)
{
    if(is_mode_enabled(AUTO_MODE_TEMP_HUMI_AC))
    {
        adjust_airConditioner_by_temp_humi();
    }
}

// 窗帘 温度 + 湿度
void curtain_task_control(void)
{
    if(is_mode_enabled(AUTO_MODE_TEMP_HUMI_CURTAIN))
    {
        adjust_curtain_by_temp_humi();
    }
    if(is_mode_enabled(AUTO_MODE_LIGHT_CURTAIN))
    {
        adjust_curtain_by_lightness();
    }
}

// 节能减排
void power_task_control(void)
{
    if(is_mode_enabled(AUTO_MODE_ENERGY_SAVER))
    {
        adjust_device_by_power();
    }
}
/************************************全体自动化总控任务************************************************ */
osThreadId_t Automation_Task_ID;
void automation_task(void)
{
    // enable_mode(AUTO_MODE_LIGHT_LIGHTNESS);
    // enable_mode(AUTO_MODE_MOTION_LIGHT);
    while (1) 
    {
        light_task_control();
        airConditioner_task_control();
        curtain_task_control();
        power_task_control();
        osDelay(TASK_DELAY_1000MS);
    }
}

void automation_task_create(void)
{
    osThreadAttr_t taskOptions;
    taskOptions.name = "automation_task";
    taskOptions.attr_bits = 0;
    taskOptions.cb_mem = NULL;
    taskOptions.cb_size = 1024;
    taskOptions.stack_mem = NULL;
    taskOptions.stack_size = 4096;
    taskOptions.priority = osPriorityAboveNormal5;

    Automation_Task_ID = osThreadNew((osThreadFunc_t)automation_task, NULL, &taskOptions);
    if (Automation_Task_ID != NULL)
    {
        printf("ID = %d, Create Automation_Task_ID is OK!\n", Automation_Task_ID);
    }
}
