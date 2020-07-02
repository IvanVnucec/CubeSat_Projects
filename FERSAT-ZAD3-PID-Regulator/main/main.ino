#include "main.h"
#include "MPU9250.h"
#include "parser.h"
#include <SoftwareSerial.h>

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
float wz_desired_value = 0.0;

bool smjer_vrtnje = false;

float desired_value = 0.0;
float actual_value = 0.0;
float error = 0.0;
float error_prior = 0.0;
float integral = 0.0;
float integral_prior = 0.0;
float derivative = 0.0;
float output = 0.0;

float bias = 0.0; // (see below)
float KP = 231.5f;   // This value was obtained based on the step response of the system
float KI = 117.0f;   // This value was obtained based on the step response of the system
float KD = 0.0f;     // This value was obtained based on the step response of the system
float iteration_time = 0.1;  // seconds

bool flag_regulation = false; // wether we use PID regulation, disabled by deafult


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

  // ========= PID control of Z-axis =================
  if (flag_regulation == true) {
    actual_value = wz;                    // mjerena vrijednost
    desired_value = wz_desired_value;     // zeljena vrijednost

    error = desired_value - actual_value; // odstupanje od zeljene vrijednosti

    // Izracun I i D clana (K clan je vec u formuli dolje)
    integral = integral_prior + error * iteration_time; // mozda se integralni dio mora ograniciti
    derivative = (error - error_prior) / iteration_time;

    // Izracunaj izlaz iz regulatora
    output = KP * error + KI * integral + KD * derivative;

    // Spremi vrijednosi D i I clana
    error_prior = error;
    integral_prior = integral;

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
  Serial.print(",");
  Serial.print(error);
  Serial.print(",");
  Serial.println(output);
}


// Ponisti akumulacijsko djelovanje I clana
void resetRegulator(void) {
  integral = 0.0f;
  integral_prior = 0.0f;

  return;
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

  resetRegulator();
}

// Parser funkcija. Poziva se kao: updateKi(+321.1234)
void updateKi(float val) {
  KI = val;

  resetRegulator();
}

// Parser funkcija. Poziva se kao: updateKd(+321.1234)
void updateKd(float val) {
  KD = val;

  resetRegulator();
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
