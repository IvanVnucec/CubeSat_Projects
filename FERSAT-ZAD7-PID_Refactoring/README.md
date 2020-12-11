# FERSAT-ZAD7-PID_Refactoring
## About
The project is dealing with the implementation of the low pass filter into Derivative term to eliminate 
its very agressive output in angle regulation.  

## Changes
The following changes were introduced:
 - Now the PID regulator has V term to introduce a low pass filter. The term tipicaly ranges from 5 to 20. If user wants to have 
 so called ideal derivative term than V term should be set to more than 20.
 - Because we had good behaviour with the PID regulator in angle regulation, its V term is set to 100. This should not introduce any different behaviour in comparation to the last project because the D term is set to 0.
 - V term in angle PID regulator is set to 10 but this can be changed to any value in `reg_angle.h` file under `#define REG_ANGLE_PID_COEFF_V  10.0f`. Lower value means lower low-pass crossover frequency.
 - We have updated simulink model with new the new PID (V = 10).

## Note
All the equations regarding new form of PID regulator can be found in `documentation.pdf` file on pages 159, 160, 199-208.   
System has the same so-called parser functions as last project. They are written below.

## Functions
User can send functions to a sattelite over serial port by typing:
- **motorPwm(value)**
	- This function takes integer [-255, 255] representing a duty cycle in both directions. 0 means 0% PWM duty cycle, 255 means 100% PWM duty cycle. Negative number means that the motor will rotate in opposite direction. Also, function will disable any ongoing regulation.
	- example: `motorPwm(100)`, `motorPwm(-255)`
	
- **regWz(value)**
	- This function takes float value representing a desired angular velocity in radians per second. Function initiates PID regulation and disables any other ongoing regulation.
	- example: `regWz(1.0)`, `regWz(-0.1)`
	
- **regAngle(value)**
	- This function takes float value representing a desired angle rotation in degrees relative to the angle when function was called. We are regulating relative angle, not absolute. Function initiates regulation and disables any other ongoing regulation.
	- example: `regAngle(20.5)`, `regAngle(-10.1)`
	
- **regNone()**
	- This function disables any ongoing regulation.
	- example: `regNone()`
