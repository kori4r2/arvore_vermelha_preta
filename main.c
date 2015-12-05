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
		// Se não for a primeira execução, coloca uma quebra de linha antes de realizar a operação
		if(start && op != EOF) printf("\n");
		start = 1;
		// De acordo com a operação selecionada, chamas as funções correspondentes
		if(op == 'I'){
			scanf("%d", &id);
			insert_item(tree, create_item(id, id));
		}
		if(op == 'R'){
			scanf("%d", &id);
			remove_item(tree, id);
		}
		if(op == 'A'){
			print_tabs(tree);
		}
		// Se for realizada uma operação, imprime a árvore
		if(op != EOF) print_nested(tree);
	}while(op != EOF);

	// Libera a memória alocada
	delete_tree(&tree);

	return 0;
}
