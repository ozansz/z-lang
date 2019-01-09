#include <stdlib.h>
#include <stdio.h>

#include "util.h"

#include "ztypes.h"
#include "byteobject.h"
#include "bytearrayobject.h"

void ZCore_Forget(ZObject *o) {
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
    case ZT_ByteArray:
      for (ZTypeN_ArraySize i = 0; i < ((ZByteArrayObject *)o)->ob_size; i++) {
        free((void* )((ZByteArrayObject *)o)->ob_stack[i]);
      }
      free((void *)((ZByteArrayObject *)o)->ob_stack);
      free((void *)o);
      break;
    case ZT_ObjArray:
      /// Will implement later
      break;
  }
}

void __dump_array(ZObject *o, char *arr_name) {
  if (o->ob_type != ZT_ByteArray) {
    printf("\n[DEBUG] Incompatible object type: %d\n", o->ob_type);
    return;
  }

  printf("\n[DEBUG] Array <%s>: ", arr_name);

  for (ZTypeN_ArraySize i = 0; i < ((ZByteArrayObject *)o)->ob_size; i++) {
    printf("%d ", *ZByteArray_GetItem(o, i));
  }

  printf("\n");
}
