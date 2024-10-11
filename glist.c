/**
 * Copyright (C) 2024-present Zhenrong WANG, zhenrongwang@live.com
 * This code is distributed under the license: MIT License
 */

/* This is a minimal experimental demo of Generic Linked-List in C using MACROs. */

#include <stdio.h>
#include <stdlib.h>

/* The index list. */
struct glist {
    struct glist *next;
};

/* Create a new node. */
#define GL_NEW_NODE(type) (calloc(1, sizeof(struct glist) + sizeof(type)))
/* Get node data type, return a type pointer. */
#define GL_GET_NODE_TYPE(node, type) ((type *)((char *)(node) + sizeof(struct glist)))
/* Append a node to an existed list. */
#define GL_APPEND_NODE(head, node, type, err) \
        *err = 127; \
        if (head == NULL) { printf("[x] fatal_err: null head.\n"); *err = 1; } \
        else if(node == NULL) { printf("[x] fatal_err: null node.\n"); *err = 3; } \
        else { struct glist *tmp = head; \
            do { \
                if(tmp->next == NULL) { tmp->next = node; *err = 0; break; } \
                tmp = tmp->next; \
            } while (1); \
            if(*err != 0) { printf("[x] fatal_err: unknown error.\n"); } \
        }
/* Erase the whole linked list. */
#define GL_DESTROY_LIST(p_head, type) \
        if (p_head == NULL) { printf("[x] fatal_err: null p_head.\n"); } \
        else if(*p_head == NULL) { printf("[!] warn_info: null head, empty list.\n"); } \
        else { struct glist *tmp = *p_head, *tmp_next; \
            do { \
                if(tmp == NULL) { break; } tmp_next = tmp->next; free(tmp); tmp = tmp_next; \
            } while (1); *p_head = NULL; \
        }

#define GL_PRINT_ELEMENT(head, position, type, err) \
        *err = 127; \
        if (head == NULL) { printf("[!] warn_info: null head, empty list.\n"); } \
        else { \
            struct glist *tmp = head; int i = 0; \
            do { \
                if (i == position) { \
                    GL_GET_NODE_TYPE(tmp, type)->print_elem_func(GL_GET_NODE_TYPE(tmp, type), i); \
                    *err = 0; break; \
                } \
                if (tmp->next == NULL) { \
                    printf("[x] fatal_err: pos_idx: %d, max_idx %d, total_elem %d.\n", \
                    position, i, i+1); *err = 1; break; \
                } \
                tmp = tmp->next; i++; \
            } while(1); \
        }

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
    if(point_ptr == NULL) {
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

    struct glist *ptr1 = GL_NEW_NODE(struct point);
    struct point *p1 = GL_GET_NODE_TYPE(ptr1, struct point);
    init_point_struct(p1);
    p1->x = 10;
    p1->y = 10;

    struct glist *ptr2 = GL_NEW_NODE(struct point);
    struct point *p2 = GL_GET_NODE_TYPE(ptr2, struct point);
    init_point_struct(p2);
    p2->x = 20;
    p2->y = 20;

    GL_APPEND_NODE(ptr1, ptr2, struct point, &err_flag);
    GL_PRINT_ELEMENT(ptr1, 0, struct point, &err_flag);
    GL_PRINT_ELEMENT(ptr1, 1, struct point, &err_flag);
    GL_PRINT_ELEMENT(ptr1, 2, struct point, &err_flag);

    GL_DESTROY_LIST(&ptr1, struct point);

    GL_PRINT_ELEMENT(ptr1, 0, struct point, &err_flag);
    GL_PRINT_ELEMENT(ptr1, 1, struct point, &err_flag);
    GL_PRINT_ELEMENT(ptr1, 2, struct point, &err_flag);
}

