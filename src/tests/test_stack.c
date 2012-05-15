#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "adt_test_data.h"
#include "adt_stack.h"
#include "adt_error.h"

#define DATA_COUNT 10000

adt_status test_push_pop(void)
{
    adt_status status;
    STACK stack;
    int i;
    char *data;
    adt_test_data *td1;
    adt_test_data *td2;
    adt_test_item *t_item;
    
    printf("%s\n",STARS);
    printf("Testing push and pop\n\n");

    printf("Creating test data.\n");

    td1=adt_test_data_create(DATA_COUNT);
    td2=adt_test_data_create(DATA_COUNT);

    if(NULL == td1 || NULL == td2)
    {
        status=ADT_NO_MEM;
        goto end;
    }

    stack=stack_create();
    printf("Testing push()\n");
    for(i=0;i<DATA_COUNT;i++)
    {
        t_item=td1->test_items[i];
        status=stack_push(stack,t_item->data);
        assert(ADT_OK == status);
    }
    
    assert(stack_size(stack) == DATA_COUNT);
    printf("push() test successful.\n");
    
    printf("Testing pop().\n");
    for(i=DATA_COUNT-1;i>=0;i--)
    {
        t_item=td2->test_items[i];
        status=stack_pop(stack,(void **)&data);
        assert(ADT_OK == status);
        assert(strcmp(t_item->data,data) == 0);
    }
    
    assert(stack_size(stack) == 0);
    printf("pop() test successful.\n");
    
    printf("Destroying stack.\n");
    stack_destroy(&stack);
    printf("Destroyed stack.\n");

    status=ADT_OK;
    printf("Test successful.\n");
end:
    printf("Destroying test data.\n\n");
    adt_test_data_destroy(&td1);
    adt_test_data_destroy(&td2);
    return status;
}


int main(int argc, char **argv)
{
    adt_status status;

    status=test_push_pop();
    assert(ADT_OK==status);

    printf("All tests successful.\n");
    printf("%s\n\n",STARS);

    return 0;
}


