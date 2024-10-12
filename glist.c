/**
 * Copyright (C) 2024-present Zhenrong WANG, zhenrongwang@live.com
 * This code is distributed under the license: MIT License
 */

#include <stdio.h>
#include "glist.h"


/**
 * An example data type. 
 * Convention: Any data type MUST wrap its functions into the struct 
 *             using function pointers.
 */
struct point {
    float x;
    float y;
    /* An example of data_type related function. */
    void (*print_elem_func)(struct point *, size_t position);
};

void print_point (struct point *point_ptr, size_t position) {
    printf("Point %lu (x, y):\t (%lf, %lf)\n", position, point_ptr->x, point_ptr->y);
}

int init_point_struct(struct point *point_ptr) {
    if (point_ptr == NULL) {
        return -1;
    }
    point_ptr->print_elem_func = print_point;
    return 0;
}

int main(int argc, char **argv) {
    int err_flag;
    /* Test the APPEND_NODE macro. It should print an error msg. */
    GL_APPEND_NODE(NULL, NULL, struct point, &err_flag);
    /* Test the DESTROY_LIST macro. It should print an error msg. */
    GL_DESTROY_LIST((struct glist **)NULL, struct point);
    printf("[i]: node num: %lu\n", GL_GET_NODE_NUM(NULL));

    struct glist *ptr1 = GL_NEW_NODE(struct point, &err_flag);
    struct point *p1 = GL_GET_NODE_TYPE(ptr1, struct point);
    init_point_struct(p1);
    p1->x = 10;
    p1->y = 10;

    struct glist *ptr2 = GL_NEW_NODE(struct point, &err_flag);
    struct point *p2 = GL_GET_NODE_TYPE(ptr2, struct point);
    init_point_struct(p2);
    p2->x = 20;
    p2->y = 20;
    printf("[i]: node num: %lu\n", GL_GET_NODE_NUM(ptr1));
    GL_APPEND_NODE(ptr1, ptr2, struct point, &err_flag);
    printf("[i]: node num: %lu\n", GL_GET_NODE_NUM(ptr1));
    
    GL_PRINT_ALL_NODES(ptr1, struct point, &err_flag);

    struct glist *ptr3 = GL_NEW_NODE(struct point, &err_flag);
    struct point *p3 = GL_GET_NODE_TYPE(ptr3, struct point);
    init_point_struct(p3);
    p3->x = 30;
    p3->y = 30;

    GL_INSERT_NODE(&ptr1, -1, ptr3, struct point, &err_flag);

    GL_INSERT_NODE(&ptr1, 1, ptr3, struct point, &err_flag);

    GL_PRINT_ALL_NODES(ptr1, struct point, &err_flag);
    GL_DELETE_NODE(&ptr1, 1, struct point, &err_flag);
    putchar('\n');
    GL_PRINT_ALL_NODES(ptr1, struct point, &err_flag);

    GL_DESTROY_LIST(&ptr1, struct point);

    GL_PRINT_ALL_NODES(ptr1, struct point, &err_flag);
}

