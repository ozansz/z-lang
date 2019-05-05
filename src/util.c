#include <stdlib.h>
#include <stdio.h>

#include "util.h"

#include "ztypes.h"

void ZCore_Forget(ZObject *o) {
  if (o == NULL)
    return;
    
  switch (o->ob_type) {
    case ZT_None:
      free((void *)o);
      break;
    case ZT_Byte:
      free((void *)o);
      break;
    case ZT_Long:
      /// Will implement later
      break;
  }
}
