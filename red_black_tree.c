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
			node->left_child = tree->nil;
			node->right_child = tree->nil;
			node->parent = parent;
			node->black_height = 1;
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

// Função que aloca e inicializa uma árvore vermelha e preta
TREE *create_tree(void){
	TREE *tree = (TREE*)malloc(sizeof(TREE));
	if(tree != NULL){
		tree->root = NULL;
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

// Função que retorna se a árvore está vazia ou não (árvore inválida tem mesmo retorno de árvore vazia)
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

NODE *check_balance(TREE *tree, NODE *node){
	if(node == NULL) return node;
	if(node->color == BLACK) return node->parent;

	NODE *parent = node->parent;
	if(parent == NULL){
		tree->root = node;
		node->color = BLACK;
		node = NULL;
	}else{
		NODE *grandparent = parent->parent;
		if(grandparent != NULL){
			NODE *aux = grandparent->parent;
			int direction;
			if(aux != NULL) direction = (aux->left_child == grandparent)? 0 : 1;
			NODE *uncle = (grandparent->left_child == parent)? grandparent->right_child : grandparent->left_child;
			if(node->color == parent->color){
				if(uncle->color == RED){
					parent->color = BLACK;
					uncle->color = BLACK;
					grandparent->color = RED;
				}else{
					if(parent == grandparent->right_child){
						if(node == parent->left_child){
							parent = rotate_right(parent);
							grandparent->right_child = parent;
						}
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
					if(aux == NULL) tree->root = grandparent;
					else if(direction == 0) aux->left_child = grandparent;
					else aux->right_child = grandparent;
				}
			}
		}
		node = grandparent;
	}
	if(node != NULL) node->black_height = black_height(node->left_child);
	return node;
}

// Função que insere um nó na árvore vermelho e preta buscando recursivamente a partir do primeiro nó passado
NODE *insert_node_down(NODE *node, ITEM **item, NODE *parent, TREE *tree, NODE **position){

	// Caso seja passado uma variável que contenha NULL, cria o nó e retorna o nó criado
	if(node == tree->nil || node == NULL){
		node = create_node(tree, *item, parent);
		// Salva o endereço do novo nó criado para possibilitar a checagem de balanceamento da árvore
		*position = node;
	}else{
		// Se o nó atual tiver um item com chave maior do que o valor inserido, se chama recursivamente para a esquerda
		if((*item)->id < node->item->id){
			node->left_child = insert_node_down(node->left_child, item, node, tree, position);
		// Se o nó atual tiver um item com chave menor do que o valor inserido, se chama recursivamente para a direita
		}else if((*item)->id > node->item->id){
			node->right_child = insert_node_down(node->right_child, item, node, tree, position);
		// Caso seja um valor igual ao do nó atual, exibe a saída correspondente
		}else{
//			printf("Chave existente\n");
			delete_item(item);
		}
	}

	// Retorna o nó analisado
	return node;
}

// Função que insere um item na árvore avl
int insert_item(TREE *tree, ITEM *item){
	// Checa se os parâmetros passados são válidos
	if(item != NULL && tree != NULL){
		NODE *check_position = NULL;
		// Caso seja, insere começando a busca a partir da raiz da árvore
		tree->root = insert_node_down(tree->root, &item, NULL, tree, &check_position);
		while((check_position = check_balance(tree, check_position)) != NULL);
		return 0;
	}
	return 1;
}

void balance_height(NODE *node, NODE *parent, TREE *tree){
	if(node == tree->root) node->color = BLACK;
	else if(node != NULL && parent != NULL && black_height(parent->right_child) != black_height(parent->left_child)){
		NODE *sibling = (parent->right_child == node)? parent->left_child : parent->right_child;
		NODE *nephew = (parent->right_child == node)? sibling->left_child : sibling->right_child;
		if(sibling->color == RED){
			if(parent->right_child == sibling){
				if(parent->parent == NULL) tree->root = rotate_left(parent);
				else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_left(parent);
				else parent->parent->left_child = rotate_left(parent);
			}else{
				if(parent->parent == NULL) tree->root = rotate_right(parent);
				else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_right(parent);
				else parent->parent->left_child = rotate_right(parent);
			}
			sibling->color = parent->color;
			parent->color = RED;
			balance_height(node, parent, tree);
		}else{
			if(nephew->color == RED){
				if(parent->right_child == sibling){
					if(parent->parent == NULL) tree->root = rotate_left(parent);
					else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_left(parent);
					else parent->parent->left_child = rotate_left(parent);
				}else{
					if(parent->parent == NULL) tree->root = rotate_right(parent);
					else if(parent->parent->right_child == parent) parent->parent->right_child = rotate_right(parent);
					else parent->parent->left_child = rotate_right(parent);
				}
				sibling->color = parent->color;
				parent->color = BLACK;
				nephew->color = BLACK;
				parent->black_height = black_height(parent->left_child);
				sibling->black_height = black_height(sibling->left_child);
				balance_height(parent, parent->parent, tree);
			}else{
				nephew = (sibling->right_child == nephew)? sibling->left_child : sibling->right_child;
				if(nephew->color == RED){
					sibling->color = RED;
					nephew->color = BLACK;
					if(parent->right_child == sibling){
						parent->right_child = rotate_right(sibling);
					}else{
						parent->left_child = rotate_left(sibling);
					}
					balance_height(node, parent, tree);
				}else{
					sibling->color = RED;
					parent->color = BLACK;
					parent->black_height = black_height(sibling);
					balance_height(parent, parent->parent, tree);
				}
			}
		}
	}
}

void remove_aux(NODE *node, NODE *deleted, TREE *tree){
	if(node->left_child == tree->nil){
		swap_items(&node->item, &deleted->item);
		if(node->parent == deleted) deleted->right_child = node->right_child;
		else node->parent->left_child = node->right_child;
		if(node->right_child != tree->nil) node->right_child->parent = node->parent;
		if(node->color == BLACK){
			if(node->right_child->color == RED) node->right_child->color = BLACK;
			else{
				node->right_child->color = BLACK;
				balance_height(node->right_child, node->parent, tree);
			}
		}
		delete_node(&node);
	}else remove_aux(node->left_child, deleted, tree);
}

void remove_down(NODE *node, ITEM **item, NODE *parent, TREE *tree){
	// Caso seja passado uma variável que contenha NULL, exibe a saída correspondente
	if(node == NULL || node == tree->nil){
//		printf("Nao encontrado\n");
	}else{
		// Se o nó atual tiver um item com valor maior do que o valor inserido, se chama recursivamente para a esquerda
		if((*item)->id < node->item->id){
			remove_down(node->left_child, item, node, tree);
		// Se o nó atual tiver um item com valor menor do que o valor inserido, se chama recursivamente para a direita
		}else if((*item)->id > node->item->id){
			remove_down(node->right_child, item, node, tree);
		// Caso seja um valor igual ao do nó atual, realiza a remoção
		}else{
			NODE *aux = node;
			if(node->left_child == tree->nil && node->right_child == tree->nil){
				node = tree->nil;
			}else if(node->left_child == tree->nil){
				node->right_child->parent = parent;
				node = node->right_child;
			}else if(node->right_child == tree->nil){
				node->left_child->parent = parent;
				node = node->left_child;
			}else{
				remove_aux(node->right_child, node, tree);
				aux = NULL;
			}
			if(aux != NULL){
				if(parent != NULL){
					if(parent->right_child == aux) parent->right_child = node;
					else parent->left_child = node;
				}else tree->root = node;
				if(aux->color == BLACK){
					if(node->color == RED) node->color = BLACK;
					else{
						node->color = BLACK;
						balance_height(node, parent, tree);
					}
				}
				delete_node(&aux);
			}
			delete_item(item);
		}
	}
}

// Função que busca um item na árvore e realiza a sua remoção
int remove_item(TREE *tree, int id){
	if(tree != NULL){
		ITEM *item = create_item(id, 0);
		int result = item->id;
		remove_down(tree->root, &item, NULL, tree);
		if(item == NULL) return result;
		else{
			delete_item(&item);
			return INT_MAX;
		}
	}
	return INT_MIN;
}

// Função que apaga todos os nós abaixo do primeiro nó chamado
void delete_node_down(NODE **node, TREE *tree){
	if(node != NULL && *node != NULL && *node != tree->nil){
		delete_node_down(&(*node)->left_child, tree);
		delete_node_down(&(*node)->right_child, tree);
		delete_node(node);
	}
}

// Função que apaga todos os nós da árvore, liberando a memória alocada
int delete_tree(TREE **tree){
	if(tree != NULL && !empty_tree(*tree)){
		delete_node_down(&(*tree)->root, *tree);
		delete_node(&(*tree)->nil);
		free(*tree);
		*tree = NULL;
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
//		printf("\n");
		return 0;
	}
	return 1;
}
