#ifndef _ITEM_H_
#define _ITEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct item{
	int id;
	int value;
}ITEM;

ITEM *create_item(int, int);
int compare_items(ITEM*, ITEM*);
int swap_items(ITEM**, ITEM**);
int delete_item(ITEM**);
void print_item(ITEM*);

#endif
