#ifndef _Z_TYPES_H
#define _Z_TYPES_H

#include <stdint.h>

typedef enum {
  _false = 0,
  _true,
  __illegal
} _ZBool;

typedef enum {
  ZT_None = 0,
  ZT_Byte,
  ZT_Long,
  ZT_ByteArray,
  ZT_ObjArray
} ZObjectTypeC;

typedef int16_t ZTypeN_SignedByte;
typedef uint8_t ZTypeN_Byte ;
typedef uint32_t ZTypeN_Long;
typedef ZTypeN_Byte ZTypeN_Char;
typedef uint64_t ZTypeN_Addr;

#endif /* end of include guard: _Z_TYPES_H */
