#include "rbtree.h"
#include <stdio.h>

void preorder(rbtree* tree, node_t *node){
    // root - left - right
    if (node == tree->nil) {
        return ;
    }

    printf("%d ", node->key);
    preorder(tree, node->left);
    preorder(tree, node->right);
}

void inorder(rbtree* tree, node_t *node){
    //inorder : left - root - right
    if (node == tree->nil) {
        return ;
    }

    inorder(tree, node->left);
    printf("%d ", node->key);
    inorder(tree, node->right);
}

void postorder(rbtree* tree, node_t *node){
    // postorder : left - right - root
    if (node == tree->nil) {
        return ;
    }

    postorder(tree, node->left);
    postorder(tree, node->right);
    printf("%d ", node->key);
}


int main(void) {
    int i;
    key_t arr[7] = {1227918265, 3978157, 263514239, 1969574147, 1833982879, 488658959, 231688945};
    rbtree* tree = new_rbtree();
    for (i=0; i < sizeof(arr)/sizeof(int); i++)
        rbtree_insert(tree, *(arr+i));
    printf("pre-order 결과:\n");
    preorder(tree, tree->root);
    printf("\n");
    
    printf("in-order 결과:\n");
    inorder(tree, tree->root);
    printf("\n");
    
    printf("post-order 결과:\n");
    postorder(tree, tree->root);
    printf("\n");

}
