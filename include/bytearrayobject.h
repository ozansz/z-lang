#ifndef _Z_BYTEARRAYOBJECT_H
#define _Z_BYTEARRAYOBJECT_H

#include "ztypes.h"
#include "object.h"
#include "byteobject.h"

typedef struct __zbytearrayobjflags {
  _ZBool empty;
  _ZBool full;
} ZByteArrayObject_Flags;

typedef struct __zbytearrayobj {
  ZObject_HEADER
  ZByteObject **ob_stack;
  ZTypeN_ArraySize ob_size;
  ZByteArrayObject_Flags ob_flags;
} ZByteArrayObject;

ZObject * ZByteArray_New(ZTypeN_ArraySize ob_sz);
ZObject * ZByteArray_FromByte(ZTypeN_ArraySize ob_sz, ZObject *o);
ZObject * ZByteArray_FromByteArray(ZObject *o);

ZObject * ZByteArray_Concat(ZObject *o, ZObject *o_other);
void ZByteArray_FillWith(ZObject *o, ZObject *fw);
void ZByteArray_FillRaw(ZObject *o, ZTypeN_Byte fw);
void ZByteArray_FillWithBA(ZObject *o, ZObject *src, ZTypeN_ArraySize start_indx);

void ZByteArray_SetItem(ZObject *o, ZTypeN_ArraySize indx, ZObject *bval);
ZTypeN_Byte * ZByteArray_GetItem(ZObject *o, ZTypeN_ArraySize indx);

// May implement in later versions
//ZTypeN_Byte * ZByteArray_PopItem(ZObject *o);
//void ZByteArray_PushItem(ZObject *o, ZObject *bval);

void _ZByteArray_SetRaw(ZObject *o, ZTypeN_ArraySize indx, ZTypeN_SignedByte bval);

#endif /* end of include guard: _Z_BYTEARRAYOBJECT_H */
