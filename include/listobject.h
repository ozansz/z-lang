#ifndef _Z_LISTOBJECT_H
#define _Z_LISTOBJECT_H

#include "object.h"

typedef struct _ZListNodeObj {
    ZObject_HEADER
    ZObject *ob;
    struct _ZListNodeObj *next;
    struct _ZListNodeObj *prev;
} ZListNode;

typedef struct _ZListObj {
    ZObject_HEADER
    ZListNode *root;
    ZListNode *__last_node;
    ZTypeN_ArraySize size;
} ZList;

ZObject *ZListNode_Create(void);
void ZListNode_Forget(ZObject *o);
void ZListNode_ForgetRecursive(ZObject *o);
void ZListNode_AssignObject(ZObject *node, ZObject *o);

ZObject *ZList_Create(void);
void ZList_Forget(ZObject *o);
ZTypeN_ArraySize ZList_Push(ZObject *list, ZObject *o);
ZObject *ZList_Pop(ZObject *list);
ZObject *ZList_Concat(ZObject *list_left, ZObject *list_right);

#endif /* end of include guard: _Z_LISTOBJECT_H */