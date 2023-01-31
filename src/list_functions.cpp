#include <stdlib.h>
#include "list_functions.h"
#include "list_errors.h"

static const int poison = 0XDEAD;

#define PRINT_ERROR(error) processor_of_errors(error, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define CHECK_NULLPTR(pointer, error, ...) if (pointer == NULL) { PRINT_ERROR(error); __VA_ARGS__; }
#define RESIZE(pointer) int retValue = resize(pointer); if (retValue != NO_ERRORS) return retValue;

static int resize(struct list* lst);

struct list* list_constructor(const int capacity) {
    if (capacity <= 0) {
        PRINT_ERROR(WRONG_CAPACITY);
        return NULL;
    }

    struct list* lst = (struct list*)calloc(1, sizeof(list));
    CHECK_NULLPTR(lst, CALLOC_RETURNED_NULL, return NULL);

    lst->nodes = (struct node*)calloc(capacity, sizeof(node));
    CHECK_NULLPTR(lst, CALLOC_RETURNED_NULL, return NULL);

    lst->empty_start = 1;
    lst->capacity = capacity;

    lst->nodes[0].data = poison;
    lst->nodes[0].next = 0;
    lst->nodes[0].prev = 0;
    lst->nodes[0].empty = true;
    lst->sorted = true;

    for (int index = 1; index < capacity - 1; ++index) {
        lst->nodes[index].data = poison;
        lst->nodes[index].next = index + 1;
        lst->nodes[index].prev = -1;
        lst->nodes[index].empty = true;
    }

    lst->nodes[capacity - 1].data = poison;
    lst->nodes[capacity - 1].next = -1;
    lst->nodes[capacity - 1].prev = -1;
    lst->nodes[capacity - 1].empty = true;

    return lst;
}

int list_destructor(struct list* lst) {
    CHECK_NULLPTR(lst, NULLPTR_GIVEN, return NULLPTR_GIVEN);

    lst->capacity = 0;
    lst->empty_start = 0;
    lst->sorted = false;
    free(lst->nodes);
    free(lst);

    return NO_ERRORS;
}

int insert_after(struct list* lst, int index, const elem_t data) {
    CHECK_NULLPTR(lst, NULLPTR_GIVEN, return NULLPTR_GIVEN);

    if (index + 1 <= 0) {
        PRINT_ERROR(WRONG_INDEX);
        return WRONG_INDEX;
    }

    if (lst->nodes[index].empty) 
        index = lst->nodes[0].prev;

    if (lst->empty_start == -1) {
        RESIZE(lst);
    }

    int insertTo = lst->empty_start;
    lst->empty_start = lst->nodes[lst->empty_start].next;

    lst->nodes[insertTo].prev = index;
    lst->nodes[insertTo].next = lst->nodes[index].next;
    lst->nodes[insertTo].data = data;
    lst->nodes[insertTo].empty = false;

    lst->nodes[lst->nodes[index].next].prev = insertTo;
    lst->nodes[index].next = insertTo;

    if (insertTo != index + 1)
        lst->sorted = false;

    return NO_ERRORS;
}

int insert_before(struct list* lst, int index, const elem_t data) {
    CHECK_NULLPTR(lst, NULLPTR_GIVEN, return NULLPTR_GIVEN);

    if (index - 1 <= 0) {
        PRINT_ERROR(WRONG_INDEX);
        return WRONG_INDEX;
    }

    return insert_after(lst, lst->nodes[index].prev, data);
}

int delete_node(struct list* lst, int index) {
    CHECK_NULLPTR(lst, NULLPTR_GIVEN, return NULLPTR_GIVEN);
    if (index <= 0) {
        PRINT_ERROR(WRONG_INDEX);
        return WRONG_INDEX;
    }

    if (lst->nodes[index].empty) {
        PRINT_ERROR(EMPTY_NODE);
        return EMPTY_NODE;
    }

    lst->nodes[lst->nodes[index].prev].next = lst->nodes[index].next;
    lst->nodes[lst->nodes[index].next].prev = lst->nodes[index].prev;

    lst->nodes[index].data = poison;
    lst->nodes[index].prev = 0;
    lst->nodes[index].next = lst->empty_start;
    lst->nodes[index].empty = true;
    lst->empty_start = index;

    if (index != lst->empty_start - 1)
       lst->sorted = false;

    return NO_ERRORS;
}

int get_physical_adress(const struct list* lst, const int logicalAdress) {
    CHECK_NULLPTR(lst, NULLPTR_GIVEN, return -1);

    if (logicalAdress <= 0) {
        PRINT_ERROR(WRONG_INDEX);
        return -1;
    }

    if (lst->sorted) {
        if (logicalAdress >= lst->empty_start) 
            return -1;

        return logicalAdress;
    }

    int result = 0;
    for (int counter = 0; counter < logicalAdress; ++counter)
        result = lst->nodes[result].next;

    return result;
}

int sort(struct list* lst) {
    CHECK_NULLPTR(lst, NULLPTR_GIVEN, return NULLPTR_GIVEN);

    struct node* tmpNodes = (struct node*)calloc(lst->capacity, sizeof(node));
    CHECK_NULLPTR(tmpNodes, CALLOC_RETURNED_NULL, return CALLOC_RETURNED_NULL);
    
    int listPointer = 0;
    size_t counter = 1;
    for (counter = 1; counter < lst->capacity; ++counter) {   
        listPointer = lst->nodes[listPointer].next;

        tmpNodes[counter] = lst->nodes[listPointer];
        tmpNodes[counter].prev = counter - 1;
        tmpNodes[counter].next = counter + 1;

        if (listPointer == 0)
            break;  
    }

    tmpNodes[counter - 1].next = 0;
    tmpNodes[0].next = 1;
    tmpNodes[0].prev = counter - 1;

    listPointer = lst->empty_start;
    lst->empty_start = counter;

    for (counter = counter; counter < lst->capacity; ++counter) {   
        tmpNodes[counter] = lst->nodes[listPointer];
        tmpNodes[counter].prev = -1;
        tmpNodes[counter].next = counter + 1;

        listPointer = lst->nodes[listPointer].next;

        if (listPointer == -1)
            break;
    }
    
    tmpNodes[counter].next = -1;

    free (lst->nodes);
    lst->nodes = tmpNodes;
    lst->sorted = true;

    return NO_ERRORS;
}

static int resize(struct list* lst) {
    lst->nodes = (node*)realloc(lst->nodes, lst->capacity * 2 * sizeof(node));
    CHECK_NULLPTR(lst->nodes, CALLOC_RETURNED_NULL, return CALLOC_RETURNED_NULL);
    
    lst->capacity *= 2;

    for (size_t index = lst->empty_start; index < lst->capacity - 1; ++index) {
        lst->nodes[index].data = poison;
        lst->nodes[index].next = index + 1;
        lst->nodes[index].prev = -1;
        lst->nodes[index].empty = true;
    }

    lst->nodes[lst->capacity - 1].data = poison;
    lst->nodes[lst->capacity - 1].next = -1;
    lst->nodes[lst->capacity - 1].prev = -1;
    lst->nodes[lst->capacity - 1].empty = true;

    return NO_ERRORS;
}