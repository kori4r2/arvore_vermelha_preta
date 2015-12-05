// Ítalo Tobler Silva		 nUSP - 8551910

#include "red_black_tree.h"

#define RED 0
#define BLACK 1

typedef struct node{
	ITEM *item;
	struct node *left_child;
	struct node *right_child;
	struct node *parent;
	unsigned char color;
	int black_height;
}NODE;

struct tree{
	struct node *root;
	struct node *nil;
};

// Função que aloca a memória e inicializa um nó da árvore
NODE *create_node(TREE *tree, ITEM *item, NODE *parent){
	NODE *node = NULL;
	if(item != NULL && tree != NULL){
		node = (NODE*)malloc(sizeof(NODE));
		if(node != NULL){
			node->item = item;
			// Os filhos do nó serao inicializados como o nó nulo da arvore
			node->left_child = tree->nil;
			node->right_child = tree->nil;
			node->parent = parent;
			node->black_height = 1;
			// E o nó criado será vermelho
			node->color = RED;
		}
	}
	return node;
}

// Função que deleta um nó e seta a variável com seu endereço para NULL
int delete_node(NODE **node){
	if(node != NULL && *node != NULL){
		delete_item(&(*node)->item);
		free(*node);
		*node = NULL;
		return 0;
	}
	return 1;
}

TREE *create_tree(void){
	TREE *tree = (TREE*)malloc(sizeof(TREE));
	if(tree != NULL){
		// A raiz da árvore é inicializada como NULL
		tree->root = NULL;
		// O nó nulo é criado
		tree->nil = (NODE*)malloc(sizeof(NODE));
		tree->nil->item = NULL;
		tree->nil->left_child = NULL;
		tree->nil->right_child = NULL;
		tree->nil->parent = NULL;
		tree->nil->black_height = 0;
		tree->nil->color = BLACK;
	}
	return tree;
}

int empty_tree(TREE *tree){
	return !(tree != NULL && tree->root != NULL);
}

// Função que retorna qual a altura preta de um nó da árvore contando com o próprio nó
int black_height(NODE *node){
	if(node != NULL){
		if(node->color == RED) return node->black_height;
		else return node->black_height + 1;
	}
	// Caso seja passado nulo para a função, retorna -1
	return -1;
}

// Função que realiza a rotação para a esquerda em um nó da árvore
NODE *rotate_left(NODE *a){
	NODE *b = a->right_child;
	a->right_child = b->left_child;
	b->left_child = a;
	if(a->right_child != NULL) a->right_child->parent = a;
	b->parent = a->parent;
	a->parent = b;

	// O nó que desce tem sua altura atualizada
	a->black_height = black_height(a->left_child);
	return b;
}

// Função que realiza a rotação para a direita em um nó da árvore
NODE *rotate_right(NODE *a){
	NODE *b = a->left_child;
	a->left_child = b->right_child;
	b->right_child = a;
	if(a->left_child != NULL) a->left_child->parent = a;
	b->parent = a->parent;
	a->parent = b;

	// O nó que desce tem sua altura atualizada
	a->black_height = black_height(a->left_child);
	return b;
}

// Funcao que checa a regra de que um no vermelho só pode ter 2 filhos pretos e corrige o problema caso exista
NODE *check_balance(TREE *tree, NODE *node){
	// Se for passado NULL, retorna NULL
	if(node == NULL) return node;
	// Se for passado um nó preto, retorna seu pai
	if(node->color == BLACK) return node->parent;

	NODE *parent = node->parent;
	// Se o nó pai for NULL, o nó atual é a raiz
	if(parent == NULL){
		// A raiz é atualizada e o nó é pintado de preto
		tree->root = node;
		node->color = BLACK;
		// Retorna NULL
		node = NULL;
	}else{
		NODE *grandparent = parent->parent;
		// Se o nó avo for diferente de NULL
		if(grandparent != NULL){
			// aux armazena o pai do nó avo
			NODE *aux = grandparent->parent;
			// direction armazena se o no avo e filho direito ou esquerdo de seu pai, caso o pai seja diferente de NULL
			int direction;
			if(aux != NULL) direction = (aux->left_child == grandparent)? 0 : 1;
			// Obtem o tio do no atual
			NODE *uncle = (grandparent->left_child == parent)? grandparent->right_child : grandparent->left_child;
			// Caso o no atual e o no pai sejam vermelhos
			if(node->color == parent->color){
				// Se o no tio for vermelho, apenas faz uma recoloração dos nós
				if(uncle->color == RED){
					parent->color = BLACK;
					uncle->color = BLACK;
					grandparent->color = RED;
				// Caso contrario
				}else{
					// Checa se o nó pai e filho esquerdo ou direito para saber qual o sentido da rotação
					// e qual ponteiro deve ser atualizado apos a rotacao
					if(parent == grandparent->right_child){
						// Se for necessario fazer rotacao dupla, rotaciona no pai
						if(node == parent->left_child){
							parent = rotate_right(parent);
							grandparent->right_child = parent;
						}
						// Independente de ser simples ou dupla, recolore os nos e rotaciona no avo
						grandparent->color = RED;
						parent->color = BLACK;
						grandparent = rotate_left(grandparent);
					}else{
						if(node == parent->right_child){
							parent = rotate_left(parent);
							grandparent->left_child = parent;
						}
						grandparent->color = RED;
						parent->color = BLACK;
						grandparent = rotate_right(grandparent);
					}
					// Checa o valor de aux e direction para saber qual ponteiro deve ser atualizado
					// com o resultado da rotação no avo
					if(aux == NULL) tree->root = grandparent;
					else if(direction == 0) aux->left_child = grandparent;
					else aux->right_child = grandparent;
				}
			}
		// Se o nó avô for NULL, o nó pai é a raiz
		}else{
			// A raiz é atualizada e o nó é pintado de preto
			tree->root = parent;
			parent->color = BLACK;
		}
		// Retorna o valor do avo no final das operacoes
		node = grandparent;
	}
	// Atualiza a altura preta do no a ser retornado, caso nao seja NULL
	if(node != NULL) node->black_height = black_height(node->left_child);
	return node;
}

// Função que insere um nó na árvore vermelho e preta buscando recursivamente a partir do primeiro nó passado
NODE *insert_node_down(NODE *node, ITEM *item, NODE *parent, TREE *tree, NODE **position){

	// Caso seja passado uma variável que contenha NULL, cria o nó e retorna o nó criado
	if(node == tree->nil || node == NULL){
		node = create_node(tree, item, parent);
		// Salva o endereço do novo nó criado para possibilitar a checagem de balanceamento da árvore
		*position = node;
	}else{
		// Se o nó atual tiver um item com chave maior do que o valor inserido, se chama recursivamente para a esquerda
		if(item->id < node->item->id){
			node->left_child = insert_node_down(node->left_child, item, node, tree, position);
		// Se o nó atual tiver um item com chave menor do que o valor inserido, se chama recursivamente para a direita
		}else if(item->id > node->item->id){
			node->right_child = insert_node_down(node->right_child, item, node, tree, position);
		// Caso seja um valor igual ao do nó atual, apaga o item criado
		}else{
			delete_item(&item);
		}
	}

	// Retorna o nó analisado
	return node;
}

int insert_item(TREE *tree, ITEM *item){
	// Checa se os parâmetros passados são válidos
	if(item != NULL && tree != NULL){
		// check_position armazena a posição de inserção, quando ela acontece
		NODE *check_position = NULL;
		// Caso seja, insere começando a busca a partir da raiz da árvore
		tree->root = insert_node_down(tree->root, item, NULL, tree, &check_position);
		// Enquanto for necessário, sobe a árvore checando o balanceamento
		while((check_position = check_balance(tree, check_position)) != NULL);
		return 0;
	}
	return 1;
}

// Funcao que sobe a arvore apos uma remocao rebalanceando em funcao da altura preta caso necessario
void balance_height(NODE *node, NODE *parent, TREE *tree){
	// Se for chamada na raiz, apenas pinta o nó de preto
	if(node == tree->root) node->color = BLACK;
	// Caso contrario, checa a a altura preta do nó pai para saber se é necessario rebalancear
	else if(node != NULL && parent != NULL && black_height(parent->right_child) != black_height(parent->left_child)){
		// Caso seja, obtem o nó irmao e o nó sobrinho oposto
		NODE *sibling = (parent->right_child == node)? parent->left_child : parent->right_child;
		NODE *nephew = (parent->right_child == node)? sibling->left_child : sibling->right_child;
		// Se o nó irmão for vermelho
		if(sibling->color == RED){
			// Checa se o nó irmão é filho direito ou esquerdo para saber o sentido da rotação a ser feita
			if(parent->right_child == sibling){
				// Checa qual ponteiro deve ser atualizado com o resultado da rotação no nó pai
				if(parent->parent == NULL) tree->root = rotate_left(parent);
				else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_left(parent);
				else parent->parent->left_child = rotate_left(parent);
			}else{
				if(parent->parent == NULL) tree->root = rotate_right(parent);
				else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_right(parent);
				else parent->parent->left_child = rotate_right(parent);
			}
			// Muda as cores dos nós
			sibling->color = parent->color;
			parent->color = RED;
			// Se chama recursivamente no mesmo nó
			balance_height(node, parent, tree);
		// Se o nó irmão for preto
		}else{
			// Se o nó sobrinho oposto for vermelho
			if(nephew->color == RED){
				// Checa se o nó irmão é filho direito ou esquerdo para saber o sentido da rotação a ser feita
				if(parent->right_child == sibling){
					// Checa qual ponteiro deve receber o resultado da rotação no nó pai
					if(parent->parent == NULL) tree->root = rotate_left(parent);
					else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_left(parent);
					else parent->parent->left_child = rotate_left(parent);
				}else{
					if(parent->parent == NULL) tree->root = rotate_right(parent);
					else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_right(parent);
					else parent->parent->left_child = rotate_right(parent);
				}
				// Recolore os nós de acordo com a necessidade
				sibling->color = parent->color;
				parent->color = BLACK;
				nephew->color = BLACK;
				// Atualiza as alturas pretas de acordo com a necessidade
				parent->black_height = black_height(parent->left_child);
				sibling->black_height = black_height(sibling->left_child);
				// Se chama recursivamente no nó pai
				balance_height(parent, parent->parent, tree);
			// Caso o sobrinho oposto seja preto
			}else{
				// Checa o outro sobrinho
				nephew = (sibling->right_child == nephew)? sibling->left_child : sibling->right_child;
				// Caso o outro sobrinho seja vermelho
				if(nephew->color == RED){
					// Recolore o irmão e o sobrinho
					sibling->color = RED;
					nephew->color = BLACK;
					// E rotaciona no irmão para que o sobrinho oposto fique sendo vermelho
					if(parent->right_child == sibling){
						parent->right_child = rotate_right(sibling);
					}else{
						parent->left_child = rotate_left(sibling);
					}
					// Se chama recursivamente no mesmo nó
					balance_height(node, parent, tree);
				// Caso o irmão e ambos os sobrinhos sejam pretos
				}else{
					// Recolore os nós
					sibling->color = RED;
					parent->color = BLACK;
					// Atualiza a altura do nó pai
					parent->black_height = black_height(sibling);
					// Se chama recursivamente no nó pai
					balance_height(parent, parent->parent, tree);
				}
			}
		}
	}
}

// Função auxiliar para remoção quando um nó tiver 2 filhos não nulos
void remove_aux(NODE *node, NODE *deleted, TREE *tree){
	// Equanto houver filho não nulo à esquerda, desce com recursão
	if(node->left_child == tree->nil){
		// Quando for encontrado o menor nó do lado direito, troca seu conteúdo com o conteúdo a ser apagado
		swap_items(&node->item, &deleted->item);
		// Atualiza as referencias para que ignorem o nó encontrado
		if(node->parent == deleted) deleted->right_child = node->right_child;
		else node->parent->left_child = node->right_child;
		if(node->right_child != tree->nil) node->right_child->parent = node->parent;
		// Se o nó encontrado for um nó preto
		if(node->color == BLACK){
			// Se o nó que subiu para substitui-lo for vermelho ,apenas pinta-o de preto
			if(node->right_child->color == RED) node->right_child->color = BLACK;
			// Caso contrario, checa o balanceamento da arvore pos-remoção
			else balance_height(node->right_child, node->parent, tree);
		}
		// Apaga o nó removido
		delete_node(&node);
	}else remove_aux(node->left_child, deleted, tree);
}

// Função que busca um item na árvore e remove o nó que o contém
void remove_down(NODE *node, int id, NODE *parent, TREE *tree){
	if(node != NULL && node != tree->nil){
		// Se o nó atual tiver um item com valor maior do que o valor inserido, se chama recursivamente para a esquerda
		if(id < node->item->id){
			remove_down(node->left_child, id, node, tree);
		// Se o nó atual tiver um item com valor menor do que o valor inserido, se chama recursivamente para a direita
		}else if(id > node->item->id){
			remove_down(node->right_child, id, node, tree);
		// Caso seja um valor igual ao do nó atual, realiza a remoção
		}else{
			// aux armazena o nó a ser apagado
			NODE *aux = node;
			// Se ambos os filhos do nó forem nulos, um nó nulo deverá subir para ocupar seu lugar
			if(node->left_child == tree->nil && node->right_child == tree->nil){
				node = tree->nil;
			// Caso apenas um filho seja não nulo, esse nó deve subir para ocupar o lugar do nó apagado
			}else if(node->left_child == tree->nil){
				node->right_child->parent = parent;
				node = node->right_child;
			}else if(node->right_child == tree->nil){
				node->left_child->parent = parent;
				node = node->left_child;
			// Caso tenha dois filhos não nulos chama a função auxiliar e muda aux para NULL
			}else{
				remove_aux(node->right_child, node, tree);
				aux = NULL;
			}
			// Caso a função auxiliar não tenha sido chamada
			if(aux != NULL){
				// Atualiza a referencia do nó pai para receber o nó que subiu
				if(parent != NULL){
					if(parent->right_child == aux) parent->right_child = node;
					else parent->left_child = node;
				}else tree->root = node;
				// Se o nó que foi apagado for preto
				if(aux->color == BLACK){
					// Se o nó que subiu for vermelho, apenas pinta-o de preto
					if(node->color == RED) node->color = BLACK;
					// Caso contrario, checa o balanceamento da arvore pos-remocao
					else balance_height(node, parent, tree);
				}
				// Apaga o nó removido
				delete_node(&aux);
			}
		}
	}
}

int remove_item(TREE *tree, int id){
	if(tree != NULL){
		// Chama a função recursiva na raiz
		remove_down(tree->root, id, NULL, tree);
		return 0;
	}
	return 1;
}

// Função que apaga todos os nós abaixo do primeiro nó chamado
void delete_node_down(NODE **node, TREE *tree){
	if(node != NULL && *node != NULL && *node != tree->nil){
		delete_node_down(&(*node)->left_child, tree);
		delete_node_down(&(*node)->right_child, tree);
		delete_node(node);
	}
}

int delete_tree(TREE **tree){
	if(tree != NULL && !empty_tree(*tree)){
		// Apaga todos os nós da raiz para baixo
		delete_node_down(&(*tree)->root, *tree);
		// Apaga o nó nulo
		delete_node(&(*tree)->nil);
		// Libera a memória alocada e muda o ponteiro para NULL
		free(*tree);
		*tree = NULL;
		return 0;
	}
	return 1;
}

void print_tabs_aux(NODE *node, int l, TREE *tree){
	if(node != NULL && node != tree->nil){
		int i;
		print_tabs_aux(node->right_child, l+1, tree);
		for(i = 0; i < l; i++) printf("     ");
		printf("%d-%c", node->item->id, (node->color == BLACK)? 'B' : 'R');
		printf("\n");
		print_tabs_aux(node->left_child, l+1, tree);
	}
}

int print_tabs(TREE *tree){
	if(!empty_tree(tree)){
		printf("---------------------------------------------------------\n");
		print_tabs_aux(tree->root, 0, tree);
		printf("---------------------------------------------------------\n");
		return 0;
	}
	return 1;
}

void print_nested_aux(NODE *node, TREE *tree){
	if(node != NULL && node != tree->nil){
		if(node->parent != NULL) printf(",");
		printf("{%d", node->item->id);
		print_nested_aux(node->left_child, tree);
		print_nested_aux(node->right_child, tree);
		printf("}");
	}
}

int print_nested(TREE *tree){
	if(!empty_tree(tree)){
		print_nested_aux(tree->root, tree);
		return 0;
	}
	return 1;
}
