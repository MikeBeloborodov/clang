#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

void for_each_linked_list(struct LinkedList* list, LinkedListCallback callback) {
    struct Node* current = list->head;

    while (current != NULL) {
        callback(current);
        current = current->next;
    }
}

void print_linked_list(struct LinkedList* list) {
    struct Node* current = list->head;

    while (current != NULL) {
        printf("Current node is: %d\n", current->value);
        current = current->next;
    }
}

int add_to_linked_list(struct LinkedList* list, int value) {
    struct Node* current = list->head;

    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (struct Node*)malloc(sizeof(struct Node));
    if (!current->next) {
        return 1;
    }
    current->next->value = value;
    current->next->next = NULL;
    return 0;
}

void delete_linked_list(struct LinkedList* list, int target) {
    struct Node* old = list->head;
    struct Node* next = old->next;

    if (old->value == target) {
        list->head = next;
        return;
    }

   while (next != NULL) {
        if (next->value == target) {
            struct Node* toFree = next;
            old->next = next->next;
            free(toFree);
            return;
        }
        old = next;
        next = next->next;
    }
}

int is_in_linked_list(struct LinkedList* list, int target) {
    struct Node* current = list->head;

    while (current != NULL) {
        if (current->value == target) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

void replace_in_linked_list(struct LinkedList* list, int target, int newValue) {
    struct Node* current = list->head;

    while (current != NULL) {
        if (current->value == target) {
            current->value = newValue;
            return;
        }
        current = current->next;
    }
}

struct LinkedList* construct_linked_list (int *arr, int size) {
    // initialize list
    struct LinkedList *list = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    if (!list) {
        return NULL;
    }

    // initialize the root
    list->head = (struct Node *)malloc(sizeof(struct Node));
    if (!list->head) {
        return NULL;
    }

    // current node
    struct Node* current = list->head;
    current->value = arr[0];
    
    for (int i = 1; i < size; i++) {
        struct Node* newNode = (struct Node *)malloc(sizeof(struct Node));
        if (!newNode) {
            free_linked_list(list);
            free(list);
            return NULL;
        }
        newNode->value = arr[i];
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
    }
    
    // init functions
    list->print_list = print_linked_list;
    list->for_each = for_each_linked_list;
    list->delete_in_list = delete_linked_list;

    return list;
}

void free_linked_list(struct LinkedList* list) {
    struct Node* current = list->head;

    while (current != NULL) {
        struct Node* nextNode = current->next;
        free(current);
        current = nextNode;
    }
}

void add_one_to_linked_list(struct Node* n) {
    n->value++;
}

void handle_error(char str[]) {
    printf("%s\n", str);
    exit(EXIT_FAILURE);
}
