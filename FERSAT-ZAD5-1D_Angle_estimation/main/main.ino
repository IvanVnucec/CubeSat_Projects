#include "main.h"
#include "MPU9250.h"
#include "parser.h"
#include <SoftwareSerial.h>
#include "pid_reg.h"
#include "kalman_1d.h"

#include <stdlib.h>

#define MAX_INPUT_STRING_BYTES 50

/**************************************************************************************
                                 GLOBALNE VARIJABLE
***************************************************************************************/
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

MPU9250 IMU(Wire, 0x68);

const int direction_pin = 8;
const int motor_pin = 9;

float wx = 0.0;
float wy = 0.0;
float wz = 0.0;
float hx;
float hy;

float wz_desired_value = 0.0;
float error;
float output;

bool smjer_vrtnje = false;

float KP = 231.5f;   // This value was obtained based on the step response of the system
float KI = 117.0f;   // This value was obtained based on the step response of the system
float KD = 0.0f;     // This value was obtained based on the step response of the system
float iteration_time = 0.1;  // seconds

bool flag_regulation = false; // wether we use PID regulation, disabled by deafult

// PID regulator handle
PID_Handle_t hpid;

// Kalman filter handle
Kalman1D_Handle_t hkf;

float dx;               /* Start value of the Transition variable */
float z;                /* Measurment */
float filter_output;    /* Filtered State variable */
float dv;               /* dv measurment */
float dt;

float x = 0.0f;         /* State variable */
float P = 100.0f;       /* Start value of the State variable variance */
float Q = 0.1f;         /* Transition function variance */
float R = 0.1f;         /* Measurment variance */

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

  // reserve 200 bytes for the inputString of parser
  inputString.reserve(MAX_INPUT_STRING_BYTES);

  // Initialize pins for motor control
  pinMode(motor_pin, OUTPUT);
  pinMode(direction_pin, OUTPUT);

  // Initialize PID regulator
  PID_Init(&hpid, KP, KI, KD, iteration_time);

  // Initialize Kalman filter
  K1D_Init(&hkf, x, P);

}



/**************************************************************************************
                                      LOOP
***************************************************************************************/
void loop() {
  // Parser
  check_for_new_command();

  // Read IMU registers
  IMU.readSensor();
  // wx = IMU.getGyroX_rads();
  // wy = IMU.getGyroY_rads();
  wz = IMU.getGyroZ_rads();

  hx = IMU.getMagX_uT();
  hy = IMU.getMagY_uT();
  float angle = atan2f(hy, hx);
  // we want angle from 0 to Pi
  angle = angle + 3.14159 / 2;
  
  x = angle;
  dv = wz;
  dt = iteration_time;
  
  dx = dv * dt;
  K1D_Predict(&hkf, dx, Q);

  z = angle;
  K1D_Update(&hkf, z, R);

  filter_output = K1D_Output(&hkf);

  // ========= PID control of Z-axis =================
  if (flag_regulation == true) {
    error = wz_desired_value - wz; // odstupanje od zeljene vrijednosti

    output = PID_Regulate(&hpid, error);

    // Ogranici vrijednost izlaza zato sto vrijednosti
    // za upravljanjem motorom idu od [-255, 255]
    if (output < -255.0) {
      output = -255.0;
    } else if (output > 255.0) {
      output = 255.0;
    }

    // Ako je Output negativan onda to znaci da se smjer vrtnje mora obrnuti.
    // Ovdje se mozda mora obrnut true i false ako motor ide u suprotnu stranu
    // nego sto bi trebao.
    if (output < 0) {
      smjer_vrtnje = false;
    } else {
      smjer_vrtnje = true;
    }

    // Djeluj na motor
    setDirection(smjer_vrtnje);     // Namjesti smjer vrtnje
    setPWM((uint8_t)fabs(output));  // Namjesti brzinu vrtnje
  }

  // Delay for iteration_time in mili seconds
  delay(iteration_time * 1000);

  // Posalji vrijednosti na racunalo
  // IMU_print_gyro();
  
  // Posalji vrijednosti vazne za PID regulaciju
  Serial.print(wz);
  Serial.print(",");
  Serial.print(wz_desired_value);
  /*
  Serial.print(",");
  Serial.print(error);
  Serial.print(",");
  Serial.println(output);
  */
}


// za debuging
void debug(float a) {
  Serial.println("DEBUG");
  Serial.println(a);
  Serial.println("DEBUG");
}


// namjestava zeljenu kutnu brzinu za regulator
// Parser funkcija. Poziva se kao: setWz(+321.1234)
void setWz(float val) {
  wz_desired_value = val;

}


// Pali i gasi PID regulacju
// Parser funkcija. Poziva se kao setRegulation(0) ili setRegulation(1)
void setRegulation(bool val) {
  flag_regulation = val;
  /* Ugasi motor */
  if (val == false) {
    setPWM(0);
  }
}


// Funkcije za promjenu parametara regulatora
// svaki puta nakon promjene parametara resetira se regulator
// Parser funkcija. Poziva se kao: updateKp(+321.1234)
void updateKp(float val) {
  KP = val;

}

// Parser funkcija. Poziva se kao: updateKi(+321.1234)
void updateKi(float val) {
  KI = val;

}

// Parser funkcija. Poziva se kao: updateKd(+321.1234)
void updateKd(float val) {
  KD = val;

}



// Mijenja smjer vrtnje motora
// Parser funkcija. Poziva se kao: setDirection(0)
void setDirection(const bool dir) {
  smjer_vrtnje = dir;
  
  digitalWrite(direction_pin, smjer_vrtnje);

  return;
}


// Mijenja brzinu vrtnje motora
// Parser funkcija. Poziva se kao: setPWM(321)
void setPWM(const uint8_t pwm) {
  analogWrite(motor_pin, pwm);

  return;
}


// Salje vrijednosti kutnih brzina na racunalo
// Stavljeno je da salje samo kutnu brzinu u z smjeru radi 
// brze petlje
void IMU_print_gyro (void) {
  
  Serial.print(wx);
  Serial.print(",");
  Serial.print(wy);
  Serial.print(",");
  Serial.println(wz);

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
