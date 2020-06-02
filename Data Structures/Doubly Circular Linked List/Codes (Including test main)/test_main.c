#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DoublyCircularLinkedList.h"

typedef struct Person {
    char name[32];
    int age;
    Node list;
} Person;

void display(const Node *head) {
    if (head == NULL) {
        fprintf(stderr, "display(): argument is null\n");
        return;
    }

    system("cls");
    printf("[head]");
    Node *node;
    list_foreach(node, head) {
        Person *p = list_entry(node, Person, list);
        printf("<->[%s(%d)]", p->name, p->age);
    }
    printf("<->[head]");
    getchar();
}

void displayBackwardly(const Node *head) {
    if (head == NULL) {
        fprintf(stderr, "displayBackwardly(): argument is null\n");
        return;
    }

    system("cls");
    printf("[head]");
    Node *node;
    list_foreach_reverse(node, head) {
        Person *p = list_entry(node, Person, list);
        printf("<->[%s(%d)]", p->name, p->age);
    }
    printf("<->[head]");
    getchar();
}

int main(void) {
    Person people[4] = {
        {"eddy", 20}, { "pororo", 30}, { "petty", 40}, { "poby", 50}
    };

    LIST_HEAD(head);
    display(&head);
    for (int i = 0; i < 4; i++) {
        listAdd(&head, &(people + i)->list);
        display(&head);
    }

    Node *node;
    list_foreach(node, &head) {
        Person *p = list_entry(node, Person, list);
        if (strcmp(p->name, "pororo") == 0) {
            listRemove(node);
        }
    }

    display(&head);
    return 0;
}