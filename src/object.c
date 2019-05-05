#include <stdlib.h>

#include "object.h"

ZObject *ZObject_Create(ZObjectTypeC type) {
  ZObject *o = (ZObject *) malloc(sizeof(ZObject));
  o->ob_type = type;

  return o;
}

void ZObject_Forget(ZObject *o) {
  if (o == NULL)
    return;

  free(o);
}

ZTypeN_Addr ZObject_GetID(ZObject *o) {
  if (o == NULL)
    return 0;

  return (ZTypeN_Addr)o;
}
