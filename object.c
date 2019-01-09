#include "object.h"

ZTypeN_Addr ZObject_GetID(ZObject *o) {
  return (ZTypeN_Addr)o;
}

ZObjectTypeC ZObject_GetType(ZObject *o) {
  return o->ob_type;
}
