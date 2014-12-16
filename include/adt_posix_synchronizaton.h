

#ifndef _ADT_POSIX_SYNCHRONIZATION_H
#define _ADT_POSIX_SYNCHRONIZATION_H

#define ADT_POSIX_SYNC

ADT_LOCK _adt_posix_rw_lock_init();
adt_status _adt_posix_read_lock(ADT_LOCK lock);
adt_status _adt_posix_read_unlock(ADT_LOCK lock);
adt_status _adt_posix_write_lock(ADT_LOCK lock);
adt_status _adt_posix_write_unlock(ADT_LOCK lock);
void _adt_posix_rw_lock_destroy(ADT_LOCK *lockp);

#endif /* _ADT_POSIX_SYNCHRONIZATION_H */
