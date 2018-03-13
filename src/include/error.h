#ifndef __error_h
#define __error_h

#include "stdio.h"

//print error
#define PR_ERROR printf("\x1B[35mERROR\x1B[0m %s/%s:%d\n",__FILE__,__FUNCTION__,__LINE__);

#endif