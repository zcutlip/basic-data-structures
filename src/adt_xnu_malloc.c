//
//  adt_xnu_malloc.c
//  KmemAnalyzer
//
//  Created by Zachary Cutlip on 9/17/14.
//  Copyright (c) 2014 Zachary Cutlip. All rights reserved.
//



#include "adt_xnu_malloc.h"
#ifdef ADT_XNU_MALLOC
#include <sys/malloc.h>
void *adt_xnu_malloc(size_t size)
{
    void *ptr;
    ptr=NULL;
    MALLOC(ptr, void *, size, M_TEMP, M_WAITOK);
    return ptr;
}

void adt_xnu_free(void *ptr)
{
    FREE(ptr, M_TEMP);
}

#endif //ADT_XNU_MALLOC