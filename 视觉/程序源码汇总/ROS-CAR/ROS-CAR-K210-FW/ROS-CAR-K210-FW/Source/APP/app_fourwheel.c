#include "app_fourwheel.h"


#include "app_mecanum.h"
#include "app_motion.h"
#include "app_bat.h"
#include "app_pid.h"

#include "app.h"

#include "stdint.h"

#include "bsp_usart.h"
#include "bsp_motor.h"
#include "bsp_common.h"


static float speed_fb = 0;
static float speed_spin = 0;

static int speed_L1_setup = 0;
static int speed_L2_setup = 0;
static int speed_R1_setup = 0;
static int speed_R2_setup = 0;




// X轴速度(前正后负：±1000)，Y轴速度(0)，旋转速度(左正右负：±5000)
void Fourwheel_Ctrl(int16_t V_x, int16_t V_y, int16_t V_z, uint8_t adjust)
{
    float robot_APB = Motion_Get_APB();
    speed_fb = V_x;
    V_y = 0;
    speed_spin = (V_z / 1000.0f) * robot_APB;
    if (V_x == 0 && V_y == 0 && V_z == 0)
    {
        Motion_Stop(STOP_BRAKE);
        return;
    }

    speed_L1_setup = speed_fb - speed_spin;
    speed_L2_setup = speed_fb - speed_spin;
    speed_R1_setup = speed_fb + speed_spin;
    speed_R2_setup = speed_fb + speed_spin;

    if (speed_L1_setup > 1000) speed_L1_setup = 1000;
    if (speed_L1_setup < -1000) speed_L1_setup = -1000;
    if (speed_L2_setup > 1000) speed_L2_setup = 1000;
    if (speed_L2_setup < -1000) speed_L2_setup = -1000;
    if (speed_R1_setup > 1000) speed_R1_setup = 1000;
    if (speed_R1_setup < -1000) speed_R1_setup = -1000;
    if (speed_R2_setup > 1000) speed_R2_setup = 1000;
    if (speed_R2_setup < -1000) speed_R2_setup = -1000;
    
    Motion_Set_Speed(speed_L1_setup, speed_L2_setup, speed_R1_setup, speed_R2_setup);
}




// 控制四轮小车运动状态。 
// 速度控制：speed=0~1000。
// 偏航角调节运动：adjust=1开启，=0不开启。
void Fourwheel_State(uint8_t state, uint16_t speed, uint8_t adjust)
{
    switch (state)
    {
    case MOTION_STOP:
        Motion_Stop(speed==0?STOP_FREE:STOP_BRAKE);
        break;
    case MOTION_RUN:
        Fourwheel_Ctrl(speed, 0, 0, adjust);
        break;
    case MOTION_BACK:
        Fourwheel_Ctrl(-speed, 0, 0, adjust);
        break;
    case MOTION_LEFT:
        Fourwheel_Ctrl(speed/2, 0, speed*2, adjust);
        break;
    case MOTION_RIGHT:
        Fourwheel_Ctrl(speed/2, 0, -speed*2, adjust);
        break;
    case MOTION_SPIN_LEFT:
        Fourwheel_Ctrl(0, 0, speed*5, 0);
        break;
    case MOTION_SPIN_RIGHT:
        Fourwheel_Ctrl(0, 0, -speed*5, 0);
        break;
    case MOTION_BRAKE:
        Motion_Stop(STOP_BRAKE);
        break;
    default:
        break;
    }
}


