

#ifndef _ADT_COND_INCL_H
#define _ADT_COND_INCL_H

#ifdef ADT_XNU
#include <libkern/libkern.h>
//include XNU-only stuff
#else
/* non-xnu stuff */
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#endif


#endif /* _ADT_COND_INCL_H */
