#include"main.h"
#include"MPU9250.h"

#include <avr/interrupt.h>

#include <stdlib.h>

/**************************************************************************************
                                 GLOBALNE VARIJABLE
***************************************************************************************/
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

MPU9250 IMU(Wire, 0x68);

const int direction_pin = 8;
const int motor_pin = 9;

volatile float wx = 0.0;
volatile float wy = 0.0;
volatile float wz = 0.0;

volatile float ax = 0.0;
volatile float ay = 0.0;
volatile float az = 0.0;

volatile float hx = 0.0;
volatile float hy = 0.0;
volatile float hz = 0.0;

/**************************************************************************************
                                      SETUP
***************************************************************************************/
void setup() {
  // wait period for stabilization of CubeSat
  delay(5000);

  // initialize serial:
  Serial.begin(115200);

  // start communication with IMU
  IMU.begin();

  IMU.calibrateGyro();

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode(motor_pin,     OUTPUT);
  pinMode(direction_pin, OUTPUT);

  TIMER1_Init();
}



/**************************************************************************************
                                      LOOP
***************************************************************************************/
void loop() {


  check_for_new_command();
  
  IMU.readSensor();
  
  IMU_print_gyro();

  // da usporimo ispisivanje podataka. Ovo ne mora biti.
  delay(100);
}


// Funkcija koja mijenja periodicnost TIMER1
// Pozivamo ju preko parsera na nacin da napisemo setTimer(00054) koji kaze rimeru
// da generira interrupt svakih 54 Hz.
void setTimer(uint16_t timer) {
  if (timer < 15625 && timer != 0) {
    cli(); // onemoguci prekide
    OCR1A = (uint16_t)((double)15625 / timer);
    sei(); // omoguci prekide
  }
}


// Funkcija koja okrece smjer motora
// Pozivamo ju na nacin setDirection(0) ili setDirection(1)
void setDirection(const bool dir) {
  digitalWrite(direction_pin, dir);

  return;
}


// Funkcija koja utjece na brzinu vrtnje motora
// Pozivamo ju preko parsera npr setPWM(22)
void setPWM(const uint8_t pwm) {
  analogWrite(motor_pin, pwm);

  return;
}


void TIMER1_Init (void) {
  cli();

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 10hz increments
  OCR1A = 1562;// = (16*10^6) / (1*1024) - 1 (must be <65536)

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();
}


// Slanje podataka preko UARTA kako bi se ispisali u SerialPlot programu
// Mozda bi se ovo trebalo odvijati sa Atomic zastitom
void IMU_print_gyro (void) {
  wx = IMU.getGyroX_rads();
  wy = IMU.getGyroY_rads();
  wz = IMU.getGyroZ_rads();

  ax = IMU.getAccelX_mss();
  ay = IMU.getAccelY_mss();
  az = IMU.getAccelZ_mss();

  hx = IMU.getMagX_uT();
  hy = IMU.getMagY_uT();
  hz = IMU.getMagZ_uT();

  Serial.print(wx, 6);
  Serial.print(",");
  Serial.print(wy, 6);
  Serial.print(",");
  Serial.print(wz, 6);
  Serial.print(",");

  Serial.print(ax, 6);
  Serial.print(",");
  Serial.print(ay, 6);
  Serial.print(",");
  Serial.print(az, 6);
  Serial.print(",");

  Serial.print(hx, 6);
  Serial.print(",");
  Serial.print(hy, 6);
  Serial.print(",");
  Serial.println(hz, 6);

}


// Funkcija koja provjerava jeli dosla naredba preko Parsera
// ako je onda se poziva parser koji taj string obradjuje i poziva
// odgovarajucu funkciju
void check_for_new_command (void) {
  if (stringComplete == true) {
    // convert String to C-type string with '\0'
    const char *inputString_c = inputString.c_str();

    parse(inputString_c);
    inputString = ""; // clear the string:

    stringComplete = false;
  }
}


// Ovdje se odlazi kada dode novi char preko UARTa
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      inChar = '\0';
      stringComplete = true;
    }
    // add it to the inputString:
    inputString += inChar;
  }
}


// ovdje ulazimo svaki puta kada TIMER1 dosegne OCR1A vrijednost
ISR(TIMER1_COMPA_vect) {
  sei();  // ovo mora biti jer u protivnom se desava neki nepoznati bug.

  IMU.readSensor();

  // Ovdje idu algoritmi

}
