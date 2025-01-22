#include "D:\IntelliHome\src\vendor\dukiya\dukiya_hi3861\common\bsp\include\bsp_step_motor.h"
//步进电机初始化
void step_motor_init(void)
{
    hi_gpio_init();                                     // GPIO初始化
    hi_io_set_pull(IN1_PIN, HI_IO_PULL_UP);             // 设置GPIO上拉
    hi_io_set_func(IN1_PIN, IN1_GPIO_FUN);              // 设置IO为GPIO功能
    hi_gpio_set_dir(IN1_PIN, HI_GPIO_DIR_OUT);          // 设置GPIO为输出模式

    hi_io_set_pull(IN2_PIN, HI_IO_PULL_UP);             // 设置GPIO上拉
    hi_io_set_func(IN2_PIN, IN2_GPIO_FUN);              // 设置IO为GPIO功能
    hi_gpio_set_dir(IN2_PIN, HI_GPIO_DIR_OUT);          // 设置GPIO为输出模式

    hi_io_set_pull(IN3_PIN, HI_IO_PULL_UP);             // 设置GPIO上拉
    hi_io_set_func(IN3_PIN, IN3_GPIO_FUN);              // 设置IO为GPIO功能
    hi_gpio_set_dir(IN3_PIN, HI_GPIO_DIR_OUT);          // 设置GPIO为输出模式

    hi_io_set_pull(IN4_PIN, HI_IO_PULL_UP);             // 设置GPIO上拉
    hi_io_set_func(IN4_PIN, IN4_GPIO_FUN);              // 设置IO为GPIO功能
    hi_gpio_set_dir(IN4_PIN, HI_GPIO_DIR_OUT);          // 设置GPIO为输出模式
}

/*******************************************************************************
* 函 数 名       : step_motor_run
* 函数功能		 : 输出一个数据给ULN2003从而实现向步进电机发送一个脉冲
	    		   步进角为5.625/64度，如果需要转动一圈，那么需要360/5.625*64=4096个脉冲信号，
				   假如需要转动90度，需要的脉冲数=90*4096/360=1024脉冲信号。
				   如果任意角度对应多少脉冲呢？脉冲=angle*512/(8*5.625)
				   正好对应下面计算公式
				   (512*angle/45)*8，这个angle就是转动90度，45表示5.625*8，8代表8个脉冲，for循环。
* 输    入       : step：指定步进控制节拍，可选值4或8
				   dir：方向选择,1：顺时针,0：逆时针
				   speed：速度1-5
				   angle：角度选择0-360
				   sta：电机运行状态，1启动，0停止
* 输    出    	 : 无
*******************************************************************************/
void step_motor_run(uint8_t step, uint8_t dir, uint8_t speed, uint16_t angle, uint8_t sta)
{
    if (sta == 1) {
        uint16_t total_steps = (512 * angle) / 45;
        for (uint16_t j = 0; j < total_steps; j++) {
            for (uint8_t i = 0; i < 8; i += (8 / step)) {
                uint8_t index = dir == 0 ? i : (8 - i) % 8; // 方向选择
                switch (index) {
                    case 0: MOTOR_IN1(1); MOTOR_IN2(0); MOTOR_IN3(0); MOTOR_IN4(0); break;
                    case 1: MOTOR_IN1(1); MOTOR_IN2(1); MOTOR_IN3(0); MOTOR_IN4(0); break;
                    case 2: MOTOR_IN1(0); MOTOR_IN2(1); MOTOR_IN3(0); MOTOR_IN4(0); break;
                    case 3: MOTOR_IN1(0); MOTOR_IN2(1); MOTOR_IN3(1); MOTOR_IN4(0); break;
                    case 4: MOTOR_IN1(0); MOTOR_IN2(0); MOTOR_IN3(1); MOTOR_IN4(0); break;
                    case 5: MOTOR_IN1(0); MOTOR_IN2(0); MOTOR_IN3(1); MOTOR_IN4(1); break;
                    case 6: MOTOR_IN1(0); MOTOR_IN2(0); MOTOR_IN3(0); MOTOR_IN4(1); break;
                    case 7: MOTOR_IN1(1); MOTOR_IN2(0); MOTOR_IN3(0); MOTOR_IN4(1); break;
                }
				//驱动要求大于1.8ms = 1800us 但是实测延时1000us也可以
                usleep(speed * 1000);
            }
        }
    } else {
        MOTOR_IN1(0); MOTOR_IN2(0); MOTOR_IN3(0); MOTOR_IN4(0);
    }
}

void curtain_open(void)
{
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_CLOCKWISE, STEP_MOTOR_SPEEP, STEP_MOTOR_ANGLE, STEP_MOTOR_START);
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_CLOCKWISE, STEP_MOTOR_SPEEP, STEP_MOTOR_ANGLE, STEP_MOTOR_STOP);
}

