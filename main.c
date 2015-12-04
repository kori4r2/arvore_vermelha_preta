// Ítalo Tobler Silva		 nUSP - 8551910

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "red_black_tree.h"

int main(int argc, char *argv[]){

	int id;
	TREE *tree = create_tree();
	char op, start = 0;

	do{
		// Le qual a operação a ser feita
		op = fgetc(stdin);
		// Evita que leia lixo da entrada
		while(op != EOF && !isalnum(op)) op = fgetc(stdin);
		if(start && op != EOF) printf("\n");
		start = 1;
		// De acordo com a operação selecionada, chamas as funções correspondentes e imprime o que for necessário
		if(op == 'I'){
			scanf("%d", &id);
			insert_item(tree, create_item(id, id));
		}
		if(op == 'R'){
			scanf("%d", &id);
			remove_item(tree, id);
//			if(result < INT_MAX && result > INT_MIN) printf("%d \n", result);
		}
		if(op == 'A'){
			print_tabs(tree);
		}
		if(op != EOF) print_nested(tree);
	}while(op != EOF);
//	printf("\n");

	// Libera a memória alocada
	delete_tree(&tree);

	return 0;
}
