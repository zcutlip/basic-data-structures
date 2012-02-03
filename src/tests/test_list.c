#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "adt_test_data.h"
#include "adt_list.h"
#include "adt_error.h"

#define DATA_COUNT 10000

LIST setup_and_populate_list(adt_test_data *test_data)
{
    int i;
    LIST list;
    adt_status status;
    adt_test_item *t_item;
    NODE node;

    assert(NULL!=test_data);

    printf("creating list.\n");
    list=list_create();
    assert(NULL!=list);
    
    printf("created list.\n");
    
    printf("populating list.\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=test_data->test_items[i];
        status=lladd(list,(void *)t_item->data,t_item->number,&node);
        assert(ADT_OK==status);

    }
    return list;

}


adt_status test_remove_last(adt_test_data *test_data)
{
    int i;
    char *data;
    adt_status status;
    adt_test_item *t_item;

    LIST list;

    list=setup_and_populate_list(test_data);
    data=NULL;
    printf("removing data from list.\n");
    for(i=DATA_COUNT-1;i>=0;i--)
    {
        t_item=test_data->test_items[i];
        status=llremove_last(list,(void **)(&data));
        assert(ADT_OK==status);
        assert(strncmp(t_item->data,data,ADT_TEST_MAX_STRLEN)==0);
        data=NULL;
    }

    printf("destroying list.\n");
    list_destroy(&list);

    printf("Destroyed list.\n");

    status=ADT_OK;
    
    return status;
}

adt_status test_remove_first(adt_test_data *test_data)
{
    int i;
    char *data;
    adt_status status;
    adt_test_item *t_item;
    
    LIST list;

    list=setup_and_populate_list(test_data);
    assert(NULL != list);
    data=NULL;
    printf("removing data from list.\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=test_data->test_items[i];
        status=llremove_first(list,(void **)(&data));
        assert(ADT_OK==status);
        assert(strncmp(t_item->data,data,ADT_TEST_MAX_STRLEN)==0);
        data=NULL;
    }

    printf("destroying list.\n");
    list_destroy(&list);
    printf("Destroyed list.\n");

    status=ADT_OK;
    
    return status;
}

int main(void)
{
    adt_status status;
    adt_test_data *test_data;
    
    
    printf("creating test data.\n");
    test_data=adt_test_data_create(DATA_COUNT);
    
    status=test_remove_last(test_data);
    assert(ADT_OK==status);
    
    status=test_remove_first(test_data);
    assert(ADT_OK==status);

    printf("destroying test data.\n");
    adt_test_data_destroy(&test_data);

    return 0;
}


