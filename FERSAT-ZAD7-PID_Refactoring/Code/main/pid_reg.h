/**
 * file: pid.h
 * 
 * Author: Ivan Vnucec, https://github.com/IvanVnucec
 * 
 * Date DD/MM/YYYY: 15/02/2020
 * 
 * License: MIT License
 * 
 */

#ifndef PID_H__
#define PID_H__

#ifdef __cplusplus
extern "C" {
#endif

struct PID_Handle_s {
	/* Regulator coefficients 	*/
	float Kp;	/* Proportional */
	float Ki;	/* Integral 	*/
	float Kd;	/* Derivative 	*/
	float Ts;	/* Sample time 	*/
  float V;  /* Real derivative term */

	float up0;  /* output of P term up(k) */
  float ui0;  /* output of I term ui(k) */
  float ui1;  /* output of I term ui(k-1) */
  float ud0;  /* output of D term ud(k) */
  float ud1;  /* output of D term ud(k-1) */
  float u0;   /* output of PID regulator u(k) */

  float e0;
  float e1;
};

typedef struct PID_Handle_s PID_Handle_t;

void  PID_Init		(PID_Handle_t *hpid, float Kp, float Ki, float Kd, float V, float Ts);
float PID_Regulate	(PID_Handle_t *hpid, float error);
void PID_ResetIntegral(PID_Handle_t *hpid);

#ifdef __cplusplus
}
#endif

#endif /* PID_H__ */
