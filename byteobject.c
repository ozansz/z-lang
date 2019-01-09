#include <stdlib.h>

#include "byteobject.h"

ZTypeN_Byte * ZByte_GetValue(ZObject* o) {
  if (o->ob_type != ZT_Byte) {
    return NULL;
  }

  ZTypeN_Byte *val = (ZTypeN_Byte *) malloc(sizeof(ZTypeN_Byte));
  *val = ((ZByteObject *)o)->ob_val;

  return val;
}

void ZByte_SetValue(ZObject* o, ZTypeN_SignedByte ob_val) {
  if (o->ob_type != ZT_Byte) {
    return;
  }

  if (ob_val == 0) {
    ((ZByteObject *)o)->ob_flags.zero = _true;
    ((ZByteObject *)o)->ob_flags.neg = _false;
  } else if (ob_val < 0) {
    ((ZByteObject *)o)->ob_flags.zero = _false;
    ((ZByteObject *)o)->ob_flags.neg = _true;
    ob_val = 0L - ob_val;
  } else {
    ((ZByteObject *)o)->ob_flags.zero = _false;
    ((ZByteObject *)o)->ob_flags.neg = _false;
  }

  ((ZByteObject *)o)->ob_val = ob_val;
}

ZObject * ZByte_New(void) {
  ZByteObject_Flags ob_fl = {
      _false,
      _true
  };

  ZByteObject *o = (ZByteObject *) malloc(sizeof(ZByteObject));

  if (NULL == o) {
    return NULL;
  }

  o->ob_type = ZT_Byte;
  o->ob_val = 0;
  o->ob_flags = ob_fl;

  return (ZObject *)o;
}

ZObject * ZByte_FromByte(ZTypeN_SignedByte ob_val) {
  ZObject *o = ZByte_New();

  if (NULL == o) {
    return NULL;
  }

  ZByte_SetValue(o, ob_val);

  return o;
}

ZObject * ZByte_FromByteObj(ZObject* o) {
  if (o->ob_type != ZT_Byte) {
    return NULL;
  }

  return ZByte_FromByte(((ZByteObject *)o)->ob_val);
}

_ZBool * ZByte_IsZero(ZObject* o) {
  if (o->ob_type != ZT_Byte) {
    return NULL;
  }

  _ZBool *ret = (_ZBool *) malloc(sizeof(_ZBool));
  *ret = ((ZByteObject *)o)->ob_flags.zero;

  return ret;
}

_ZBool * ZByte_IsNegative(ZObject* o) {
  if (o->ob_type != ZT_Byte) {
    return NULL;
  }

  _ZBool *ret = (_ZBool *) malloc(sizeof(_ZBool));
  *ret = ((ZByteObject *)o)->ob_flags.neg;

  return ret;
}
