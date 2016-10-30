#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "avltree.h"

#define MAX_TREE_HEIGHT 50

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

static int height(AVLTreeNode *node){
    return (node != NULL) ? node->height : -1;
}

static int balance(AVLTreeNode *node){
    if(node == NULL){
        return 0;
    }
    //int hl = height(node->left);
    //int hr = height(node->right);
    return height(node->left) - height(node->right);
}

static void fixheight(AVLTreeNode* node)
{
    if(node == NULL){
        return ;
    }

    int hl = height(node->left);
    int hr = height(node->right);
    node->height = ((hl > hr) ? hl : hr) + 1;
}

static void small_left(AVLTree *tree, AVLTreeNode *Bnode){

    /*
        B
       / \
      A   Br
     / \
    Al  Ar
    */
    if(tree == NULL || Bnode == NULL){
        return ;
    }
    AVLTreeNode *Anode = Bnode->left;

    Anode->parent = Bnode->parent;

    avl_inform_parent(Bnode, Anode);
    if(tree->root == Bnode){
        tree->root = Anode;
    }
    Bnode->parent = Anode;
    Bnode->left = Anode->right;
    Anode->right = Bnode;
    fixheight(Bnode);
    fixheight(Anode);


}

static void small_right(AVLTree *tree, AVLTreeNode *Bnode){

    /*
        B
       / \
      Bl  A
         / \
        Al  Ar
    */
    if(tree == NULL || Bnode == NULL){
        return ;
    }
    AVLTreeNode *Anode = Bnode->right;
    int b = *(int*)Bnode->data;
    int a = *(int*)Anode->data;


    Anode->parent = Bnode->parent;

    avl_inform_parent(Bnode, Anode);
    if(tree->root == Bnode){
        tree->root = Anode;
    }

    Bnode->parent = Anode;
    Bnode->right = Anode->left;
    Anode->left = Bnode;
    fixheight(Bnode);
    fixheight(Anode);

}

static void big_left(AVLTree *tree, AVLTreeNode *Anode){
    small_right(tree, Anode->left);
    small_left(tree, Anode);
}

static void big_right(AVLTree *tree, AVLTreeNode *Anode){
    small_left(tree, Anode->right);
    small_right(tree, Anode);
}

static void balance_tree(AVLTree *tree, AVLTreeNode *node){

    if(tree == NULL || tree->size < 1 || node == NULL){
        return ;
    }
    int tmp = *(int*)node->data;

    fixheight(node);
    int blnc = balance(node);
    if(blnc >= 2){
        if(balance(node->left) >= 0){
            small_left(tree, node);
        }else{
            big_left(tree, node);
        }
        return ;
    }
    if(blnc <= -2){
        if(balance(node->right) <= 0){
            small_right(tree, node);
        }else{
            big_right(tree, node);
        }
        return ;
    }
    return balance_tree(tree, node->parent);
}

static AVLTreeNode *empty_node(Pointer data){

    AVLTreeNode *res = (AVLTreeNode*) malloc(sizeof(AVLTreeNode));
    if(res == NULL){
        return NULL;
    }
    res->data = data;
    //res->balance = 0;
    res->parent = NULL;
    res->left = NULL;
    res->right = NULL;
    res->height = 0;

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

    int tmp = *(int*)data;
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
        runner->left = node;
        node->parent = runner;
    }else if(cmp < 0 && runner->right == NULL){
        runner->right = node;
        node->parent = runner;
    } else {
        return NULL;
    }
    printf("\nRoot:");
    print_tree(tree->root, 0);
    balance_tree(tree, node);
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



Pointer avl_delete(AVLTree *tree, Pointer data){

    AVLTreeNode *node = avl_find_raw(tree, data);
    if(node == NULL || tree->cmp_func(node->data, data) != 0){
        return NULL;
    }


    if(node->left == NULL && node->right == NULL){
        avl_inform_parent(node, NULL);
        balance_tree(tree, node->parent);
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
    assert(replace != NULL);
    assert(replace->left == NULL && replace->right == NULL);
    if(replace == NULL){
        return NULL;
    }

    tree->size--;

    avl_inform_parent(node, replace);
    avl_inform_parent(replace, NULL);



    replace->left = node->left;
    if(replace->left != NULL){
        replace->left->parent = replace;
    }
    replace->right = node->right;

    if(replace->right != NULL){
        replace->right->parent = replace;
    }

    if(tree->root == node){
        tree->root = replace;
    }


    balance_tree(tree, replace);
    free(node);
    return data;
}

void print_tree(AVLTreeNode *node, int level){

    if(node == NULL){
        printf("L(%d) Empty\n", level);
        return ;
    }
    int tmp = *(int*)node->data;
    printf("H(%d) { %d }\n", level, tmp);

    if(node->left != NULL)
    {
        printf("Left:");
        print_tree(node->left, level + 1);
    }
    if(node->right != NULL){
        printf("Right:");
        print_tree(node->right, level + 1);
    }




}
static int avl_node_check(AVLTree *tree, AVLTreeNode *node){

    if(node->left == NULL && node->right == NULL){
        return 1;
    }
    if(node->left != NULL){
        if(tree->cmp_func(node->data, node->left->data) <= 0)
        {
            return 0;
        }
        avl_node_check(tree, node->left);
    }
    if(node->right != NULL){
        if(tree->cmp_func(node->data, node->right->data) >= 0)
        {
            return 0;
        }
        avl_node_check(tree, node->right);
    }
    return 1;

}

int avl_check(AVLTree *tree){

    if(tree == NULL || (tree->size > 0 && tree->root == NULL)){
        return 0;
    }
    if(tree->root == NULL){
        return 1;
    }
    return avl_node_check(tree, tree->root);
}





