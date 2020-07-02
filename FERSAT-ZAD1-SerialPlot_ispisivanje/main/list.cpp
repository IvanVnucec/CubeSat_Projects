#include"list.h"

void list_init (list_t *list){
  list->in = NULL;
  list->out = NULL;
}

void list_deinit (list_t *list){
    list_atom_t *old_atom;

    while (list->out) {
        free((list->out)->data);
    old_atom = list->out;
    list->out = (list->out)->next;
    free (old_atom);
    if (list->out == NULL){
            list->in = NULL;
    }
    }
}

int list_append (list_t *list, const void *data, int data_lenght) {
  list_atom_t *new_atom;

  new_atom = (list_atom_t *)malloc(sizeof(list_atom_t));

  new_atom->data = (void *)malloc(data_lenght);

  if (new_atom && new_atom->data) {
    memcpy(new_atom->data, data, data_lenght);

    new_atom->next = NULL;
    if (list->out == NULL) {
      list->out = new_atom;
    } else {
      (list->in)->next = new_atom;
    }
    list->in = new_atom;

    return 0;
  }

  return 1;
}

int list_remove (list_t *list, void *data, int data_lenght) {
  list_atom_t *old_atom;

  if (list->out) {
        memcpy(data, (list->out)->data, data_lenght);    // STA AKO NE STANE
        free((list->out)->data);
    old_atom = list->out;
    list->out = (list->out)->next;
    free (old_atom);
    if (list->out == NULL){
            list->in = NULL;
    }

    return 0;
    }

  return 1;
}

int list_lenght (list_t *list){
    list_atom_t *curr = list->out;
    int i = 0;

    if(curr == NULL)
        return 0;

    while ((curr = curr->next))
        i++;

    return (i+1);
}
