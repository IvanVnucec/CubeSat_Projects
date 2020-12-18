#ifndef REG_WZ_H__
#define REG_WZ_H__


#include "pid_reg.h"
#include "motor.h"


#define REG_WZ_PID_COEFF_P  231.5f
#define REG_WZ_PID_COEFF_I  117.0f
#define REG_WZ_PID_COEFF_D  0.0f
#define REG_WZ_PID_COEFF_V  15.0f
#define REG_WZ_PID_PERIOD_S 0.1f
#define REG_WZ_PID_PERIOD_MS (REG_WZ_PID_PERIOD_S * 1000.0f)


void REG_WZ_init(void);
void REG_WZ_startRegulation(void);
void REG_WZ_stopRegulation(void);
void REG_WZ_pauseRegulation(void);
float REG_WZ_regulate(float desired_wz, float real_wz);


#endif /* REG_WZ_H__ */
