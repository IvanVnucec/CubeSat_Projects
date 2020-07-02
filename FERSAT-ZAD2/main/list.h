#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#include <stdio.h>      // FOR TESTING PURPOSES

typedef struct list_atom {
  void *data;
  struct list_atom *next;
} list_atom_t;


typedef struct list {
  list_atom_t *in, *out;
} list_t;

void list_init   (list_t *list);
void list_deinit (list_t *list);
int  list_append (list_t *list, const void *data, int data_lenght);
int  list_remove (list_t *list, void *data, int data_lenght);
int  list_lenght (list_t *list);

#endif // LIST_H_INCLUDED
