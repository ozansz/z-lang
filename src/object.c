#include "object.h"

ZTypeN_Addr ZObject_GetID(ZObject *o) {
  if (o == NULL)
    return 0;

  return (ZTypeN_Addr)o;
}
