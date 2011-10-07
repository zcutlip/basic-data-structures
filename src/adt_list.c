#include <stdlib.h>
#include <string.h>

#include "adt_error.h"
#include "adt_list.h"

typedef struct list_struct
{
    int length;
    NODE first;
    NODE last;
    NODE current;
} list_t;

typedef struct node_struct
{
    int key;
    void *data;
    NODE next;
    NODE previous;
} node_t;

LIST list_create(void)
{
    LIST list;

    list = NULL;

    list = malloc(sizeof(list_t));

    if(NULL == list)
    {
        goto end;
    }
    memset(list,0,sizeof(list_t));
    
end:
    return list;
}

void list_destroy(LIST *list_p)
{
    if(!list_p)
    {
        return;
    }

    free(*list_p);
    *list_p = NULL;
    
    return;
}


