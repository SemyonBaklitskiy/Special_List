enum errors {
    NO_ERRORS            = 0,
    CALLOC_RETURNED_NULL = 1,
    NULLPTR_GIVEN        = 2,
    EMPTY_LIST           = 3,
    WRONG_INDEX          = 4,
    WRONG_CAPACITY       = 5,
};

void processor_of_errors(errors error, const char* fileName, const char* functionName, const int line);