#ifndef WRAPPER_H_INCLUDED
#define WRAPPER_H_INCLUDED

#include"function.h"

int function_wrapper_init (function_list_t *function_list);

void motor_wrapp  (list_t *arg);
void stop_wrapp (list_t *arg);

#endif // WRAPPER_H_INCLUDED
