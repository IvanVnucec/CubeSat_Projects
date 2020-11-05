# FERSAT-ZAD6-Modes
Implementing few functions for mode selection.

## Functions
User can send functions to a sattelite over serial port by typing:
- **motorPwm(value)**
	- This function takes integer [-255, 255] representing a duty cycle in both directions. 0 means 0% PWM duty cycle, 255 means 100% PWM duty cycle. Negative number means that the motor will rotate in opposite direction. Also, function will disable ongoing regulation.
	
- **regWz(value)**
	- This function takes float value representing a desired angular velocity. Function initiates PID regulation and disables any other ongoing regulation.
	
- **regAngle(value)**
	- This function takes float value representing a desired angle rotation. Function initiates regulation and disables any other ongoing regulation.
	
- **regNone()**
	- This function disables any ongoing regulation.
