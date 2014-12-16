//
//  adt_posix_synchronization.c
//  KmemAnalyzer
//
//  Created by Zachary Cutlip on 10/9/14.
//  Copyright (c) 2014 Zachary Cutlip. All rights reserved.
//


#include "adt_synchronization.h"
#include "adt_conditional_includes.h"

#ifdef ADT_POSIX_SYNC

ADT_LOCK _adt_posix_rw_lock_init()
{
    return NULL;
}

adt_status _adt_posix_read_lock(ADT_LOCK lock)
{
    return ADT_ERROR;
}

adt_status _adt_posix_read_unlock(ADT_LOCK lock)
{
    return ADT_ERROR;
}

adt_status _adt_posix_write_lock(ADT_LOCK lock)
{
    return ADT_ERROR;
}
adt_status _adt_posix_write_unlock(ADT_LOCK lock)
{
    return ADT_ERROR;
}
void _adt_posix_rw_lock_destroy(ADT_LOCK *lockp)
{
    return;
}

#endif /* ADT_POSIX_SYNC */