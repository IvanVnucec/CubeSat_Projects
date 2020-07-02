#include"parser.h"

static void string_shift_left (char *string, int n) {
    int i;
    int len = strlen(string);

    for(i=0; i<=(len-n); i++){
        string[i] = string[i+n];
    }
}


/* Funkcija parser_function_name koristi malloc() za *function_name
 * i potrebno je taj pointer poslije ocistit sa free()!
*/
int parser_function_name (char *string, char **function_name) {
    int i;
    int len = strlen(string);

    i=0;
    while(i<len && string[i] != '(')
        i++;

    if (i==0 || i==len) {
        // ERROR
        return 1;
    }

    if((*function_name = (char *)malloc((i + 1) * sizeof(char))) == NULL)
        return 1;

    strncpy(*function_name, string, i);
    (*function_name)[i] = '\0';

    string_shift_left(string, i);

    return 0;
}


int parser_function_arg (char *string, list_t *arg){
    char buffer[MAX_ARG_LEN + 1];
    int i, j;

    i = 1;
    j = 0;

    if(string[0] != '('){       // ERROR
        return 1;
    }

    if(string[1] == ')'){       // no arguments
        return 0;
    }

    /* PARSE ARGUMENTS OF A FUNCTION */
    while(string[i] != ')'){
        if(string[i] == ','){
            buffer[j] = '\0';
            if (list_append(arg, buffer, sizeof(char)*(j+1))) {
                return 1;
            }

            i++;
            j = 0;
        }

        buffer[j] = string[i];
        i++;
        j++;
    }

    buffer[j] = '\0';                          // ovaj je zadnji argument liste

    if(list_append(arg, buffer, sizeof(char)*(j+1))) {
        return 1;
    }

    return 0;
}

int parse(const char *str){
    function_list_t function_list;
    list_t args;
    char *str_copy = (char *)malloc((strlen(str)+1) * sizeof(char));

    if(str_copy == NULL)
        return 0;

    char *function_name = NULL;

    strcpy(str_copy, str);

    list_init(&args);
    function_list_init(&function_list);
    if(!function_wrapper_init(&function_list))    // TREBA POPRAVIT OVO SA ERROR HANDLINGOM. TO JE STURO SVE NAPISANO
        return 1;

    if(parser_function_name (str_copy, &function_name))
        return 2;

    if(parser_function_arg (str_copy, &args))   // OVO JE KRIVO KADA SE STAVI PUNO ARGUMENATA
        return 3;

    if(function_list_call(&function_list, function_name, &args))    // isto error hendling
        return 4;

    free (function_name);
    free (str_copy);

    function_list_deinit(&function_list);
    list_deinit(&args);

    return 5;
}
