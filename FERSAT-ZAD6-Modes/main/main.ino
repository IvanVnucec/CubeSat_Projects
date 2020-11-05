#include "main.h"
#include "MPU9250.h"
#include "parser.h"
#include <SoftwareSerial.h>
#include "reg_wz.h"

#include <stdlib.h>

#define MAX_INPUT_STRING_BYTES 50

/**************************************************************************************
                                 GLOBALNE VARIJABLE
***************************************************************************************/
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

MPU9250 IMU(Wire, 0x68);

enum regulationMode_E regulationMode = REGULATION_MODE_NONE;

float wz_desired = 0.0f;

/**************************************************************************************
                                      SETUP
***************************************************************************************/
void setup() {
  // wait period for stabilization of CubeSat
  delay(5000);

  // initialize serial:
  Serial.begin(115200);

  // Initialize IMU
  IMU.begin();
  IMU.calibrateGyro();

  /* Wz regulator init */
  REG_WZ_init();

  // reserve 200 bytes for the inputString of parser
  inputString.reserve(MAX_INPUT_STRING_BYTES);
}



/**************************************************************************************
                                      LOOP
***************************************************************************************/
void loop() {
  float wz_real;
  
  // Parser
  check_for_new_command();

  switch(regulationMode) {
    case REGULATION_MODE_NONE :
      break;
      
    case REGULATION_MODE_OMEGA :
      IMU.readSensor();
      wz_real = IMU.getGyroZ_rads();
      
      REG_WZ_regulate(wz_desired, wz_real);
      
      delay(REG_WZ_PID_PERIOD_MS);
      break;
      
    case REGULATION_MODE_ANGLE :  
      break;
    
    default :
      // Invalid state
      break;
   }
}


void USER_CMD_regNone(void) {
  regulationMode = REGULATION_MODE_NONE;

  REG_WZ_pause();
}


void USER_CMD_motorPwm(int pwm) {
  regulationMode = REGULATION_MODE_NONE;

  REG_WZ_pause();
  
  MOTOR_setPwm(pwm);
}


void USER_CMD_regWz(float wz) {
  regulationMode = REGULATION_MODE_OMEGA;
  wz_desired = wz;

  REG_WZ_startRegulation();
}


void USER_CMD_regAngle(float a) {
  regulationMode = REGULATION_MODE_ANGLE;

  REG_WZ_pause();
}


// Funkcija za parser koja provjerava jeli stigla nova naredba
// za parser
void check_for_new_command (void) {
  if (stringComplete == true) {
    // convert String to array of chars
    const char *inputString_c = inputString.c_str();

    PSR_Parse_command(inputString_c, inputString.length());
    
    inputString = ""; // clear the string:

    stringComplete = false;
  }
}


// Prekidna rutina za UART komunikaciju
void serialEvent() {
  while (stringComplete == false && Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    // add it to the inputString:
    inputString += inChar;
  }
}