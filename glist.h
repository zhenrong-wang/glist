/**
 * Copyright (C) 2024-present Zhenrong WANG, zhenrongwang@live.com
 * This code is distributed under the license: MIT License
 */

/* This is a minimal experimental demo of Generic Linked-List in C using MACROs. */

#ifndef GLIST_H
#define GLIST_H

#include <stdio.h>
#include <stdlib.h>

/* The index list. */
struct glist {
    struct glist *next;
};

#define UNKNOWN_ERR     255
#define MEM_ALLOC_ERR   127
#define NULL_HEAD_ERR   126
#define NULL_NODE_ERR   125
#define POSITION_OOB    124
#define EMPTY_LIST_WARN 123
#define SUCCESS_CALL    0

/* Create a new node. */
#define GL_NEW_NODE(type, p_err) ({ \
        struct glist *p_node = (struct glist *)calloc(1, sizeof(struct glist) + sizeof(type)); \
        if (p_node == NULL) { *p_err = MEM_ALLOC_ERR; } \
        else { *p_err = SUCCESS_CALL; } \
        p_node; \
        })

/* Get node data type, return a type pointer. */
#define GL_GET_NODE_TYPE(p_node, type) ((type *)((char *)(p_node) + sizeof(struct glist)))

#define GL_PRINT_GENERAL_INFO(summary_msg) \
        printf("\n[i] %s", summary_msg);

#define GL_PRINT_NULL_HEAD_ERR(p_err) \
        { printf("[x] fatal_err: null head.\n"); *p_err = NULL_HEAD_ERR; }

#define GL_PRINT_NULL_P_HEAD_ERR(p_err) \
        { printf("[x] fatal_err: null p_head.\n"); *p_err = NULL_HEAD_ERR; }

#define GL_PRINT_NULL_NODE_ERR(p_err) \
        { printf("[x] fatal_err: null node.\n"); *p_err = NULL_NODE_ERR; }

#define GL_PRINT_OOB_ERR(position, max_idx, p_err) \
        { printf("[x] fatal_err: pos_idx: %d, max_idx %lu, total_elem %lu.\n", \
        position, max_idx, max_idx + 1); *p_err = POSITION_OOB; }

#define GL_PRINT_EMPTY_WARN(p_err) \
        { printf("[!] warn_info: null head, empty list.\n"); *p_err = EMPTY_LIST_WARN; }

/* Append a node to an existed list. */
#define GL_APPEND_NODE(p_head, p_node, type, p_err) \
        if (p_head == NULL) GL_PRINT_NULL_HEAD_ERR(p_err) \
        else if (p_node == NULL) GL_PRINT_NULL_NODE_ERR(p_err) \
        else { struct glist *tmp = p_head; \
            do { \
                if (tmp->next == NULL) { tmp->next = p_node; *p_err = SUCCESS_CALL; break; } \
                tmp = tmp->next; \
            } while (1); \
        }

/* Erase the whole linked list. */
#define GL_DESTROY_LIST(pp_head, type, p_err) \
        if (pp_head == NULL) GL_PRINT_NULL_P_HEAD_ERR(p_err) \
        else if (*pp_head == NULL) GL_PRINT_EMPTY_WARN(p_err) \
        else { struct glist *tmp = *pp_head, *tmp_next; \
            do { \
                if (tmp == NULL) { break; } tmp_next = tmp->next; free(tmp); tmp = tmp_next; \
            } while (1); *pp_head = NULL; \
        }

/**
 * Generic print function. You need to implement a type-specific 
 * print function with format:
 * 
 * void print_type(type *p, size_t index) {
 *     ...
 * }
 * 
 * Example:
 *  void print_point (struct point *point_ptr, size_t position) {
 *      if(position == 0) {
 *          printf("[i] all the list nodes:\n");
 *      }
 *      printf(" +- Point %lu (x, y):\t (%lf, %lf)\n", position, point_ptr->x, point_ptr->y);
 *  }
 * 
 */
#define GENERIC_PRINT(node, node_idx, type, p_print_func) \
        p_print_func(GL_GET_NODE_TYPE(node, type), node_idx);

#define GL_PRINT_ELEMENT(p_head, position, type, p_print_func, p_err) \
        if (p_head == NULL) GL_PRINT_EMPTY_WARN(p_err) \
        else { \
            struct glist *tmp = p_head; size_t i = 0; \
            do { \
                if (i == position) { \
                    GENERIC_PRINT(tmp, i, type, p_print_func); \
                    *p_err = SUCCESS_CALL; break; \
                } \
                if (tmp->next == NULL) { \
                    GL_PRINT_OOB_ERR(position, i, p_err); break; \
                } \
                tmp = tmp->next; i++; \
            } while(1); \
        }

#define GL_PRINT_ALL_NODES(p_head, type, p_print_func, p_err) \
        if (p_head == NULL) GL_PRINT_EMPTY_WARN(p_err) \
        else { struct glist *tmp = p_head; size_t i = 0; \
            do { \
                GENERIC_PRINT(tmp, i, type, p_print_func); \
                i++; tmp = tmp->next; \
            } while(tmp != NULL); *p_err = 0; \
        }

#define GL_GET_NODE_NUM(p_head) ({ \
        struct glist *tmp = p_head; size_t i = 0; \
        do { \
            if (tmp == NULL) { break; } \
            tmp = tmp->next; i++; \
        } while(1); \
        i; })

#define GL_INSERT_NODE(pp_head, position, p_node, type, p_err) ({ \
        size_t node_num = GL_GET_NODE_NUM(*pp_head); \
        if (position < 0 || position >= node_num) { GL_PRINT_OOB_ERR(position, node_num - 1, p_err); } \
        else if (position == 0) { \
            struct glist *tmp = *pp_head; *pp_head = p_node; p_node->next = tmp; *p_err = SUCCESS_CALL; \
        } \
        else { size_t i = 1; struct glist *tmp_prev = *pp_head, *tmp_this = tmp_prev->next; \
            do { \
                if (i == position) { \
                    tmp_prev->next = p_node; p_node->next = tmp_this; *p_err = SUCCESS_CALL; break; \
                } \
                i++; tmp_prev = tmp_prev->next; tmp_this = tmp_prev->next; \
            } while (1); \
        } \
        })

#define GL_DELETE_NODE(pp_head, position, type, p_err) ({ \
        size_t node_num = GL_GET_NODE_NUM(*pp_head); \
        if (position < 0 || position >= node_num) { GL_PRINT_OOB_ERR(position, node_num - 1, p_err); } \
        else if (position == 0) { \
            struct glist *tmp = *pp_head; *pp_head = *pp_head->next; free(tmp); *p_err = SUCCESS_CALL; \
        } \
        else { size_t i = 1; struct glist *tmp_prev = *pp_head, *tmp_this = tmp_prev->next; \
            do { \
                if (i == position) { \
                    tmp_prev->next = tmp_this->next; free(tmp_this); *p_err = SUCCESS_CALL; break; \
                } \
                i++; tmp_prev = tmp_prev->next; tmp_this = tmp_prev->next; \
            } while (1); \
        } \
        })


#endif