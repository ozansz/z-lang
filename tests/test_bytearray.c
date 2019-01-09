#include <stdio.h>
#include <stdlib.h>

#include "zlang.h"

int main(int argc, char const *argv[]) {
  ZObject *bnum = ZByte_FromByte(119);
  ZObject *barr = ZByteArray_New(10);

  ZByteArray_FillWith(barr, bnum);

  ZByte_SetValue(bnum, -4);
  ZByteArray_SetItem(barr, 4, bnum);

  ZByte_SetValue(bnum, 0);
  ZByteArray_SetItem(barr, 9, bnum);

  printf("obj addr: %p\n", barr);
  printf("obj id: %lu\n", ZObject_GetID(barr));
  printf("obj type: %d\n", ZObject_GetType(barr));
  printf("arr size: %lu\n", ((ZByteArrayObject *)barr)->ob_size);

  __dump_array(barr, "barr");

  ZObject *barr2 = ZByteArray_New(4);
  ZByteArray_FillRaw(barr2, 3);

  printf("\nobj addr: %p\n", barr2);
  printf("obj id: %lu\n", ZObject_GetID(barr2));
  printf("obj type: %d\n", ZObject_GetType(barr2));
  printf("arr size: %lu\n", ((ZByteArrayObject *)barr2)->ob_size);

  __dump_array(barr2, "barr2");

  ZObject *bcat = ZByteArray_Concat(barr, barr2);

  printf("\n cat obj addr: %p\n", bcat);
  printf("cat obj id: %lu\n", ZObject_GetID(bcat));
  printf("cat obj type: %d\n", ZObject_GetType(bcat));
  printf("cat arr size: %lu\n", ((ZByteArrayObject *)bcat)->ob_size);

  __dump_array(bcat, "bcat");

  ZCore_Forget(bnum);
  ZCore_Forget(barr);
  ZCore_Forget(barr2);

  return 0;
}
