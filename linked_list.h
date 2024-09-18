#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct Node {
    int value;
    struct Node* next;
};

typedef void (*LinkedListCallback)(struct Node* n);

struct LinkedList {
    struct Node* head;
    struct LinkedList* (*constructor)(int *arr, int size);
    void (*print_list)(struct LinkedList*);
    void (*for_each)(struct LinkedList*, LinkedListCallback);
    void (*delete_in_list)(struct LinkedList*, int target);
};

void free_linked_list(struct LinkedList* list);
void for_each_linked_list(struct LinkedList* list, LinkedListCallback callback);
void print_linked_list(struct LinkedList* list);
int add_to_linked_list(struct LinkedList* list, int value);
void delete_linked_list(struct LinkedList* list, int target);
int is_in_linked_list(struct LinkedList* list, int target);
void replace_in_linked_list(struct LinkedList* list, int target, int newValue);
struct LinkedList* construct_linked_list (int *arr, int size);
void add_one_to_linked_list(struct Node* n);
int handle_error(char str[]);

#endif // LINKED_LIST_H