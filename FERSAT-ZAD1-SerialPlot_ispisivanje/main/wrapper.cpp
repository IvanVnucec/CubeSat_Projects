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
void motor_wrapp(list_t *arg) {
  char *arg1_str = (char *)malloc(1 * sizeof(char));
  list_remove(arg, arg1_str, 1*sizeof(char));

  int state = (int)(arg1_str[0] - '0'); // convert ASCII to int

  motor(state);
    
  free(arg1_str);
}


void stop_wrapp(list_t *arg) {
  stop();
}


void print_w_wrapper(list_t *arg) {
  char *arg1_str = (char *)malloc(1 * sizeof(char));
  list_remove(arg, arg1_str, 1*sizeof(char));

  int state = (int)(arg1_str[0] - '0'); // convert ASCII to int

  print_w(state);
    
  free(arg1_str);
}


int function_wrapper_init (function_list_t *function_list) {
  
    if(function_list_add(function_list, "motor",   1, motor_wrapp))  return 1;
    if(function_list_add(function_list, "stop",    0, stop_wrapp))   return 1;
    if(function_list_add(function_list, "print_w", 1, print_w_wrapper))  return 1;

    return 1;
}
