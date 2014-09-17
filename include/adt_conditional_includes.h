//
//  adt_conditional_includes.h
//  KmemAnalyzer
//
//  Created by Zachary Cutlip on 9/16/14.
//  Copyright (c) 2014 Zachary Cutlip. All rights reserved.
//

#ifndef _ADT_COND_INCL_H
#define _ADT_COND_INCL_H

#ifdef ADT_XNU
#include <libkern/libkern.h>
//include XNU-only stuff
#else
/* non-xnu stuff */
#include <stdlib.h>
#include <stdio.h>
#endif


#endif /* _ADT_COND_INCL_H */
