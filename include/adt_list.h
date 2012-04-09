#ifndef _ADT_LIST_H
#define _ADT_LIST_H

#include "adt_error.h"

typedef struct list_struct *LIST;
typedef struct node_struct *NODE;

LIST list_create(void);
void list_destroy(LIST *list_p);

adt_status lladd(LIST list, void *data, int key,NODE* node_p);
adt_status llinsert_after(LIST list, void *data, int key, NODE *node_p);
adt_status llinsert_before(LIST list, void *data, int key, NODE *node_p);

adt_status llget_first(LIST list, NODE *node_p, void **data_p);
adt_status llget_last(LIST list, NODE *node_p, void **data_p);
adt_status llget_prev(LIST list, NODE *node_p, void **data_p);
adt_status llget_next(LIST list, NODE *node_p, void **data_p);


/* get a handle on the list's "current" node, and get it's data */
adt_status llget_current(LIST list, NODE *node_p, void **data_p);

adt_status llremove_node(LIST list, NODE *node_p, void **data_p);
adt_status llremove_first(LIST list, void **data_p);
adt_status llremove_last(LIST list, void **data_p);

#endif /* _ADT_LIST_H */

