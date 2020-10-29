/**
 * file: pid.c
 * 
 * Author: Ivan Vnucec, https://github.com/IvanVnucec
 * 
 * Date DD/MM/YYYY: 15/02/2020
 * 
 * License: MIT License
 * 
 */

#include <stddef.h> /* for NULL definition */

#include "pid_reg.h"

/**
 * Initialize PID regulator
 * 
 * PID_Handle_t *hpid - PID regulator handle
 * float Kp - Proportional coefficient
 * float Ki - Integral coefficient
 * float Kd - Derivative coefficient
 * float Ts - Sample time in seconds
 * 
 * This function can be called when the User wants to change some coefficients.
 * But the function will also reset the Integral part of the controller to 
 * zero.
 * 
 * */
void PID_Init(PID_Handle_t *hpid, float Kp, float Ki, float Kd, float Ts) {
    if (hpid == NULL) {
        return; /* failure */
    }

    if (Ts <= 0.0f) {
        return; /* Failure */
    }

    /* Set coefficients */
	hpid->Kp = Kp;
	hpid->Ki = Ki;
	hpid->Kd = Kd;
	hpid->Ts = Ts;

    /* Set starting input and output values */
	hpid->e0 = 0.0f;
	hpid->e1 = 0.0f;
	hpid->u0 = 0.0f;
    hpid->integral = 0.0f;

    return; /* Success */
}


/**
 * Function for the PID regulator output.
 * 
 * PID_Handle_t *hpid - PID regulator handle
 * float error - defined as Set_point_value minus Measured_value
 * 
 * return float - Output of the PID regulator
 * 
 * */
float PID_Regulate(PID_Handle_t *hpid, float error) {
    float derivative;

    /* error := setpoint − measured_value */
    hpid->e0 = error;
    /* integral := integral + error * dt */
    hpid->integral = hpid->integral + hpid->e0 * hpid->Ts;
    /* derivative := (error − previous_error) / dt */
    derivative = (hpid->e0 - hpid->e1) / hpid->Ts;
    /* output := Kp × error + Ki * integral + Kd * derivative */
    hpid->u0 =  hpid->Kp * hpid->e0 + 
                hpid->Ki * hpid->integral + 
                hpid->Kd * derivative;
    /* previous_error := error */
    hpid->e1 = hpid->e0;

    return hpid->u0;    /* Return regulator output value */
}


void PID_ResetIntegral(PID_Handle_t *hpid) {
  hpid->integral = 0.0f;
}
