#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "adt_test_data.h"
#include "adt_list.h"
#include "adt_error.h"

#define DATA_COUNT 10000

typedef enum even_odd {EVENS,ODDS} even_odd_t;

#define CREATE_LIST_EVENS(tdata) setup_and_populate_list_every_other(tdata,EVENS)
#define CREATE_LIST_ODDS(tdata) setup_and_populate_list_every_other(tdata,ODDS)

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

LIST setup_and_populate_list_every_other(adt_test_data *test_data,even_odd_t which)
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
    if(ODDS==which)
    {
        i=0;
    }else if(EVENS==which)
    {
        i=1;
    } else
    {
        list_destroy(&list);
        return list;
    }

    for(;i<DATA_COUNT;i+=2)
    {
        t_item=test_data->test_items[i];
        status=lladd(list,(void *)t_item->data,t_item->number,&node);
        assert(ADT_OK==status);

    }
    return list;

}

adt_status test_insert_before(void)
{
    int i;
    char *data;
    adt_status status;
    adt_test_item *t_item;
    adt_test_item *next_t_item;
    
    adt_test_data *td1;
    adt_test_data *td2;
    
    NODE node;
    LIST list;
    
    printf("%s\n",STARS);
    printf("Testing llinsert_before().\n\n");
    
    printf("creating test data.\n");
    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);
    
    if(NULL==td1 || NULL==td2)
    {
        printf("Failed to generate test data.\n");
        status=ADT_NO_MEM;
        goto end;
    }

    list=CREATE_LIST_EVENS(td1);
       
    assert(NULL!=list);

    data=NULL;
    status=llget_first(list,&node,(void **)(&data));
    assert(ADT_OK==status);

    for(i=0;i<DATA_COUNT;i+=2)
    {
        t_item=td1->test_items[i];
        next_t_item=td1->test_items[i+1];
        assert(strcmp(next_t_item->data,data)==0);
        
        status=llinsert_before(list,(void *)(t_item->data),t_item->number, &node);
        assert(ADT_OK==status);
        
        status=llget_next(list,&node,(void **)&data);
        assert(ADT_OK==status);
        //skip 2
        status=llget_next(list,&node,(void **)&data);
        assert(ADT_OK==status);
    }
    

    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td2->test_items[i];
        status=llremove_first(list,(void **)(&data));
        assert(ADT_OK==status);
        assert(strcmp(t_item->data,data)==0);
        data=NULL;
    }

    printf("destroying list.\n");
    list_destroy(&list);

    printf("Destroyed list.\n");

    status=ADT_OK;
    
    printf("Test successful\n");

end: 
    printf("destroying test data.\n\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);
    
    return status;
}

adt_status test_insert_after(void)
{
    int i;
    char *data;
    adt_status status;
    adt_test_item *t_item;
    adt_test_item *next_t_item;
    adt_test_data *td1;
    adt_test_data *td2;
    NODE node;
    LIST list;

    printf("%s\n",STARS);
    printf("Testing llinsert_after().\n\n");
    
    printf("creating test data.\n");
    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);
    
    if(NULL==td1 || NULL==td2)
    {
        printf("Failed to generate test data.\n");
        status=ADT_NO_MEM;
        goto end;
    }
    
    list=CREATE_LIST_ODDS(td1);
    
    assert(NULL!=list);
    
    data=NULL;
    status=llget_first(list,&node,(void **)(&data));
    assert(ADT_OK==status);

    for(i=0;i<DATA_COUNT;i+=2)
    {
        t_item=td1->test_items[i];
        next_t_item=td1->test_items[i+1];
        assert(strcmp(t_item->data,data)==0);

        status=llinsert_after(list,(void *)(next_t_item->data),t_item->number,&node);
        assert(ADT_OK==status);
        
        //skip 1
        status=llget_next(list,&node,(void **)&data);
        assert(ADT_OK==status);
    }

    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td2->test_items[i];
        status=llremove_first(list,(void **)(&data));
        assert(ADT_OK==status);
        assert(strcmp(t_item->data,data)==0);
        data=NULL;
    }

    printf("destroying list.\n");
    list_destroy(&list);

    printf("Destroyed list.\n");

    status=ADT_OK;

    printf("Test successful.\n");

end:
    printf("destroying test data.\n\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);
    
    return status;

}


adt_status test_remove_last(void)
{
    int i;
    char *data;
    adt_status status;
    adt_test_item *t_item;
    adt_test_data *td1;
    adt_test_data *td2;

    LIST list;
    
    printf("%s\n",STARS);
    printf("Testing llremove_last().\n\n");

    printf("creating test data.\n");
    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);
    
    if(NULL==td1 || NULL==td2)
    {
        printf("Failed to generate test data.\n");
        status=ADT_NO_MEM;
        goto end;
    }   
    list=setup_and_populate_list(td1);
    data=NULL;
    printf("removing data from list.\n");
    for(i=DATA_COUNT-1;i>=0;i--)
    {
        t_item=td2->test_items[i];
        status=llremove_last(list,(void **)(&data));
        assert(ADT_OK==status);
        assert(strcmp(t_item->data,data)==0);
        data=NULL;
    }

    printf("destroying list.\n");
    list_destroy(&list);

    printf("Destroyed list.\n");

    status=ADT_OK;
    
    printf("Test successful.\n");
end:
    printf("destroying test data.\n\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);

    return status;
}

adt_status test_remove_first(void)
{
    int i;
    char *data;
    adt_status status;
    adt_test_item *t_item;
    adt_test_data *td1;
    adt_test_data *td2;
    
    LIST list;

    printf("%s\n",STARS);
    printf("Testing llremove_first()\n\n");

    printf("creating test data.\n");
    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);
    
    if(NULL==td1 || NULL==td2)
    {
        printf("Failed to generate test data.\n");
        status=ADT_NO_MEM;
        goto end;
    }

    list=setup_and_populate_list(td1);
    assert(NULL != list);
    data=NULL;
    printf("removing data from list.\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td2->test_items[i];
        status=llremove_first(list,(void **)(&data));
        assert(ADT_OK==status);
        assert(strcmp(t_item->data,data)==0);
        data=NULL;
    }

    printf("destroying list.\n");
    list_destroy(&list);
    printf("Destroyed list.\n");

    status=ADT_OK;
    
    printf("Test successful.\n");

end:
    printf("destroying test data.\n\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);
    return status;
}

int main(void)
{
    adt_status status;
    
    
    status=test_remove_last();
    assert(ADT_OK==status);
    
    status=test_remove_first();
    assert(ADT_OK==status);
    
    status=test_insert_before();
    assert(ADT_OK==status);
    
    status=test_insert_after();
    assert(ADT_OK==status);

    printf("All tests succesful.\n");
    printf("%s\n\n",STARS);

    
    return 0;
}


