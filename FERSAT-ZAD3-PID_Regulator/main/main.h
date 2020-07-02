#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

void debug(float a);

void setWz(float val);
void setRegulation(bool val);
void updateKp(float val);
void updateKi(float val);
void updateKd(float val);
void setDirection(bool dir);
void setPWM(uint8_t pwm);

#endif // MAIN_H_INCLUDED
