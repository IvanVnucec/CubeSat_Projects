#ifndef REG_ANGLE_H__
#define REG_ANGLE_H__


#include "pid_reg.h"
#include "motor.h"


#define REG_ANGLE_PID_COEFF_P  73.6f
#define REG_ANGLE_PID_COEFF_I  6.3f
#define REG_ANGLE_PID_COEFF_D  74.4f
#define REG_ANGLE_PID_PERIOD_S 0.1f
#define REG_ANGLE_PID_PERIOD_MS (REG_ANGLE_PID_PERIOD_S * 1000.0f)


void REG_ANGLE_init(void);
void REG_ANGLE_startRegulation(void);
void REG_ANGLE_stopRegulation(void);
void REG_ANGLE_pauseRegulation(void);
void REG_ANGLE_regulate(float desired_angle, float real_angle);


#endif /* REG_ANGLE_H__ */
