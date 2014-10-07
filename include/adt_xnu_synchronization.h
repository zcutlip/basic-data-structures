
#ifndef _ADT_XNU_SYNCHRONIZATOIN_H
#define _ADT_XNU_SYNCHRONIZATOIN_H


#ifdef ADT_XNU
#define ADT_XNU_SYNC

#include "adt_conditional_includes.h"
#include "adt_error.h"

ADT_LOCK _adt_xnu_rw_lock_init();
adt_status _adt_xnu_read_lock(ADT_LOCK lock);
adt_status _adt_xnu_read_unlock(ADT_LOCK lock);
adt_status _adt_xnu_write_lock(ADT_LOCK lock);
adt_status _adt_xnu_write_unlock(ADT_LOCK lock);
void _adt_xnu_rw_lock_destroy(ADT_LOCK *lockp);

#endif /* ADT_XNU */

#endif /* _ADT_XNU_SYNCHRONIZATOIN_H */
