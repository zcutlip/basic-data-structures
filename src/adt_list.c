#include "adt_conditional_includes.h"
#include <string.h>

#include "adt_malloc.h"
#include "adt_error.h"
#include "adt_list.h"

typedef struct list_struct
{
    size_t length;
    NODE first;
    NODE last;
    NODE current;
} list_t;

typedef struct node_struct
{
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

    list = adt_malloc(sizeof(list_t));

    if(NULL == list)
    {
        goto end;
    }
    list->length=0;
    list->first=NULL;
    list->last=NULL;
    list->current=NULL;
    
end:
    return list;
} //end list_create()

void list_destroy(LIST *list_p)
{
    if(!list_p)
    {
        return;
    }

    adt_free(*list_p);
    *list_p = NULL;
    
    return;
} //end list_destroy()

size_t list_length(LIST list)
{
    if(NULL == list)
    {
        return 0;
    }
    
    return list->length;

}

adt_status lladd(LIST list, void *data,NODE* node_p)
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

adt_status llinsert_after(LIST list, void *data, NODE *node_p)
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

    node->data=data;
    node->next=after;
    node->previous=before;

    before->next=node;
    
    if(NULL != after)
    {
        after->previous=node;
    }
    
    list->current=node;
    if(list->last == before)
    {
        list->last = node;
    }
    
    list->length++;

    status = ADT_OK;
end:
    return status;
}

adt_status llinsert_before(LIST list, void *data, NODE *node_p)
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
   
    node->data=data;
    node->next=after;
    node->previous=before;

    after->previous=node;

    if(NULL!=before)
    {
        before->next=node;
    }
    
    list->current=node;
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
    NODE node;

    if(NULL==list || NULL==node_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    if(NULL == list->first)
    {
        status=ADT_EMPTY;
        goto end;
    }
    node=list->first;
    list->current=node;

    if(NULL!=node && NULL != data_p)
    {
        *data_p = node->data;
    }

    *node_p=node;
    
    
    status=ADT_OK;
end:
    return status;
}

adt_status llget_last(LIST list, NODE *node_p, void **data_p)
{
    adt_status status;
    NODE node;

    if(NULL==list || NULL == data_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }
    
    if(NULL == list->last)
    {
        status=ADT_EMPTY;
        goto end;
    }
    node=list->last;
    list->current=node;
    
    if(NULL!= node && NULL != data_p)
    {
        *data_p=node->data;
    }
    
    *node_p=node;

    status=ADT_OK;
end:
    return status;
}

adt_status llget_next(LIST list, NODE *node_p, void **data_p)
{
    adt_status status;
    NODE node;


    node=NULL;

    if(NULL == list || NULL == node_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    if(NULL != list->current && list->current != list->last)
    {
        node=list->current->next;
        list->current=node;
    }else
    {
        status=ADT_END_LIST;
        goto end;
    }
    
    if(NULL != node && NULL != data_p)
    {
        *data_p=node->data;
    }

    *node_p=node;
    
    status=ADT_OK;
end:
    return status;
}

adt_status llget_prev(LIST list, NODE *node_p, void **data_p)
{
    adt_status status;
    NODE node;

    node=NULL;

    if(NULL == list || NULL == node_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    if(NULL != list->current && list->current != list->first)
    {
        node=list->current->previous;
        list->current=node;
    }else
    {
        status = ADT_END_LIST;
        goto end;
    }

    if(NULL != node && NULL != data_p)
    {
        *data_p = node->data;
    }

    *node_p=node;
    
    status=ADT_OK;
end:
    return status;
}

adt_status llget_current(LIST list,NODE *node_p, void **data_p)
{
    adt_status status;
    NODE node;

    node=NULL;

    if(NULL==list || NULL == node_p)
    {
        status=ADT_INVALID_PARAM;
        goto end;
    }

    node=list->current;

    if(NULL != node && NULL != data_p)
    {
        *data_p = node->data;
    }

    *node_p=node;
    
    status=ADT_OK;
end:
    return status;
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
        status=ADT_EMPTY;
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
        status=ADT_EMPTY;
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
    
    if(list->length <= 0)
    {
        return ADT_EMPTY;
        goto end;
    }

    if(list->last == *node_p)
    {
        if(list->current == list->last)
        {
            list->current = (*node_p)->previous;
        }
        list->last = (*node_p)->previous;
    } else if(list->first == *node_p)
    {
        if(list->current == list->first)
        {
            list->current = (*node_p)->next;
        }
        list->first = (*node_p)->next;
    }else
    {
        list->current=(*node_p)->next;
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
    status=ADT_OK;
end:
    return status;
}



static NODE __llnode_create(void)
{
    NODE node;

    node=adt_malloc(sizeof(node_t));
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
    adt_free(*node_p);
    *node_p=NULL;

    return;
}

