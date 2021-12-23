#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  // 21.12.06: completed: initialize struct if needed
  rbtree *p = (rbtree *)malloc(sizeof(rbtree));
  node_t *NIL = (node_t *)malloc(sizeof(node_t));
  NIL->color = RBTREE_BLACK;  // 초기값 (property 3)
  p->nil = NIL;   // 초기값
  p->root = NIL;  // 초기값
  return p;
}

void delete_node(rbtree* t, node_t* n);

void delete_rbtree(rbtree *t)
{ // 21.12.06: reclaim the tree nodes's memory
  // 
  delete_node(t, t->root);  
  free(t->nil); 
  free(t);
}


void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);

void rbtree_insert_fixup(rbtree *t, node_t *z)
{ // 새 노드는 무조건 Red, 부모까지 Red이면 Fix-up 
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *uncle = z->parent->parent->right;
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
    else //z->parent == z->parent->parent->right
    {
      node_t *uncle = z->parent->parent->left;
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
  // 부모가 Black 이면 Color change로 레드트리 만족
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 21.12.06 completed: implement insert
  node_t *z = (node_t *)malloc(sizeof(node_t));
  node_t *tmp_parent = t->nil;
  node_t *tmp_child = t->root;
  // key의 위치에 맞는 곳에 접근
  while (tmp_child != t->nil)
  {
    tmp_parent = tmp_child;
    if (key < tmp_child->key)
      tmp_child = tmp_child->left;
    else
      tmp_child = tmp_child->right; // key값이 같은 값이 들어가면 저장한다. 
  }
  // z의 parent를 설정
  z->parent = tmp_parent; 
  // t에 root가 없을 경우
  if (tmp_parent == t->nil)
    t->root = z;
  // parent의 child를 설정
  else if (key < tmp_parent->key)
    tmp_parent->left = z;
  else
    tmp_parent->right = z;
  //z값 초기화
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // 레드트리 규칙
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

node_t *rbtree_min(const rbtree *t);
node_t *rbtree_max(const rbtree *t);
void rb_transplant(rbtree* t, node_t* u, node_t* v);
node_t* tree_minimum(rbtree* t, node_t* z);

void rb_delete_fixup(rbtree* t, node_t* x)
{
  node_t* w;
  while (x != t->root && x->color == RBTREE_BLACK)
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
      w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->left->color ==RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x-> parent -> left;
        }
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
int rbtree_erase(rbtree *t, node_t *z)
{
  // 21.12.06 completed: implement erase
  node_t* y = z; 
  node_t* x;
  color_t y_original_color = y->color;

  if (z->right == t->nil) 
  {
    x = z->left;
    rb_transplant(t, z, z->left);
  }

  else if (z->left == t->nil)
  {  
    x = z->right;
    rb_transplant(t, z, z->right);
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
  if (y_original_color == RBTREE_BLACK)
    rb_delete_fixup(t, x);
  free(z);
  return 0;
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



///////////////// 유틸 함수///////////////////////
void delete_node(rbtree* t, node_t* n)
{// 후위순회를 하면서 root를 마지막에 할당을 반환한다. 
  if (n == t->nil)
    return;
  delete_node(t, n->left);
  delete_node(t, n->right);
  free(n);
}

void left_rotate(rbtree *t, node_t *x)
{
  // x->right가 nil이 아니라는 가정 하에 함수 진행
  // x-> right이 없다면 
  // x : rotate를 하는 축
  // y : x 의 오른쪽 자식
  /*
       x
     /   \
   x.l    y
        /   \
      y.l   y.r
  */

  node_t *y = x->right;
  x->right = y->left;  // x, y.l 연결

  if (y->left != t->nil)
    y->left->parent = x; // x, y.l 연결

  y->parent = x->parent; // x.p, y.p 대체하기
  if (x->parent == t->nil) // x가 root 인 경우
    t->root = y;
  else if (x == x->parent->left) // x가 왼쪽 자식인 경우
    x->parent->left = y;
  else   // x가 오른쪽 자식인 경우
    x->parent->right = y;
  y->left = x;  // y 왼쪽 자식에 x 연결해주기
  x->parent = y; // y 왼쪽 자식에 x 연결해주기
}


void right_rotate(rbtree *t, node_t *x)  // left_rotate와 동일
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
  node_t* temp_parent = t->nil;
  node_t* temp_child = z;

  while (temp_child != t->nil)
  {
    temp_parent = temp_child;
    temp_child = temp_child->left;
  }
  return temp_parent;
}
