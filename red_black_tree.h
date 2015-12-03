#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "item.h"

typedef struct tree TREE;

TREE* create_tree(void);
int empty_tree(TREE*);
int insert_item(TREE*, ITEM*);
int remove_item(TREE*, int);
int print_tabs(TREE*);
int print_nested(TREE*);
int delete_tree(TREE**);

#endif
