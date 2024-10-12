/**
 * Copyright (C) 2024-present Zhenrong WANG, zhenrongwang@live.com
 * This code is distributed under the license: MIT License
 */

#include <stdio.h>
#include "glist.h"

/* An example data type. */
struct point {
    float x;
    float y;
};

void print_point (struct point *point_ptr, size_t position) {
    if(position == 0) {
        printf("[i] all the list nodes:\n");
    }
    printf(" +- Point %lu (x, y):\t (%lf, %lf)\n", position, point_ptr->x, point_ptr->y);
}

int set_point (struct point *ptr, double a, double b) {
    if(ptr == NULL) {
        return -1;
    }
    ptr->x = a;
    ptr->y = b;
    return 0;
}

int main (int argc, char **argv) {
    int err_flag;
    /* Test the APPEND_NODE macro. It should print an error msg. */
    GL_APPEND_NODE(NULL, NULL, struct point, &err_flag);
    /* Test the DESTROY_LIST macro. It should print an error msg. */
    GL_DESTROY_LIST((struct glist **)NULL, struct point, &err_flag);
    printf("[i] node num: %lu\n", GL_GET_NODE_NUM(NULL));

    struct glist *ptr0 = GL_NEW_NODE(struct point, &err_flag);
    GL_PRINT_GENERAL_INFO("created the head node (node 0).\n");

    struct glist *head = ptr0;
    struct point *p0 = GL_GET_NODE_TYPE(ptr0, struct point);
    set_point(p0, 10, 10);

    struct glist *ptr1 = GL_NEW_NODE(struct point, &err_flag);
    GL_PRINT_GENERAL_INFO("created node 1.\n");
    struct point *p1 = GL_GET_NODE_TYPE(ptr1, struct point);
    set_point(p1, 20, 20);
    printf("[i] node num: %lu\n", GL_GET_NODE_NUM(head));

    GL_APPEND_NODE(head, ptr1, struct point, &err_flag);
    GL_PRINT_GENERAL_INFO("node 1 appended to the list.\n");
    printf("[i] node num: %lu\n", GL_GET_NODE_NUM(head));
    
    GL_PRINT_ALL_NODES(head, struct point, print_point, &err_flag);

    struct glist *ptr2 = GL_NEW_NODE(struct point, &err_flag);
    GL_PRINT_GENERAL_INFO("created node 2.\n");
    struct point *p2 = GL_GET_NODE_TYPE(ptr2, struct point);
    set_point(p2, 30, 30);

    GL_INSERT_NODE(&head, -1, ptr2, struct point, &err_flag);
    GL_INSERT_NODE(&head, 1, ptr2, struct point, &err_flag);
    GL_PRINT_GENERAL_INFO("node 2 inserted at position 1.\n");

    GL_PRINT_ALL_NODES(head, struct point, print_point, &err_flag);
    GL_DELETE_NODE(&head, 1, struct point, &err_flag);
    GL_PRINT_GENERAL_INFO("node 1 deleted.\n");
    GL_PRINT_ALL_NODES(head, struct point, print_point, &err_flag);

    GL_DELETE_NODE(&head, 1, struct point, &err_flag);
    GL_PRINT_GENERAL_INFO("node 1 deleted.\n");
    GL_DELETE_NODE(&head, 1, struct point, &err_flag);
    GL_PRINT_ALL_NODES(head, struct point, print_point, &err_flag);

    GL_DESTROY_LIST(&head, struct point, &err_flag);

    GL_PRINT_ALL_NODES(head, struct point, print_point, &err_flag);
}

