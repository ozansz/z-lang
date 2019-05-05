#include <stdlib.h>

#include "listobject.h"
#include "ztypes.h"
#include "object.h"

ZObject *ZListNode_Create(void) {
    ZListNode *node = (ZListNode *) malloc(sizeof(ZListNode));

    if (node == NULL)
        return NULL;

    node->ob_type = ZT_ListNode;
    node->ob = NULL;
    node->next = NULL;
    node->prev = NULL;

    return (ZObject *) node;
}

void ZListNode_Forget(ZObject *o) {
    if (o == NULL || o->ob_type != ZT_ListNode)
        return;

    free(o);
}

void ZListNode_ForgetRecursive(ZObject *o) {
    if (o == NULL || o->ob_type != ZT_ListNode)
        return;

    if (((ZListNode *) o)->next != NULL)
        ZListNode_ForgetRecursive(((ZListNode *) o)->next);

    ZListNode_Forget(o);
}

void ZListNode_AssignObject(ZObject *node, ZObject *o) {
    if (o == NULL || o->ob_type != ZT_ListNode)
        return;

    ((ZListNode *) o)->ob = o;
}

ZObject *ZList_Create(void) {
    ZList *list = (ZList *) malloc(sizeof(ZList));

    list->ob_type = ZT_List;
    list->size = 0;
    list->root = NULL;
    list->__last_node = NULL;

    return (ZObject *) list;
}

void ZList_Forget(ZObject *o) {
    if (o == NULL || o->ob_type != ZT_List)
        return;

    ZListNode_ForgetRecursive(((ZList *) o)->root);
    free(o);
}

ZTypeN_ArraySize ZList_Push(ZObject *list, ZObject *o) {
    ZObject *new_node;

    if (list == NULL || o == NULL || o->ob_type != ZT_List)
        return;

    new_node = ZListNode_Create();
    ZListNode_AssignObject(new_node, o);

    if (((ZList *) list)->root == NULL) {
        ((ZList *) list)->root = new_node;
        ((ZList *) list)->__last_node = new_node;
    } else {
        ((ZListNode *) new_node)->prev = ((ZList *) list)->__last_node;
        ((ZList *) list)->__last_node->next = new_node;
        ((ZList *) list)->__last_node = new_node;
    }

    ((ZList *) list)->size++;

    return ((ZList *) list)->size;
}

ZObject *ZList_Pop(ZObject *list) {
    ZObject *pop_node;

    if (list == NULL || list->ob_type != ZT_List)
        return;

    if (((ZList *) list)->size == 0)
        return;
    else if (((ZList *) list)->size == 1) {
        pop_node = ((ZList *) list)->root;
        ((ZList *) list)->root = NULL;
        ((ZList *) list)->__last_node = NULL;
    } else {
        pop_node = ((ZList *) list)->__last_node;
        ((ZList *) list)->__last_node = ((ZList *) list)->__last_node->prev;
    }

    ((ZList *) list)->size--;

    return (ZObject *) pop_node;
}

ZObject *ZList_Concat(ZObject *list_left, ZObject *list_right) {
    ZListNode *curr_node;
    
    if (list_left == NULL || list_left->ob_type != ZT_List || list_right == NULL || list_right->ob_type != ZT_List)
        return;

    if (((ZList *) list_right)->size == 0)
        return;

    curr_node = ((ZList *) list_right)->root;

    while (1) {
        ZList_Push(list_left, curr_node->ob);

        if (curr_node->next == NULL)
            break;
        else
            curr_node = curr_node->next;
    }

    return (ZObject *) list_left;
}