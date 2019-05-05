#ifndef _Z_OBJECT_H
#define _Z_OBJECT_H

#include "ztypes.h"

#define ZObject_HEADER \
  ZObjectTypeC ob_type;

typedef struct __zobj {
  ZObject_HEADER
} ZObject;

ZTypeN_Addr ZObject_GetID(ZObject *o);
ZObjectTypeC ZObject_GetType(ZObject *o);

#endif /* end of include guard: _Z_OBJECT_H */
