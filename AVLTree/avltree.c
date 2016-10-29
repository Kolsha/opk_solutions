#include <stdlib.h>

#include "avltree.h"

static void balance_tree(AVLTree *tree){
    if(tree == NULL){
        return ;
    }
}

static AVLTreeNode *empty_node(Pointer data){

    AVLTreeNode *res = (AVLTreeNode*) malloc(sizeof(AVLTreeNode));
    if(res == NULL){
        return NULL;
    }
    res->data = data;
    res->balance = 0;
    res->parent = NULL;
    res->left = NULL;
    res->right = NULL;

    return res;
}

AVLTree *avl_create(CmpFunc cmp_func){

    if(cmp_func == NULL){
        return NULL;
    }
    AVLTree *res = (AVLTree*) malloc(sizeof(AVLTree));
    if(res == NULL){
        return NULL;
    }

    res->cmp_func = cmp_func;
    res->root = NULL;
    res->size = 0;

    return res;
}

size_t avl_size(AVLTree *tree){

    if(tree == NULL){
        return 0;
    }
    return tree->size;
}

Pointer avl_insert(AVLTree *tree, Pointer data){
    if(tree == NULL){
        return NULL;
    }
    AVLTreeNode *node = empty_node(data);
    if(node == NULL){
        return NULL;
    }

    tree->size++;
    if(tree->root == NULL){
        tree->root = node;
        return data;
    }
    AVLTreeNode *runner = tree->root;

    for(;;){
        int cmp = tree->cmp_func(runner->data, data);
        if(cmp == 0){
            free(node);
            return data;
        }
        if(cmp > 0){
            if(runner->left == NULL){
                runner->balance++;
                runner->left = node;
                node->parent = runner;
                break;
            }
            runner = runner->left;

        }else{
            if(runner->right == NULL){
                runner->balance--;
                runner->right = node;
                node->parent = runner;
                break;
            }
            runner = runner->right;
        }
    }

    balance_tree(tree);
    return data;

}

Pointer avl_find(AVLTree *tree, Pointer data){

    if(tree == NULL || tree->size == 0){
        return NULL;
    }

    AVLTreeNode *runner = tree->root;

    for(;;){

        int cmp = tree->cmp_func(runner->data, data);

        if(cmp == 0){
            return data;
        }

        if(cmp > 0){
            if(runner->left == NULL){
                return NULL;
            }
            runner = runner->left;

        }else{
            if(runner->right == NULL){
                return NULL;
            }
            runner = runner->right;
        }
    }
    return NULL;
}

static void avl_foreach_raw(AVLTreeNode *node,
                            void (*foreach_func)(Pointer data, Pointer extra_data),
                            Pointer extra_data){
    if(node == NULL || foreach_func == NULL){
        return ;
    }
    foreach_func(node->data, extra_data);
    avl_foreach_raw(node->left, foreach_func, extra_data);
    avl_foreach_raw(node->right, foreach_func, extra_data);

}

void avl_foreach(AVLTree *tree,
                 void (*foreach_func)(Pointer data, Pointer extra_data),
                 Pointer extra_data){ // from head to tail

    if(tree == NULL || tree->size == 0 || foreach_func == NULL){
        return ;
    }

    avl_foreach_raw(tree->root, foreach_func, extra_data);
}

static void avl_free_node(AVLTreeNode *node){

    if(node == NULL){
        return ;
    }
    avl_free_node(node->left);
    avl_free_node(node->right);
    free(node);
}

void avl_destroy(AVLTree *tree){

    if(tree == NULL){
        return ;
    }
    avl_free_node(tree->root);
    free(tree);
}
