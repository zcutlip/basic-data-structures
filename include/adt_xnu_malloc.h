
#ifndef _ADT_XNU_MALLOC_H
#define _ADT_XNU_MALLOC_H

#ifdef ADT_XNU
#define ADT_XNU_MALLOC

#include "adt_conditional_includes.h"

void *adt_xnu_malloc(size_t size);
void adt_xnu_free(void *ptr);

#endif /* ADT_XNU */
#endif /* _ADT_XNU_MALLOC_H */
