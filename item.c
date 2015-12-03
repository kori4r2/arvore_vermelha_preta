#include "item.h"

ITEM *create_item(int id, int value){
	ITEM *new_item = malloc(sizeof(ITEM));
	if(new_item != NULL){
		new_item->value = value;
		new_item->id = id;
	}

	return new_item;
}

int compare_items(ITEM *item1, ITEM *item2){
	if(item1 != NULL && item2 != NULL){
		if(item1->id == item2->id) return 0;
		if(item1->id < item2->id) return -1;
		else return 1;
	}
	return INT_MIN;
}

int swap_items(ITEM **item1, ITEM **item2){
	if(item1 != NULL && *item1 != NULL  && item2 != NULL && *item2 != NULL){
		ITEM *aux = *item1;
		*item1 = *item2;
		*item2 = aux;
		return 1;
	}
	return 0;
}

int delete_item(ITEM **item){
	if(item != NULL){
		if((*item) != NULL){
			free(*item);
			(*item) = NULL;
			return 0;
		}
		return 1;
	}
	return 2;
}

void print_item(ITEM *item){
	if(item != NULL){
		printf("id = %d , value = %d\n", item->id, item->value);
	}
}
