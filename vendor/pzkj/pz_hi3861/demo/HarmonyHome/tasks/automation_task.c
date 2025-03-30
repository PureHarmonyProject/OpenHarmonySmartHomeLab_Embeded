#include "automation_task.h"
#include "template.h"

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

void print_enabled_modes() {
    printf("当前自动化标志位: 0x%08X\n", auto_mode_flags);

    if (is_mode_enabled(AUTO_MODE_MOTION_LIGHT)) {
        printf("✅ 有人自动开灯已启用\n");
    }
    if (is_mode_enabled(AUTO_MODE_TEMP_AC)) {
        printf("✅ 空调自动温控已启用\n");
    }
    if (is_mode_enabled(AUTO_MODE_CUSTOM_SCENE1)) {
        printf("✅ 自定义场景1 已启用\n");
    }
    // ...你可以继续列出所有功能
}

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

osThreadId_t Automation_Task_ID;

void automation_task(void)
{
    // enable_mode(AUTO_MODE_LIGHT_LIGHTNESS);
    enable_mode(AUTO_MODE_MOTION_LIGHT);
    while (1) 
    {
        light_task_control();
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
