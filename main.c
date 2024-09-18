#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    struct LinkedList* list = construct_linked_list(arr, size);
    if (list == NULL) {
        return handle_error("could not create linked list");
    }

    int errAddToList = add_to_linked_list(list, 10);
    if (errAddToList != 0) {
        free_linked_list(list);
        return handle_error("could not add to the linked list");
    }

    int target = 44;
    int replacement = 18;
    int inLinkedList = is_in_linked_list(list, target);
    if (inLinkedList == 1) {
        printf("%d is in the linked list! We are going to change it to %d\n", target, replacement);
        replace_in_linked_list(list, target, replacement);
    } else {
        printf("%d is not in the linked list\n", target);
    }

    list->for_each(list, add_one_to_linked_list);
    list->delete_in_list(list, 5);
    list->print_list(list);
    free_linked_list(list);
    return 0;
}