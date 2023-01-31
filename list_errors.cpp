#include <stdio.h>
#include "list_errors.h"

void processor_of_errors(errors error, const char* fileName, const char* functionName, const int line) {
    switch (error) {
    case CALLOC_RETURNED_NULL:
        printf("In file %s in function %s on line %d: calloc returned NULL can`t give memory\n", fileName, functionName, line);
        break;

    case NULLPTR_GIVEN:
        printf("In file %s in function %s on line %d: NULL was given as an argument\n", fileName, functionName, line);
        break;

    case EMPTY_LIST:
        printf("In file %s in function %s on line %d: trying to pop from empty list\n", fileName, functionName, line);
        break;

    case WRONG_INDEX:
        printf("In file %s in function %s on line %d: given wrong index of element in list\n", fileName, functionName, line);

    case WRONG_CAPACITY:
        printf("In file %s in function %s on line %d: given wrong capacity of list\n", fileName, functionName, line);
    
    default:
        return;
    }
}