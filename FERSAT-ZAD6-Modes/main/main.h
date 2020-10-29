#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>


enum regulationMode_E {
  REGULATION_MODE_NONE,
  REGULATION_MODE_OMEGA,
  REGULATION_MODE_ANGLE
};


void USER_CMD_motorPwm(int pwm);
void USER_CMD_regWz(float wz);
void USER_CMD_regAngle(float angle);
void USER_CMD_regNone(void);


#endif // MAIN_H_INCLUDED
