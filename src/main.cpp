#include <stdio.h>
#include "list_functions.h"

int main() {
    struct list* lst = list_constructor(8);
    insert_after(lst, 0, 1);
    insert_after(lst, 7, 2);
    insert_after(lst, 1, 228);
    insert_after(lst, 0, -555);
    list_destructor(lst);

    return 0;
}