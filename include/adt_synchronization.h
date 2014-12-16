
#ifndef _ADT_SYNCHRONIZATION_H
#define _ADT_SYNCHRONIZATION_H

typedef struct adt_lock_t *ADT_LOCK;
#include "adt_error.h"

#ifdef ADT_XNU

#include "adt_xnu_synchronization.h"

#define _adt_rw_lock_init()    _adt_xnu_rw_lock_init()
#define _adt_read_lock(x)       _adt_xnu_read_lock(x)
#define _adt_read_unlock(x)     _adt_xnu_read_unlock(x)
#define _adt_write_lock(x)      _adt_xnu_write_lock(x)
#define _adt_write_unlock(x)    _adt_xnu_write_unlock(x)
#define _adt_rw_lock_destroy(x) _adt_xnu_rw_lock_destroy(x)

#else

#include "adt_posix_synchronizaton.h"

#define _adt_rw_lock_init()    _adt_posix_rw_lock_init()
#define _adt_read_lock(x)       _adt_posix_read_lock(x)
#define _adt_read_unlock(x)     _adt_posix_read_unlock(x)
#define _adt_write_lock(x)      _adt_posix_write_lock(x)
#define _adt_write_unlock(x)    _adt_posix_write_unlock(x)
#define _adt_rw_lock_destroy(x) _adt_posix_rw_lock_destroy(x)

#endif





#endif /* _ADT_SYNCHRONIZATION_H */
