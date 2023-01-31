typedef long unsigned int size_t;
typedef int elem_t;

struct list {
    elem_t* data;
    int* next;
    int* prev;
    size_t capacity;
    int head;
    int tail;
    int empty_start;
    int empty_end;
};