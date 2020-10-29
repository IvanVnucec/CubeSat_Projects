#ifndef MOTOR_H__
#define MOTOR_H__

#define MOTOR_PIN 9
#define MOTOR_DIRECTION_PIN 8

#define MOTOR_DIRECTION_CW 0
#define MOTOR_DIRECTION_ACW 1

#define MOTOR_MAX_PWM 255


inline void MOTOR_init(void);
void MOTOR_setPwm(int pwm);


#endif /* MOTOR_H__*/
