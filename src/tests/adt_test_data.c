#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_test_data.h"

#define DEFAULT_TEST_STRING "This is test data item number"

static adt_test_item *__test_item_create(int number);
static void __test_item_destroy(adt_test_item **t_item_p);


adt_test_data *adt_test_data_create(unsigned int count)
{
    adt_test_data *t_data;
    adt_test_item *t_item;
    unsigned int i;

    t_data=malloc(sizeof(struct adt_test_data_struct));
    if(NULL==t_data)
    {
        goto end;
    }
    memset(t_data,0,sizeof(struct adt_test_data_struct));

    t_data->test_items=malloc(count * sizeof(struct adt_test_item_struct));

    if(NULL==t_data->test_items)
    {
        free(t_data);
        t_data=NULL;
        goto end;
    }
    
    for(i=0;i<count;i++)
    {
        t_item=__test_item_create(i+1);
        if(NULL==t_item)
        {
            adt_test_data_destroy(&t_data);
            goto end;
        }
        t_data->test_items[i]=t_item;
        t_data->item_count=i+1;
    }
    
end:
    return t_data;
}


void adt_test_data_destroy(adt_test_data **t_data_p)
{
    int i;
    adt_test_item *t_item;

    if(NULL==t_data_p || NULL==*t_data_p)
    {
        return;
    }


    for(i=0;i<(*t_data_p)->item_count;i++)
    {
        t_item=(*t_data_p)->test_items[i];
        __test_item_destroy(&t_item);
        (*t_data_p)->test_items[i]=NULL;
    }
    free((*t_data_p)->test_items);
    free(*t_data_p);
    *t_data_p=NULL;

    return;
}


static adt_test_item *__test_item_create(int number)
{
    adt_test_item *t_item;

    t_item=malloc(sizeof(struct adt_test_item_struct));
    if(NULL==t_item)
    {
        goto end;
    }

    memset(t_item,0,sizeof(struct adt_test_item_struct));

    t_item->number=number;
    t_item->removed=0;

    snprintf(t_item->data,ADT_TEST_MAX_STRLEN,
            "%s: %d",DEFAULT_TEST_STRING,number);

end:
    return t_item;
}

static void __test_item_destroy(adt_test_item **t_item_p)
{
    if(NULL == t_item_p || NULL == *t_item_p)
    {
        return;
    }
    free(*t_item_p);
    *t_item_p=NULL;

    return;

}
