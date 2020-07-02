#include"wrapper.h"
#include"main.h"

/**************************************************************************************
    Ovo je tzv. Wrapper funkcija koju pisemo za svaku funkciju koju zelimo pozvati
    preko parsera. Wrapper funkcija je svojevrsni interface izmedju parsera i vec
    napisanih funkcija u sustavu

    Svaka Wrapper funkcija prima listu argumenata list_t *arg! (OVO JE VAZNO)
    Lista argumenata je vec implementirana lista varijabli tipa string
    (sa terminatorom '\0') koja sadrzi varijable koje funkciji zelimo poslati.
    Kako su te varijable tipa string, njih je potrebno konvertirati u tipove varij. koje
    svaka od orginalnih funkcija koje se pozivaju primaju.

    Svaka Wrapper funkcija prima:
      1. Listu argumenata   (OBAVEZNO)

    Svaka Wrapper funkcija sadrzi:
      1. Rezerviranje podataka za argumente iz liste
      2. Kopiranje podataka iz liste u rezervirane podatke
      3. Konverziju iz stringova u pripadajuce tipove varijabli
      4. Pozivanje Orginalne funkcije   (OBAVEZNO)
      5. Oslobadjanje rezerviranog prostora

    Svaka Wrapper funkcija vraca:
      1. Nista, odnosno void  (OBAVEZNO)

 *    ** Stavke sa (OBAVEZNO) su vrlo vazne i one se uvijek moraju postivati, inace
         program nece raditi. Stavke bez obavezno se mogu izostaviti.
***************************************************************************************/

// WRAPPER FUNKCIJE
void setDirection_wrapp(list_t *arg) {

  // 1. Allocating memory for the function arguments
  char *arg1_str = (char *)malloc(1 * sizeof(char));

  // 2. Copying the function arguments into allocated memory
  list_remove(arg, arg1_str, 1 * sizeof(char));

  // 3. Converting function arguments from type c-string to bool
  bool dir;
  if (*arg1_str == '0') {
    dir = false;
  } else if (*arg1_str == '1') {
    dir = true;
  }

  // 4. Calling of the original function
  setDirection(dir);

  // 5. Freeing allocated memory
  free(arg1_str);
}


void setPWM_wrapp(list_t *arg) {
  // 1. Allocating memory for the function arguments
  char *arg1 = (char *)malloc(4 * sizeof(char));

  // 2. Copying the function arguments into allocated memory
  list_remove(arg, arg1, 4 * sizeof(char));

  // 3. Converting function arguments from type string to the other types (int,float etc.)
  uint8_t  fan_speed;
  fan_speed  = (arg1[0] - '0') * 100;
  fan_speed += (arg1[1] - '0') * 10;
  fan_speed += (arg1[2] - '0') * 1;

  // 4. Calling of the original function
  setPWM(fan_speed);

  // 5. Freeing allocated memory
  free(arg1);
}


void setTimer_wrapp(list_t *arg) {
  // 1. Allocating memory for the function arguments
  char *arg1 = (char *)malloc(5 * sizeof(char));

  // 2. Copying the function arguments into allocated memory
  list_remove(arg, arg1, 5 * sizeof(char));

  // 3. Converting function arguments from type string to the other types (int,float etc.)
  uint16_t pr;
  pr  = (arg1[0] - '0') * 10000;
  pr += (arg1[1] - '0') * 1000;
  pr += (arg1[2] - '0') * 100;
  pr += (arg1[3] - '0') * 10;
  pr += (arg1[4] - '0') * 1;

  // 4. Calling of the original function
  setTimer(pr);

  // 5. Freeing allocated memory
  free(arg1);
}


int function_wrapper_init (function_list_t *function_list) {

  function_list_add(function_list, "setDirection", 1, setDirection_wrapp);
  function_list_add(function_list, "setPWM",       1, setPWM_wrapp      );
  function_list_add(function_list, "setTimer",     1, setTimer_wrapp    );
  
  return 1;
}
