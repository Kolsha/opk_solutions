#include <stdlib.h>
#include "rbtree.h"

/***********************
    * Static functions  *
    ***********************/
static nodeColor getColor(RBTreeNode *node){

    return (node != NULL) ? node->color : BLACK;
}

static RBTreeNode *getParent(RBTreeNode *node){

    return (node != NULL) ? node->parent : NULL;
}

static void rb_left_rotate(RBTree *tree, RBTreeNode *node){

    /*
     (node)              (right)
     /     \       =>    /     \
    a    (right)       (node)   c
         /     \      /     \
        b       c    a       b
    */

    if(tree == NULL || node == NULL){
        return ;
    }
    RBTreeNode *right = node->right;

    node->right = right->left;
    if (right->left != NULL)
    {
        right->left->parent = node;
    }

    if (right != NULL)
    {
        right->parent = node->parent;
    }
    if (node->parent != NULL) {
        if (node == node->parent->left)
        {
            node->parent->left = right;
        }
        else{
            node->parent->right = right;
        }
    } else{
        tree->root = right;
    }

    right->left = node;
    if (node != NULL)
    {
        node->parent = right;
    }

}

static void rb_right_rotate(RBTree *tree, RBTreeNode *node){


    /*
     (node)              (left)
     /     \       =>    /     \
   (left)  a           b     (node)
   /     \                   /     \
  b      c                  c       a
    */

    if(tree == NULL || node == NULL){
        return ;
    }

    RBTreeNode *left = node->left;
    node->left = left->right;

    if (left->right != NULL)
    {
        left->right->parent = node;
    }
    if (left != NULL)
    {
        left->parent = node->parent;
    }

    if (node->parent != NULL) {
        if (node == node->parent->right)
        {
            node->parent->right = left;
        }
        else{
            node->parent->left = left;
        }

    } else {
        tree->root = left;
    }

    left->right = node;
    if (node != NULL){
        node->parent = left;
    }

}

static void rb_fixup(RBTree *tree, RBTreeNode *x) {

    if(tree == NULL || x == NULL){
        return ;
    }
    int v_x = *(int*)x->data;
    int v_r = *(int*)tree->root->data;
    while ((x != tree->root) && getColor(x->parent) == RED) {
        v_x = *(int*)x->data;
        v_r = *(int*)tree->root->data;

        if(getParent(x) == NULL || getParent(getParent(x)) == NULL){
            break;
        }
        if (x->parent == x->parent->parent->left) {
            RBTreeNode *y = x->parent->parent->right;
            if (getColor(y) == RED) {

                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {

                /* uncle is BLACK */
                if (x == x->parent->right) {
                    /* make x a left child */
                    x = x->parent;
                    rb_left_rotate(tree, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rb_right_rotate(tree, x->parent->parent);
            }
        } else {

            RBTreeNode *y = x->parent->parent->left;
            if (getColor(y) == RED) {

                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {

                /* uncle is BLACK */
                if (x == x->parent->left) {
                    x = x->parent;
                    rb_right_rotate(tree, x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rb_left_rotate(tree, x->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

static RBTreeNode *create_node(Pointer data){

    RBTreeNode *res = (RBTreeNode*) malloc(sizeof(RBTreeNode));
    if(res == NULL){
        return NULL;
    }
    res->data = data;
    res->parent = NULL;
    res->left = NULL;
    res->right = NULL;
    res->color = RED;

    return res;
}

static void rb_free_node(RBTreeNode *node){

    if(node == NULL){
        return ;
    }
    RBTreeNode *left = node->left;
    RBTreeNode *right = node->right;
    free(node);
    rb_free_node(left);
    rb_free_node(right);
}

static RBTreeNode *rb_find_raw(RBTree *tree, Pointer data){

    if(tree == NULL || tree->size == 0 || tree->root == NULL){
        return NULL;
    }

    RBTreeNode *runner = tree->root;

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

static void rb_foreach_raw(RBTreeNode *node,
                           ForeachFunc foreach_func,
                           Pointer extra_data){

    if(node == NULL || foreach_func == NULL){
        return ;
    }
    if(!foreach_func(node->data, extra_data)){
        return ;
    }
    rb_foreach_raw(node->left, foreach_func, extra_data);
    rb_foreach_raw(node->right, foreach_func, extra_data);

}

static int rb_node_check(RBTree *tree, RBTreeNode *node){

    if(getColor(node) == RED){
        if(!(getColor(node->right) == BLACK && getColor(node->left) == BLACK)){
            return 0;
        }
    }

    if(node->left == NULL && node->right == NULL){
        return 1;
    }
    if(node->left != NULL){
        if(tree->cmp_func(node->data, node->left->data) <= 0)
        {
            return 0;
        }
        rb_node_check(tree, node->left);
    }
    if(node->right != NULL){
        if(tree->cmp_func(node->data, node->right->data) >= 0)
        {
            return 0;
        }
        rb_node_check(tree, node->right);
    }
    return 1;

}

/***********************
    * Public functions  *
    ***********************/

RBTree * rb_create(CmpFunc cmp_func){

    if(cmp_func == NULL){
        return NULL;
    }
    RBTree *res = (RBTree*) malloc(sizeof(RBTree));
    if(res == NULL){
        return NULL;
    }

    res->cmp_func = cmp_func;
    res->root = NULL;
    res->size = 0;

    return res;
}

size_t rb_size(RBTree *tree){

    if(tree == NULL){
        return 0;
    }
    return tree->size;
}

void rb_destroy(RBTree *tree){

    if(tree == NULL){
        return ;
    }
    rb_free_node(tree->root);
    free(tree);
}

void rb_clear(RBTree *tree){

    if(tree == NULL){
        return ;
    }
    rb_free_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

Pointer rb_find(RBTree *tree, Pointer data){
    RBTreeNode *node = rb_find_raw(tree, data);
    if(node == NULL || tree->cmp_func(node->data, data) != 0){
        return NULL;
    }
    return node->data;
}

Pointer rb_insert(RBTree *tree, Pointer data){

    if(tree == NULL){
        return NULL;
    }
    RBTreeNode *node = create_node(data);

    if(node == NULL){
        return NULL;
    }

    tree->size++;
    if(tree->root == NULL){
        node->color = BLACK;
        tree->root = node;
        return data;
    }
    RBTreeNode *runner = rb_find_raw(tree, data);
    if(runner == NULL){
        tree->size--;
        free(node);
        return NULL;
    }

    int cmp = tree->cmp_func(runner->data, data);
    if(cmp == 0){
        Pointer tmp = runner->data;
        runner->data = data;
        free(node);
        return tmp;
    }
    if(cmp > 0){
        runner->left = node;
        node->parent = runner;
    }else if(cmp < 0){
        runner->right = node;
        node->parent = runner;
    }

    rb_fixup(tree, node);

    return data;
}

void rb_foreach(RBTree *tree,
                ForeachFunc foreach_func,
                Pointer extra_data){
    if(tree == NULL || tree->size == 0 || foreach_func == NULL){
        return ;
    }

    rb_foreach_raw(tree->root, foreach_func, extra_data);
}

int rb_check(RBTree *tree){

    if(tree == NULL || (tree->size > 0 && tree->root == NULL)){
        return 0;
    }
    if(getColor(tree->root) != BLACK){
        return 0;
    }
    if(tree->root == NULL){
        return 1;
    }
    return rb_node_check(tree, tree->root);
}
