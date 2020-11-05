#include "reg_wz.h"


static PID_Handle_t hpid;
static int regulation_running = 0;


void REG_WZ_init(void) {
  PID_Init(&hpid, 
    REG_WZ_PID_COEFF_P, 
    REG_WZ_PID_COEFF_I, 
    REG_WZ_PID_COEFF_D, 
    REG_WZ_PID_PERIOD_S);

  MOTOR_init();

  regulation_running = 0;
}


void REG_WZ_startRegulation(void) {
  regulation_running = 1;
}


void REG_WZ_stopRegulation(void) {
  regulation_running = 0;
  PID_ResetIntegral(&hpid);
  MOTOR_setPwm(0);
}


void REG_WZ_pause(void) {
  regulation_running = 0;
}


void REG_WZ_regulate(float desired_wz, float real_wz) {
  float error;
  float output;
  
  if (regulation_running == 1) {
    error = desired_wz - real_wz;  
    output = PID_Regulate(&hpid, error);
    MOTOR_setPwm(output);
  }
}
