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

	/* Output values */
	float u0;	/* u(k) */

	/* Input values */
	float e0;	/* e(k)	  */
	float e1;	/* e(k-1) */

    /* Integral part */
    float integral;
};

typedef struct PID_Handle_s PID_Handle_t;

void  PID_Init		(PID_Handle_t *hpid, float Kp, float Ki, float Kd, float Ts);
float PID_Regulate	(PID_Handle_t *hpid, float error);
void PID_ResetIntegral(PID_Handle_t *hpid);

#ifdef __cplusplus
}
#endif

#endif /* PID_H__ */
