#include <stdlib.h>

#include "bytearrayobject.h"

ZObject * ZByteArray_New(ZTypeN_ArraySize ob_sz) {
  ZByteArrayObject_Flags ob_fl = {
    _true,
    _false
  };

  ZByteObject **stack = (ZByteObject **) malloc(ob_sz*sizeof(ZByteObject));

  if (NULL == stack) {
    return NULL;
  }

  for (ZTypeN_ArraySize i = 0; i < ob_sz; i++) {
    ZObject *tmp_obj = ZByte_FromByte(0);

    if (NULL == tmp_obj) {
      return NULL;
    }

    stack[i] = (ZByteObject *)tmp_obj;
  }

  ZByteArrayObject *o = (ZByteArrayObject *) malloc(sizeof(ZByteArrayObject));

  if (NULL == o) {
    return NULL;
  }

  o->ob_type = ZT_ByteArray;
  o->ob_stack = stack;
  o->ob_size = ob_sz;
  o->ob_flags = ob_fl;

  ZByteArray_FillRaw((ZObject *)o, (ZTypeN_Byte)0);

  return (ZObject *)o;
}

ZObject * ZByteArray_FromByte(ZTypeN_ArraySize ob_sz, ZObject *o) {
  if (o->ob_type != ZT_Byte) {
    return NULL;
  }

  ZObject *ba = ZByteArray_New(ob_sz);

  if (NULL == ba) {
    return NULL;
  }

  ZByteArray_FillWith(ba, o);

  return ba;
}

ZObject * ZByteArray_FromByteArray(ZObject *o) {
  if (o->ob_type != ZT_ByteArray) {
    return NULL;
  }

  ZObject *ba = ZByteArray_New(((ZByteArrayObject *)o)->ob_size);

  if (NULL == ba) {
    return NULL;
  }

  for (ZTypeN_ArraySize i = 0; i < ((ZByteArrayObject *)o)->ob_size; i++) {
    _ZByteArray_SetRaw(ba, i, ((ZByteArrayObject *)o)->ob_stack[i]->ob_val);
  }

  return ba;
}

void _ZByteArray_SetRaw(ZObject *o, ZTypeN_ArraySize indx, ZTypeN_SignedByte bval) {
  if (indx >= ((ZByteArrayObject *)o)->ob_size) {
    return;
  } else if (indx == ((ZByteArrayObject *)o)->ob_size) {
    ((ZByteArrayObject *)o)->ob_flags.full = _true;
    ((ZByteArrayObject *)o)->ob_flags.empty = _false;
  }

  ZByte_SetValue((ZObject *)((ZByteArrayObject *)o)->ob_stack[indx], bval);
}

void ZByteArray_FillWith(ZObject *o, ZObject *fw) {
  if ((o->ob_type != ZT_ByteArray) || (fw->ob_type != ZT_Byte)) {
    return;
  }

  for (ZTypeN_ArraySize i = 0; i < ((ZByteArrayObject *)o)->ob_size; i++) {
    _ZByteArray_SetRaw(o, i, ((ZByteObject *)fw)->ob_val);
  }
}

void ZByteArray_FillRaw(ZObject *o, ZTypeN_Byte fw) {
  if (o->ob_type != ZT_ByteArray) {
    return;
  }

  for (ZTypeN_ArraySize i = 0; i < ((ZByteArrayObject *)o)->ob_size; i++) {
    _ZByteArray_SetRaw(o, i, fw);
  }
}

void ZByteArray_SetItem(ZObject *o, ZTypeN_ArraySize indx, ZObject *bval) {
  if ((o->ob_type != ZT_ByteArray) || (bval->ob_type != ZT_Byte)) {
    return;
  }

  _ZByteArray_SetRaw(o, indx, ((ZByteObject *)bval)->ob_val);
}

ZTypeN_Byte * ZByteArray_GetItem(ZObject *o, ZTypeN_ArraySize indx) {
  if (o->ob_type != ZT_ByteArray) {
    return NULL;
  }

  ZTypeN_Byte *val = (ZTypeN_Byte *) malloc(sizeof(ZTypeN_Byte));
  *val = *ZByte_GetValue((ZObject *)((ZByteArrayObject *)o)->ob_stack[indx]);

  return val;
}

void
ZByteArray_FillWithBA(ZObject *o, ZObject *src, ZTypeN_ArraySize start_indx) {
  if ((o->ob_type != ZT_ByteArray) || (src->ob_type != ZT_ByteArray)) {
    return;
  }

  ZTypeN_ArraySize i, j = 0;

  for (i = start_indx; (i < ((ZByteArrayObject *)o)->ob_size) && (j < ((ZByteArrayObject *)src)->ob_size); i++, j++) {
    _ZByteArray_SetRaw(o, i, ((ZByteArrayObject *)src)->ob_stack[j]->ob_val);
  }
}

ZObject * ZByteArray_Concat(ZObject *o, ZObject *o_other) {
  if ((o->ob_type != ZT_ByteArray) || (o_other->ob_type != ZT_ByteArray)) {
    return NULL;
  }

  ZObject *new_ba = ZByteArray_New(((ZByteArrayObject *)o)->ob_size + ((ZByteArrayObject *)o_other)->ob_size);

  ZByteArray_FillWithBA(new_ba, o, 0);
  ZByteArray_FillWithBA(new_ba, o_other, ((ZByteArrayObject *)o)->ob_size);

  return new_ba;
}
