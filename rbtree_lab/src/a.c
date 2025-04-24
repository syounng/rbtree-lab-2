#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void post_order(rbtree* t, node_t* root) {
    if(root != t->nil){
      post_order(t, root->left);
      post_order(t, root->right);
      free(root);
    }
  }