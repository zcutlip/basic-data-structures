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

static void __llnode_destroy(NODE *node_p);
static NODE __llnode_create(void);


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
} //end list_create()

void list_destroy(LIST *list_p)
{
    if(!list_p)
    {
        return;
    }

    free(*list_p);
    *list_p = NULL;
    
    return;
} //end list_destroy()

adt_status lladd(LIST list, void *data, int key,NODE* node_p)
{
    adt_status status;
    NODE node;
    NODE tmp;

    if(NULL==list)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    

    node=__llnode_create();

    if(NULL==node)
    {
        status=ADT_NO_MEM;
        goto end;
    }
    
    node->key=key;
    node->data=data;
    node->next=NULL;
    
    tmp=list->last;
    if(NULL!=tmp)
    {
        tmp->next=node;
    }
    node->previous=tmp;
    
    list->last=node;
    list->current=node;
    if(NULL == list->first)
    {
       list->first=node; 
    }
    
    list->length++;

    if(NULL != node_p)
    {
        *node_p=node;
    }

    status=ADT_OK;
    
end:
    return status;

} //end lladd()

adt_status llinsert_after(LIST list, void *data, int key, NODE *node_p)
{
    adt_status status;
    NODE node;
    NODE before;
    NODE after;

    if(NULL == list || NULL==node_p || NULL==*node_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    node=__llnode_create();

    if(NULL==node)
    {
        status=ADT_NO_MEM;
        goto end;
    }
    
    before=*node_p;
    after=before->next;

    node->key=key;
    node->data=data;
    node->next=after;
    node->previous=before;

    before->next=node;
    
    if(NULL != after)
    {
        after->previous=node;
    }

    if(list->last == before)
    {
        list->last = node;
    }
    
    list->length++;

    status = ADT_OK;
end:
    return status;
}

adt_status llinsert_before(LIST list, void *data, int key, NODE *node_p)
{
    adt_status status;
    NODE node;
    NODE before;
    NODE after;

    if(NULL==list || NULL==node_p || NULL==*node_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    node=__llnode_create();
    if(NULL==node)
    {
        status=ADT_NO_MEM;
        goto end;
    }

    after=*node_p;
    before=after->previous;
   
    node->key=key;
    node->data=data;
    node->next=after;
    node->previous=before;

    after->previous=node;

    if(NULL!=before)
    {
        before->next=node;
    }

    if(list->first==after)
    {
        list->first=node;
    }

    list->length++;

    status=ADT_OK;
end:
    return status;
}
    

adt_status llget_first(LIST list, NODE *node_p, void **data_p)
{
    adt_status status;

    if(NULL==list || NULL==node_p)
    {
    }
}


adt_status llremove_last(LIST list, void **data_p)
{
    adt_status status;
    NODE node;

    if(NULL == list || NULL == data_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    
    node=list->last;
    if(NULL==node)
    {
        //should an empty list be an error?
        status=ADT_OK;
        goto end;
    }

    if(list->first == list->last)
    {
        list->first = NULL;
    }
    if(list->current == node)
    {
        list->current = list->last;
    }
    list->last=node->previous;
    if(NULL != node->previous)
    {
        //node->previous is now the last, so there is no previous->next
        node->previous->next=NULL;
    }

    *data_p=node->data;
    
    __llnode_destroy(&node);
    list->length--;

    status=ADT_OK;
end:
    return status;
} //end llremove_last()


adt_status llremove_first(LIST list, void **data_p)
{
    adt_status status;
    NODE node;
    if(NULL == list || NULL == data_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    
    node=list->first;
    if(NULL==node)
    {
        //should an empty list be an error?
        status=ADT_OK;
        goto end;


    }
    if(list->last == list->first)
    {
        list->last=NULL;
    }

    list->first=node->next;
    if(list->current == node)
    {
        list->current=node->next;
    }

    if(NULL != node->next)
    {
        //node->next is now the first, so there is no next->previous
        node->next->previous=NULL;
    }

    *data_p=node->data;

    __llnode_destroy(&node);
    list->length--;

    status=ADT_OK;
end:
    return status;
    
} //end llremove_first()


adt_status llremove_node(LIST list, NODE *node_p, void **data_p)
{
    adt_status status;
    
    if(NULL == list || NULL == node_p || NULL == *node_p || NULL == data_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    if(list->last == *node_p)
    {
        if(list->current == list->last)
        {
            list->current = (*node_p)->previous;
        }
        list->last = (*node_p)->previous;
    }
    
    if(list->first == *node_p)
    {
        if(list->current == list->first)
        {
            list->current = (*node_p)->next;
        }
        list->first = (*node_p)->next;
    }

    if(NULL != (*node_p)->next)
    {
        (*node_p)->next->previous=(*node_p)->previous;
    }
    if(NULL != (*node_p)->previous)
    {
        (*node_p)->previous->next=(*node_p)->next;
    }
    
    *data_p=(*node_p)->data;

    __llnode_destroy(node_p);
    list->length--;
    
end:
    return status;
}



static NODE __llnode_create(void)
{
    NODE node;

    node=malloc(sizeof(node_t));
    if(NULL==node)
    {
        goto end;
    }

    memset(node,0,sizeof(node_t));

end:
    return node;
}

static void __llnode_destroy(NODE *node_p)
{
    if(!node_p || !(*node_p))
    {
        return;
    }
    free(*node_p);
    *node_p=NULL;

    return;
}
