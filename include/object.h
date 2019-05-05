#ifndef _Z_OBJECT_H
#define _Z_OBJECT_H

#include "ztypes.h"

#define ZObject_HEADER \
  ZObjectTypeC ob_type;

#define CAST_ZOBJECT_PTR(typ, obj) (((typ *))(obj))

typedef struct __zobj {
  ZObject_HEADER
} ZObject;

ZObject *ZObject_Create(ZObjectTypeC type);
void ZObject_Forget(ZObject *o);
ZTypeN_Addr ZObject_GetID(ZObject *o);

#endif /* end of include guard: _Z_OBJECT_H */
