#include <stdlib.h>
#include "list_functions.h"
#include "list_errors.h"

static const int poison = 0XDEAD;

#define PRINT_ERROR(error) processor_of_errors(error, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define CHECK_NULLPTR(pointer, error, ...) if (pointer == NULL) { PRINT_ERROR(error); __VA_ARGS__; }

struct list* list_constructor(const int capacity) {
    if (capacity <= 0) {
        PRINT_ERROR(WRONG_CAPACITY);
        return NULL;
    }

    struct list* lst = (struct list*)calloc(1, sizeof(list));
    CHECK_NULLPTR(lst, CALLOC_RETURNED_NULL, return NULL);

    lst->data = (elem_t*)calloc(capacity, sizeof(elem_t));
    CHECK_NULLPTR(lst->data, CALLOC_RETURNED_NULL, return NULL);

    lst->next = (int*)calloc(capacity, sizeof(int));
    CHECK_NULLPTR(lst->next, CALLOC_RETURNED_NULL, return NULL);

    lst->prev = (int*)calloc(capacity, sizeof(int));
    CHECK_NULLPTR(lst->prev, CALLOC_RETURNED_NULL, return NULL);

    lst->head = 1;
    lst->tail = 1;
    lst->empty_start = 1;
    lst->empty_end = capacity - 1;
    lst->capacity = capacity;

    lst->data[0] = poison;
    lst->next[0] = 0;
    lst->prev[0] = 0;

    for (int index = 1; index < capacity - 1; ++index) {
        lst->data[index] = poison;
        lst->next[index] = index + 1;
        lst->prev[index] = -1;
    }

    lst->data[capacity - 1] = poison;
    lst->prev[capacity - 1] = -1;
    lst->next[capacity - 1] = -1;

    return lst;
}

int insert_after(struct list* lst, const int index, const elem_t data) {
    CHECK_NULLPTR(lst, NULLPTR_GIVEN, return NULLPTR_GIVEN);

    if (index + 1 <= 0) {
        PRINT_ERROR(WRONG_INDEX);
        return WRONG_INDEX;
    }

    if (lst->empty_start == lst->empty_end) {
        lst->data = (elem_t*)realloc(lst->data, lst->capacity * 2 * sizeof(elem_t));
        CHECK_NULLPTR(lst->data, CALLOC_RETURNED_NULL, return CALLOC_RETURNED_NULL);

        lst->next = (int*)realloc(lst->next, lst->capacity * 2 * sizeof(int));
        CHECK_NULLPTR(lst->next, CALLOC_RETURNED_NULL, return CALLOC_RETURNED_NULL);

        lst->prev = (int*)realloc(lst->prev, lst->capacity * 2 * sizeof(int));
        CHECK_NULLPTR(lst->prev, CALLOC_RETURNED_NULL, return CALLOC_RETURNED_NULL);
        
        lst->capacity *= 2;

        for (int index = lst->empty_start + 1; index < lst->capacity - 1; ++index) {
            lst->data[index] = poison;
            lst->next[index] = index + 1;
            lst->prev[index] = -1;
        }

        lst->data[lst->capacity - 1] = poison;
        lst->prev[lst->capacity - 1] = -1;
        lst->next[lst->capacity - 1] = -1;

        lst->empty_end = lst->capacity - 1; 
    }

    if (index + 1 >= lst->empty_start) { // push_back
        lst->data[lst->empty_start] = data;
        lst->next[lst->empty_start] = 0;  
        lst->prev[lst->empty_start] = lst->next[lst->empty_start - 1];

        lst->next[lst->next[lst->empty_start - 1]] = lst->empty_start;

    } else if (index + 1 == 1) { //push_front
        lst->data[lst->empty_start] = data;
        lst->next[lst->empty_start] = lst->prev[lst->empty_start - 1];
        lst->prev[lst->prev[lst->empty_start - 1]] = lst->empty_start;

        lst->prev[lst->empty_start] = 0; 

    } else { // insert_after 
        lst->data[lst->empty_start] = data;
        lst->next[lst->empty_start] = lst->next[index];
        lst->prev[lst->empty_start] = lst->prev[lst->next[index]];

        lst->next[lst->prev[index + 1]] = lst->empty_start;
        lst->prev[lst->next[index]] = lst->empty_start;
    }

    ++lst->tail;
    ++lst->empty_start;

    return NO_ERRORS;
}