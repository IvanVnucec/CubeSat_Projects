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
 * PID_Handle_t *hpid - PID regulator handle. Regulator has Parrarel structure. 
 *    See documentation folder for more info.
 * float Kp - Proportional coefficient
 * float Ki - Integral coefficient
 * float Kd - Derivative coefficient
 * float V  - Low pass coefficint in derivative term (typ. 5 to 20).
 *    Higher the value, lower the cutt-off frequency.
 * float Ts - Sample time in seconds
 * 
 * This function can be called when the User wants to change some coefficients.
 * But the function will also reset the Integral part of the controller to 
 * zero.
 * 
 * */
void PID_Init(PID_Handle_t *hpid, float Kp, float Ki, float Kd, float V, float Ts) {
    if (hpid == NULL) {
        return; /* failure */
    }

    if (Ts <= 0.0f || V <= 0.0f) {
        return; /* Failure */
    }

    /* Set coefficients */
  	hpid->Kp = Kp;
  	hpid->Ki = Ki;
  	hpid->Kd = Kd;
  	hpid->Ts = Ts;
    hpid->V  = V;
  
    hpid->up0 = 0.0f;  
    hpid->ui0 = 0.0f;  
    hpid->ui1 = 0.0f;  
    hpid->ud0 = 0.0f;  
    hpid->ud1 = 0.0f;  
    hpid->u0  = 0.0f;   
    hpid->e0  = 0.0f;
    hpid->e1  = 0.0f;
  
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
    /* transform PID coeff from Parralel to Standard form. See documentation */
    static const float Kr = hpid->Kp;
    static const float Ti = Kr / hpid->Ki;
    static const float Td = hpid->Kd / Kr;
    static const float T  = hpid->Ts;
    static const float v  = hpid->V;

    /* e(k) = error */
    hpid->e0 = error;

    /* up(k)  = Kr *   e(k) */
    hpid->up0 = Kr * hpid->e0;

    /* ui(k)  = ui(k-1)   + Kr * T / Ti * (  e(k)   + e(k-1)  ) / 2.0f */
    hpid->ui0 = hpid->ui1 + Kr * T / Ti * (hpid->e0 + hpid->e1) / 2.0f;

    /* ud(k)  = Td / (Td + v * T) * ud(k-1)   + Kr * v * Td / (Td + v * T) * (  e(k)   - e(k-1)  ) */
    hpid->ud0 = Td / (Td + v * T) * hpid->ud1 + Kr * v * Td / (Td + v * T) * (hpid->e0 - hpid->e1);

    /* u(k)  =    up(k)  +    ui(k)  +    ud(k) */
    hpid->u0 = hpid->up0 + hpid->ui0 + hpid->ud0; 

    /* u(k-1) = u(k) */
    /* e(k-1) = e(k) */
    hpid->ui1 = hpid->ui0;
    hpid->ud1 = hpid->ud0;
    hpid->e1  = hpid->e0;

    /* Return regulator output value */
    return hpid->u0;    
}


void PID_ResetIntegral(PID_Handle_t *hpid) {
    hpid->ui1 = 0.0f;
}
