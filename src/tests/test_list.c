#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "adt_test_data.h"
#include "adt_list.h"
#include "adt_error.h"

#define DATA_COUNT 100

int main(void)
{
    int i;
    char *data;
    adt_status status;
    adt_test_data *test_data;
    adt_test_item *t_item;
    
    NODE node;
    LIST list;
    
    printf("creating test data.\n");
    test_data=adt_test_data_create(DATA_COUNT);

    printf("creating list.");
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
    
    printf("removing data from list.\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=test_data->test_items[i];
        status=llremove_last(list,(void **)(&data));
        assert(ADT_OK==status);
        assert(strncmp(t_item->data,data,ADT_TEST_MAX_STRLEN)==0);

    }

    printf("destroying list.\n");
    list_destroy(&list);

    printf("Destroyed list.\n");
    
    printf("destroying test data.\n");
    adt_test_data_destroy(&test_data);

    return 0;
}


