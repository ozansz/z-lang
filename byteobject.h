#ifndef _Z_BYTEOBJECT_H
#define _Z_BYTEOBJECT_H

#include "object.h"
#include "ztypes.h"

typedef struct __zbyteobjflag {
  _ZBool neg;
  _ZBool zero;
} ZByteObject_Flag;

typedef struct __zbyteobj {
  ZObject_HEADER
  ZTypeN_Byte ob_val;
  ZByteObject_Flag ob_flags;
} ZByteObject;

ZObject * ZByte_New(void);
ZObject * ZByte_FromByte(ZTypeN_SignedByte ob_val);
ZObject * ZByte_FromByteObj(ZObject* o);

ZTypeN_Byte * ZByte_GetValue(ZObject* o);
void ZByte_SetValue(ZObject* o, ZTypeN_SignedByte ob_val);

_ZBool * ZByte_IsZero(ZObject* o);
_ZBool * ZByte_IsNegative(ZObject* o);

#endif /* end of include guard: _Z_BYTEOBJECT_H */
