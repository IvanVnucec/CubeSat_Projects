#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include<stdio.h>   // this library is for testing purposes only
#include<string.h>  // this too

#include"parser.h"
#include"function.h"
#include"list.h"
#include"wrapper.h"

void stop(void);
void motor(int state);
void print_w(int sec);

void setDirection(bool dir);
void setPWM(uint8_t pwm);
void setTimer(uint16_t timer);

#endif // MAIN_H_INCLUDED
