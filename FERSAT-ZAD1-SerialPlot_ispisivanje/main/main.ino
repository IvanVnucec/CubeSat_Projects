/**********************************************************************************/
/*  FERSAT PARSER
     Ovo je primjer kako ce izgledat interface parsera
     Da bi parser funkcionirao potrebno je uciniti par stvari:
      Napisati tzv. Wrapper funkcije (vidi ispod) i
      Stvoriti listu funkcija koja sluzi kao svojevrsna baza podataka funkcija koje
      se zele pozivati preko parsera.
*/


#include"main.h"


#define MOTOR_PIN 13

#define MAX_INPUT_STRING_BYTES 200


/**************************************************************************************
   GLOBALNE VARIJABLE
***************************************************************************************/
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

double wx = 2.3, wy = 3.3, wz = 4.3;
double angle = 0;

void print_w(int sec){
  Serial.flush();
  
  for(int j = 50 * sec; j > 0; j--){
    Serial.print(wx, 4);
    Serial.print(",");
    Serial.print(wy, 4);
    Serial.print(",");
    Serial.println(wz, 4);
    delay(20);

    wx = 50*cos(angle + 0);
    wy = 70*cos(angle + 1.3);
    wz = 90*cos(angle + 2.3);
    angle += 0.05;
    
  }
}

void stop(void) {
  while (1);
}


void motor(int state) {
  if (state == 0 || state == 1) {
    digitalWrite(MOTOR_PIN, state);
  } else
    return;
}


void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(MAX_INPUT_STRING_BYTES);

  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
}

void loop() {
  if (stringComplete) {
    // convert String to C-type string with '\0'
    const char *inputString_c = inputString.c_str();

    Serial.print(parse(inputString_c));
    inputString = ""; // clear the string:
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
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
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:

  }
}
