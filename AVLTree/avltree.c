#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avltree.h"

#define MAX_TREE_HEIGHT 50
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

static AVLTreeNode *avl_find_raw(AVLTree *tree, Pointer data){

    if(tree == NULL || tree->size == 0 || tree->root == NULL){
        return NULL;
    }

    AVLTreeNode *runner = tree->root;

    for(;;){

        int cmp = tree->cmp_func(runner->data, data);

        if(cmp == 0){
            return runner;
        }

        if(cmp > 0){
            if(runner->left == NULL){
                return runner;
            }
            runner = runner->left;

        }else{
            if(runner->right == NULL){
                return runner;
            }
            runner = runner->right;
        }
    }
    return NULL;
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
    AVLTreeNode *runner = avl_find_raw(tree, data);
    if(runner == NULL){
        tree->size--;
        free(node);
        return NULL;
    }

    int cmp = tree->cmp_func(runner->data, data);
    if(cmp == 0){
        free(node);
        return data;
    }
    if(cmp > 0 && runner->left == NULL){
        runner->balance++;
        runner->left = node;
        node->parent = runner;
    }else if(cmp < 0 && runner->right == NULL){
        runner->balance--;
        runner->right = node;
        node->parent = runner;
    } else {
        return NULL;
    }

    balance_tree(tree);
    return data;
}

Pointer avl_find(AVLTree *tree, Pointer data){

    AVLTreeNode *node = avl_find_raw(tree, data);
    if(node == NULL || tree->cmp_func(node->data, data) != 0){
        return NULL;
    }
    return node->data;
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

static AVLTreeNode *avl_leftmost(AVLTreeNode *node){
    if(node == NULL){
        return NULL;
    }
    AVLTreeNode *runner = node;
    while(runner->left != NULL){
        runner = runner->left;
    }
    return runner;
}

static AVLTreeNode *avl_rightmost(AVLTreeNode *node){
    if(node == NULL){
        return NULL;
    }
    AVLTreeNode *runner = node;
    while(runner->right != NULL){
        runner = runner->right;
    }
    return runner;
}

static void avl_inform_parent(AVLTreeNode *node, AVLTreeNode *new_node){
    if(node == NULL || node->parent == NULL){
        return ;
    }
    AVLTreeNode *parent = node->parent;
    if(parent->left == node){
        parent->left = new_node;
    }else {
        parent->right = new_node;
    }
}

Pointer avl_delete(AVLTree *tree, Pointer data){

    AVLTreeNode *node = avl_find_raw(tree, data);
    if(node == NULL || tree->cmp_func(node->data, data) != 0){
        return NULL;
    }


    if(node->left == NULL && node->right == NULL){
        avl_inform_parent(node, NULL);
        free(node);
        if(tree->root == node){
            tree->root = NULL;
        }
        return data;
    }


    AVLTreeNode *replace = avl_leftmost(node->right);
    if(replace == NULL){
        replace = avl_rightmost(node->left);
    }
    if(replace == NULL){
        return NULL;
    }

    tree->size--;

    avl_inform_parent(node, replace);
    avl_inform_parent(replace, NULL);

    replace->left = node->left;
    replace->right = node->right;

    if(tree->root == node){
        tree->root = replace;
    }

    free(node);
    balance_tree(tree);
    return data;
}

void print_tree(AVLTreeNode *node){
    if(node == NULL){
        return ;
    }
    int tmp = *(int*)node->data;
    printf("{ %d }\n", tmp);


}







