


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