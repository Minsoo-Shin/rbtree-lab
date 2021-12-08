void rb_delete_fixup(rbtree *t, node_t *x){
  node_t *w;
  while(t->root != x && x->color == RBTREE_BLACK){
    if(x->parent->left == x){
      w = x->parent->right;
      // case1: uncle이 red
      if(w->color == RBTREE_RED){
        x->parent->color = RBTREE_RED;
        w->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      // case2: uncle이 black이고 두 자식이 모두 black
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // case3: uncle이 black이고 왼쪽 자식은 red 오른쪽 자식은 black
      else{
        if(w->right->color == RBTREE_BLACK){
          w->color = RBTREE_RED;
          w->left->color = RBTREE_BLACK;
          right_rotate(t, w);
          w = x->parent->right;
        }
        // case4: uncle이 black이고 오른쪽 자식이 red
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }else{
      w = x->parent->left;
      // case1: uncle이 red
      if(w->color == RBTREE_RED){
        x->parent->color = RBTREE_RED;
        w->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      // case2: uncle이 black이고 두 자식이 모두 black
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // case3: uncle이 black이고 오른쪽 자식은 red 왼쪽 자식은 black
      else{ 
        if(w->left->color == RBTREE_BLACK){
          w->color = RBTREE_RED;
          w->right->color = RBTREE_BLACK;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // case4: uncle이 black이고 왼쪽 자식이 red
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}