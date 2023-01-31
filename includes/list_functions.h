typedef long unsigned int size_t;
typedef int elem_t;

struct node {
    int next;
    int prev;
    bool empty;
    elem_t data;
};

struct list {
    struct node* nodes; 
    size_t capacity;
    //int head;
    //int tail;
    int empty_start;
    //int empty_end;
    bool sorted;
};

struct list* list_constructor(const int capacity);
int list_destructor(struct list* lst);
int insert_after(struct list* lst, int index, const elem_t data);
int insert_before(struct list* lst, int index, const elem_t data);
int delete_node(struct list* lst, int index);
int get_physical_adress(const struct list* lst, const int logicalAdress);
int sort(struct list* lst);