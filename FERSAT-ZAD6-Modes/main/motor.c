#include <Arduino.h> /* for OUTPUT definitions */
#include "motor.h"


inline void MOTOR_init(void) {
  // Initialize pins for motor control
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_DIRECTION_PIN, OUTPUT);

  // Turn off motor
  analogWrite(MOTOR_PIN, 0u);

  // Set motor direction to default
  digitalWrite(MOTOR_DIRECTION_PIN, 0u);
}


void MOTOR_setPwm(int pwm) {
  if (pwm < 0) {
    digitalWrite(MOTOR_DIRECTION_PIN, MOTOR_DIRECTION_CW);
  } else {
    digitalWrite(MOTOR_DIRECTION_PIN, MOTOR_DIRECTION_ACW);
  }

  if (pwm < 0) {
    pwm = -pwm;
  }

  if (pwm > MOTOR_MAX_PWM) {
    pwm = MOTOR_MAX_PWM;
  }

  analogWrite(MOTOR_PIN, pwm);
}
