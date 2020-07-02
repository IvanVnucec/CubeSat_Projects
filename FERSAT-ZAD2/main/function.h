#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED


#include <stdlib.h>
#include<string.h>
#include"list.h"

#define MAX_FUN_NAME 20

typedef struct function_call {
    char name[MAX_FUN_NAME + 1];
    int n_args;
    void (*function_wrapper_ptr)(list_t *);
} function_call_t;


typedef struct atom {
  function_call_t *data;
  struct atom *next;
} atom_t;

typedef list_t function_list_t;


void function_list_init(function_list_t *function_list);
void function_list_deinit(function_list_t *function_list);
int function_list_append(function_list_t *function_list, function_call_t *data);
int function_list_remove(function_list_t *function_list, function_call_t *data);

int function_list_add(function_list_t *function_list, const char* fun_name, int num_args, void (*fun_wrapper_ptr)(list_t *));
int function_list_call(function_list_t *function_list, const char *fun_name, list_t *arg);

#endif // FUNCTION_LIST_H_INCLUDED
