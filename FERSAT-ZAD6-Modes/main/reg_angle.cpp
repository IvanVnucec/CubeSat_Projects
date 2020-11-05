#include "reg_angle.h"


static PID_Handle_t hpid;
static int regulation_running = 0;


void REG_ANGLE_init(void) {
  PID_Init(&hpid, 
    REG_ANGLE_PID_COEFF_P, 
    REG_ANGLE_PID_COEFF_I, 
    REG_ANGLE_PID_COEFF_D, 
    REG_ANGLE_PID_PERIOD_S);

  MOTOR_init();

  regulation_running = 0;
}


void REG_ANGLE_startRegulation(void) {
  regulation_running = 1;
}


void REG_ANGLE_stopRegulation(void) {
  regulation_running = 0;
  PID_ResetIntegral(&hpid);
  MOTOR_setPwm(0);
}


void REG_ANGLE_pauseRegulation(void) {
  regulation_running = 0;
}


void REG_ANGLE_regulate(float desired_angle, float real_angle) {
  float error;
  float output;
  
  if (regulation_running == 1) {
    error = desired_angle - real_angle;  
    output = PID_Regulate(&hpid, error);
    MOTOR_setPwm(output);
  }
}
