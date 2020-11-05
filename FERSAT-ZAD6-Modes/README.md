# FERSAT-ZAD6-Modes
This code revision has implemented regulation of angular velocity over Z axis and regulation of relative angle displacment (still work in progress). User can send commands to the sattelite over COM port (see listed functions below).

## Functions
User can send functions to a sattelite over serial port by typing:
- **motorPwm(value)**
	- This function takes integer [-255, 255] representing a duty cycle in both directions. 0 means 0% PWM duty cycle, 255 means 100% PWM duty cycle. Negative number means that the motor will rotate in opposite direction. Also, function will disable ongoing regulation.
	- example: `motorPwm(100)`, `motorPwm(-255)`
	
- **regWz(value)**
	- This function takes float value representing a desired angular velocity in radians per second. Function initiates PID regulation and disables any other ongoing regulation.
	- example: `regWz(1.0)`, `regWz(-0.1)`
	
- **regAngle(value)**
	- This function takes float value representing a desired angle rotation in degrees. Function initiates regulation and disables any other ongoing regulation.
	- example: `regAngle(20.5)`, `regAngle(-10.1)`
	
- **regNone()**
	- This function disables any ongoing regulation.
	- example: `regNone()`
