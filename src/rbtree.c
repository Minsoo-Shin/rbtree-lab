#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  // 21.12.06 completed: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  p->nil = NIL;
  p->root = NIL;
  return p;
}

void delete_node(rbtree* t, node_t* n)
{
  if (n == t->nil){
    return;
  }
  delete_node(t, n->left);
  delete_node(t, n->right);
  free(n);
}


void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;

  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;

  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *uncle = z->parent->parent->right; //y = uncle을 의미함
      if (uncle->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else
    {
      node_t *uncle = z->parent->parent->left; //y = uncle을 의미함
      if (uncle->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 21.12.06 completed: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  node_t *tmp_parent = t->nil;
  node_t *tmp_child = t->root;
  while (tmp_child != t->nil)
  {
    tmp_parent = tmp_child;
    if (key < tmp_child->key)
      tmp_child = tmp_child->left;
    else
      tmp_child = tmp_child->right;
  }
  z->parent = tmp_parent;
  if (tmp_parent == t->nil) //초기값을 왜 여기에?
    t->root = z;
  else if (key < tmp_parent->key)
    tmp_parent->left = z;
  else
    tmp_parent->right = z;

  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t* temp = t->root;
  //21.12.06 completed: implement find
  while (temp != t->nil)
  {
    if (temp->key > key)
      temp = temp-> left;
    else if (temp->key < key)
      temp = temp ->right;
    else
      return temp;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // 21.12.06 completed: implement find
  node_t* temp_parent = t-> nil;
  node_t* temp_child = t->root;

  while (temp_child != t->nil)
  {
    temp_parent = temp_child;
    temp_child = temp_child->left;
  }
  return temp_parent;
}

node_t *rbtree_max(const rbtree *t)
{
  // 21.12.06 completed: implement find
  node_t* temp_parent = t->nil;
  node_t* temp_child = t-> root;
  while (temp_child != t->nil)
  {
    temp_parent = temp_child;
    temp_child = temp_child->right;
  }  
  return temp_parent;
}

void rb_transplant(rbtree* t, node_t* u, node_t* v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left) 
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;

}

node_t* tree_minimum(rbtree* t, node_t* z)
{
  node_t* temp_parent;
  node_t* temp_child = z;

  while (temp_child == t->nil)
  {
    temp_parent = temp_child;
    temp_child = temp_child->left;
  }
  return temp_parent;
}

void rb_delete_fixup(rbtree* t, node_t* x)
{
  node_t* w;
  while (x!= t->nil && x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color ==RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x-> parent -> right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color ==RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x-> parent -> right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;    
      }
    }
  x->color = RBTREE_BLACK;
  }
}
int rbtree_erase(rbtree *t, node_t *z)
{
  // 21.12.06 completed: implement erase
  node_t* y = z; 
  node_t* x;
  color_t y_original_color = y->color;

  if (z->left == t->nil)
  {  
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  else if (z->right == t->nil) 
  {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  
  else
  {
    y = tree_minimum(t, z->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z)
      x->parent = y;
    else
    {
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == RBTREE_BLACK);
    rb_delete_fixup(t, x);
  free(z);
}

int node_to_array(const rbtree* t, node_t *n, key_t *arr, int i){
  if (n == t->nil)
    return i; 

  i = node_to_array(t, n->left, arr, i);   //left recur
  arr[i++] = n->key;   //print
  i = node_to_array(t, n->right, arr, i);  //right recur
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // 21.12.06 completed: implement to_array
  node_to_array(t, t->root, arr, 0);
  return 0;
}
