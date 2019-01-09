#include <stdio.h>

#include "zlang.h"

int main(int argc, char const *argv[]) {
  ZObject *num = ZByte_FromByte(5);

  printf("obj addr: %p\n", num);
  printf("obj id  : %lu\n", ZObject_GetID(num));
  printf("obj type raw: %d\n", num->ob_type);
  printf("obj type: %d\n", ZObject_GetType(num));
  printf("byte val: %d\n", *ZByte_GetValue(num));
  printf("zero flag: %d\n", *ZByte_IsZero(num));
  printf("neg flag: %d\n", *ZByte_IsNegative(num));

  ZByte_SetValue(num, -100);

  printf("\nAfter set to -100:\n");
  printf("obj addr: %p\n", num);
  printf("obj id  : %lu\n", ZObject_GetID(num));
  printf("obj type raw: %d\n", num->ob_type);
  printf("obj type: %d\n", ZObject_GetType(num));
  printf("byte val: %d\n", *ZByte_GetValue(num));
  printf("zero flag: %d\n", *ZByte_IsZero(num));
  printf("neg flag: %d\n", *ZByte_IsNegative(num));

  return 0;
}
