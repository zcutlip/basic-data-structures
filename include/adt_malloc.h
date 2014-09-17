//
//  adt_malloc.h
//  KmemAnalyzer
//
//  Created by Zachary Cutlip on 9/17/14.
//  Copyright (c) 2014 Zachary Cutlip. All rights reserved.
//

#ifndef _ADT_MALLOC_H
#define _ADT_MALLOC_H

#ifdef ADT_XNU
#include "adt_xnu_malloc.h"
#define adt_malloc(x)   adt_xnu_malloc(x)
#define adt_free(x)     adt_xnu_free(x)
#else
#define adt_malloc(x)   malloc(x)
#define adt_free(x)     free(x)
#endif

#endif /* _ADT_MALLOC_H */
