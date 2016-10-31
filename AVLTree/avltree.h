#ifndef AVL_TREE_H
#define AVL_TREE_H


typedef void * Pointer;

/*
 * data1 > data2 :  1
 * data1 < data2 : -1
 * data1 = data2 :  0
 */
typedef int (*CmpFunc)(Pointer data1, Pointer data2);

/*
 * work : 1
 * stop : 0
 */
typedef int (*ForeachFunc)(Pointer data, Pointer extra_data);

typedef struct tAVLTreeNode {
    Pointer data;
    int height;
    struct tAVLTreeNode *parent;
    struct tAVLTreeNode *left , *right;
} AVLTreeNode;

typedef struct tAVLTree {
    AVLTreeNode *root;
    CmpFunc cmp_func;
    size_t size;
} AVLTree;

// Create empty tree
AVLTree *avl_create(CmpFunc cmp_func);

// Clear tree but do not destroy tree struct
void avl_clear(AVLTree *tree);

// Completely destroy tree
void avl_destroy(AVLTree *tree);

size_t avl_size(AVLTree *tree);

int avl_height(AVLTree *tree);

// Find element with equal data and return its data if any else NULL
Pointer avl_find(AVLTree *tree, Pointer data);

// Return data which was replaced by this insertion if any else NULL
Pointer avl_insert(AVLTree *tree, Pointer data);

// Delete node with equal data and return its data if any else NULL
Pointer avl_delete(AVLTree *tree, Pointer data);

// Call foreach_func for every node's data in tree passing given extra_data
void avl_foreach(AVLTree *tree,
                 ForeachFunc foreach_func,
                 Pointer extra_data);

int avl_check(AVLTree *tree);

#endif
