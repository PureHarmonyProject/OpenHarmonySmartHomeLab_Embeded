#ifndef __AUTOMATION_TASK_H_
#define __AUTOMATION_TASK_H_

#include "stdint.h"
typedef uint32_t AutoModeFlags;

// ===== 基础自动化功能（0~7位） =====
#define AUTO_MODE_MOTION_LIGHT            (1 << 0)  // 有人自动开灯
#define AUTO_MODE_TEMP_AC                 (1 << 1)  // 温度控制空调
#define AUTO_MODE_SMOKE_ALARM             (1 << 2)  // 烟雾/燃气报警
#define AUTO_MODE_NFC_HOME_MODE           (1 << 3)  // NFC触发回家模式
#define AUTO_MODE_LIGHT_CURTAIN           (1 << 4)  // 光照控制窗帘
#define AUTO_MODE_AUTO_LOCK               (1 << 5)  // 离家自动锁门
#define AUTO_MODE_LIGHT_LIGHTNESS         (1 << 6)  // 光照控制光亮
#define AUTO_MODE_HUMI_AC                 (1 << 7)  // 湿度控制空调

// ===== 高级环境与状态联动（8~15位） =====
#define AUTO_MODE_TEMP_CURTAIN            (1 << 8)   // 温度控制窗帘
#define AUTO_MODE_ENERGY_SAVER            (1 << 9)   // 节能模式（自动关闭不必要设备）
#define AUTO_MODE_OCCUPANCY_LOCK          (1 << 10)  // 多传感器判断在屋保持状态
#define AUTO_MODE_SILENT_ALARM            (1 << 11)  // 静音报警（仅OLED/通知，不蜂鸣）
#define AUTO_MODE_LOW_VOLTAGE_PROTECT     (1 << 12)  // 电压过低自动关大负载
#define AUTO_MODE_NIGHT_MODE              (1 << 13)  // 晚间自动进入夜间模式（关OLED等）
#define AUTO_MODE_IDLE_SLEEP_MODE         (1 << 14)  // 无人自动休眠/关设备
#define AUTO_MODE_TIME_BLOCK_LIGHTING     (1 << 15)  // 固定时间段控制灯光

// ===== 用户自定义场景（16~23位） =====
#define AUTO_MODE_CUSTOM_SCENE1           (1 << 16)  // 自定义场景1（如影院模式）
#define AUTO_MODE_CUSTOM_SCENE2           (1 << 17)
#define AUTO_MODE_CUSTOM_SCENE3           (1 << 18)
#define AUTO_MODE_CUSTOM_SCENE4           (1 << 19)
#define AUTO_MODE_CUSTOM_SCENE5           (1 << 20)
#define AUTO_MODE_CUSTOM_SCENE6           (1 << 21)
#define AUTO_MODE_CUSTOM_SCENE7           (1 << 22)
#define AUTO_MODE_CUSTOM_SCENE8           (1 << 23)

// ===== 系统控制类功能位（24~31位） =====
#define AUTO_MODE_STATS_REPORT            (1 << 24)  // 每日能耗报告记录/发送
#define AUTO_MODE_REMOTE_WEB_CONFIG       (1 << 25)  // 允许远程网页配置开关
#define AUTO_MODE_FACTORY_DIAGNOSTIC      (1 << 26)  // 进入诊断测试模式
#define AUTO_MODE_DEBUG_DISPLAY           (1 << 27)  // OLED显示调试信息
#define AUTO_MODE_AUTO_FIRMWARE_UPDATE    (1 << 28)  // OTA自动更新允许
#define AUTO_MODE_SCENE_OVERRIDE_LOCK     (1 << 29)  // 禁止某些模式被自动切换
#define AUTO_MODE_EMERGENCY_SHUTDOWN      (1 << 30)  // 紧急断电保护（火警、过载）
#define AUTO_MODE_RESERVED                (1 << 31)  // 保留位

// ===== 由小模块组成大场景 =====
#define SCENE_LIGHT_CTRL                  (AUTO_MODE_MOTION_LIGHT | AUTO_MODE_LIGHT_LIGHTNESS)                                                       // 人体感应 + 光照控制
#define SCENE_TEMP_HUMI_CTRl              (AUTO_MODE_TEMP_AC      | AUTO_MODE_HUMI_AC               | AUTO_MODE_TEMP_CURTAIN)                        // 温度 + 湿度 + 空调 + 窗帘
#define SCENE_SECURITY_ALARM              (AUTO_MODE_SMOKE_ALARM  | AUTO_MODE_NFC_HOME_MODE         | AUTO_MODE_AUTO_LOCK | AUTO_MODE_SILENT_ALARM)  // 烟雾报警 + NFC回家模式 + 自动锁门 + 静音报警
#define SCENE_ENERGY_SAVER                (AUTO_MODE_ENERGY_SAVER | AUTO_MODE_LOW_VOLTAGE_PROTECT)                                                   // 节能模式 + 电压保护


void automation_task(void);
void automation_task_create(void);
void enable_mode(AutoModeFlags mode);
void disable_mode(AutoModeFlags mode);
uint8_t is_mode_enabled(AutoModeFlags mode);
void enable_scene(AutoModeFlags scene_mask);
void disable_scene(AutoModeFlags scene_mask);

#endif // __AUTOMATION_TASK_H_
