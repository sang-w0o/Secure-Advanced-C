#ifndef _DOUBLYCIRCULARLINKEDLIST_H_
#define _DOUBLYCIRCULARLINKEDLIST_H_

#define list_entry(ptr, type, member)   \
    (type*)((char*)ptr - (long)&((type*)0)->member)

#define list_foreach(node, head) \
    for(node = (head)->next; node!=(head); node = node->next)

#define list_foreach_reverse(node, head) \
    for(node = (head)->prev; node != (head); node = node->prev)

#define LIST_HEAD(name) \
    Node name = {&(name), &(name)}


typedef struct Node {
    struct Node *next;
    struct Node *prev;
} Node;

inline static void insertNode(Node * newNode, Node * prev, Node * next) {
    newNode->next = next;
    newNode->prev = prev;
    prev->next = newNode;
    next->prev = newNode;
}

inline void listAdd(Node * head, Node * node) {
    insertNode(node, head->prev, head);
}

inline void listAddHead(Node *head, Node *node) {
    insertNode(node, head, head->next);
}

inline void listRemove(Node *node) {

    Node *prev = node->prev;
    Node *next = node->next;
    prev->next = next;
    next->prev = prev;
}
#endif