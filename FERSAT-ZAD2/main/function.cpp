#include"function.h"

void function_list_init (function_list_t *function_list){
  list_init(function_list);
}

void function_list_deinit (function_list_t *function_list){
    list_deinit(function_list);
}

int function_list_append (function_list_t *function_list, function_call_t *data) {
    return list_append(function_list, data, sizeof(*data));
}

int function_list_remove (function_list_t *function_list, function_call_t *data) {
    return list_remove(function_list, data, sizeof(*data));
}


int function_list_add(function_list_t *function_list, const char *fun_name, int num_args, void (*fun_wrapper_ptr)(list_t *)){
    function_call_t function_call;

    if(strlen(fun_name) > MAX_FUN_NAME)
        return 1;

    strcpy(function_call.name, fun_name);
    function_call.n_args = num_args;
    function_call.function_wrapper_ptr = fun_wrapper_ptr;

    return function_list_append(function_list, &function_call);
}


int function_list_call(function_list_t *function_list, const char *fun_name, list_t *arg){
    atom_t *curr = (atom_t *)function_list->out;
    int num_arg = 0;
    num_arg = list_lenght(arg);

    while (curr != NULL) {
        if (strcmp(fun_name, curr->data->name) == 0) {
            if (curr->data->n_args == num_arg) {
                curr->data->function_wrapper_ptr(arg);
                return 0;
            } else {
                return 1;
            }
        } else {
            curr = curr->next;
        }
    }

return 1;
}
