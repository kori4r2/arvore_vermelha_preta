// Ítalo Tobler Silva		 nUSP - 8551910

#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "item.h"

typedef struct tree TREE;

/*------------------------------------------------------------------------------------------------------------
   create_tree()
        Funcao que cria uma arvore vermelho-preta, alocando a memoria necessaria
        - Parametros
          void
        - Retorno
          TREE* : arvore vermelho-preta criada; NULL caso erro de alocação de memoria
*/
TREE* create_tree(void);

/*------------------------------------------------------------------------------------------------------------
   empty_tree()
        Funcao que retorna se a arvore esta vazia. Se for passado NULL considera arvore vazia
        - Parametros
          TREE* : ponteiro para a arvore
        - Retorno
          int : 1 - arvore vazia; 0 - arvore nao vazia;
*/
int empty_tree(TREE*);

/*------------------------------------------------------------------------------------------------------------
   insert_item()
        Funcao que insere um item na arvore vermelho-preta, rebalanceando como necessario
        - Parametros
          TREE* : ponteiro para a arvore
	  ITEM* : item a ser inserido
        - Retorno
          int : 1 - parametro invalido passado; 0 - operacao bem-sucedida;
*/
int insert_item(TREE*, ITEM*);

/*------------------------------------------------------------------------------------------------------------
   remove_item()
        Funcao que busca uma chave na arvore e remove o item com esa chave caso seja encontrado, rebalanceando
	a arvore como necessario
        - Parametros
          TREE* : ponteiro para a arvore
	  int : chave a ser removida
        - Retorno
          int : 1 - parametro invalido passado; 0 - operacao bem-sucedida;
*/
int remove_item(TREE*, int);

/*------------------------------------------------------------------------------------------------------------
   print_tabs()
        Funcao que imprime a arvore com a raiz a esquerda e os filhos a direita (filho a direita acima, filho 
	a direita abaixo) para facilitar a visualizacao
        - Parametros
          TREE* : ponteiro para a arvore
        - Retorno
          int : 1 - parametro invalido passado; 0 - operacao bem-sucedida;
*/
int print_tabs(TREE*);

/*------------------------------------------------------------------------------------------------------------
   print_nested()
        Funcao que imprime a arvore vermelho-preta no formato aninhado
        - Parametros
          TREE* : ponteiro para a arvore
        - Retorno
          int : 1 - parametro invalido passado; 0 - operacao bem-sucedida;
*/
int print_nested(TREE*);

/*------------------------------------------------------------------------------------------------------------
   delete_tree()
        Funcao que apaga uma arvore vermelho-preta, liberando toda a memoria alocada
        - Parametros
          TREE** : endereco do ponteiro para a arvore
        - Retorno
          int : 1 - parametro invalido passado; 0 - operacao bem-sucedida;
*/
int delete_tree(TREE**);

#endif
