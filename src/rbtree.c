#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

void leftRotation(rbtree *t, node_t *x);
void rightRotation(rbtree *t, node_t *x);
void delete_rbtree(rbtree *t);
void RB_InsertFixup(rbtree *t, node_t *newNode);
node_t *rbtree_insert(rbtree *t, const key_t key);
node_t *rbtree_find(const rbtree *t, const key_t key);
node_t *rbtree_min(const rbtree *t);
node_t *rbtree_max(const rbtree *t);
void RB_DeleteFixup(rbtree *t, node_t *db);
void RB_TransPlant(rbtree *t, node_t *u, node_t *v);
int rbtree_erase(rbtree *t, node_t *rmNode);
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n);

// 스택 시작
typedef struct
{
  node_t *nodeArr[100000];
  int top;
} Stack;

void push(Stack *s, node_t *data)
{
  s->nodeArr[s->top++] = data;
}

node_t *pop(Stack *s)
{
  if (s->top == 0)
  {
    return NULL;
  }
  return s->nodeArr[--(s->top)];
}
// 스택 끝

// 큐 시작
typedef struct
{
  node_t *nodeArr[100000];
  int front;
  int rear;
} Queue;

void enqueue(Queue *queue, node_t *data)
{
  if (queue->rear == 1000)
  {
    return;
  }
  queue->nodeArr[(queue->rear)++] = data;
}

node_t *dequeue(Queue *queue)
{
  if (queue->front == queue->rear)
  {
    return NULL;
  }
  return queue->nodeArr[(queue->front)++];
}
// 큐 끝

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  p->root->color = RBTREE_BLACK;
  return p;
}

void leftRotation(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x->parent->left == x)
  {
    // 부모의 왼쪽 노드가 x인 경우
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void rightRotation(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  while (t->root != t->nil)
  {
    rbtree_erase(t, t->root);
  }
  free(t->nil);
  free(t);
}

void RB_InsertFixup(rbtree *t, node_t *newNode)
{
  node_t *uncleNode;
  while (newNode->parent->color == RBTREE_RED)
  {
    if (newNode->parent == newNode->parent->parent->left)
    {
      uncleNode = newNode->parent->parent->right;
      if (uncleNode->color == RBTREE_RED)
      {

        newNode->parent->color = RBTREE_BLACK;
        uncleNode->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        newNode = newNode->parent->parent;
      }
      else
      {
        if (newNode == newNode->parent->right)
        {
          newNode = newNode->parent;
          leftRotation(t, newNode);
        }
        newNode->parent->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        rightRotation(t, newNode->parent->parent);
      }
    }
    else
    {
      uncleNode = newNode->parent->parent->left;

      if (uncleNode->color == RBTREE_RED)
      {

        newNode->parent->color = RBTREE_BLACK;
        uncleNode->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        newNode = newNode->parent->parent;
      }
      else
      {
        if (newNode == newNode->parent->left)
        {
          newNode = newNode->parent;
          rightRotation(t, newNode);
        }
        newNode->parent->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        leftRotation(t, newNode->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  // 부모 노드 y
  node_t *parentNode = t->nil;
  // 현재 노드 x
  node_t *nowNode = t->root;
  // 새로운 노드 newNode
  node_t *newNode = (node_t *)malloc(sizeof(node_t));
  newNode->key = key;

  // 저장할 위치를 찾아감
  while (nowNode != t->nil)
  {
    parentNode = nowNode;
    if (newNode->key < nowNode->key)
    {
      nowNode = nowNode->left;
    }
    else
    {
      nowNode = nowNode->right;
    }
  }

  // x가 t.nil일 경우 이므로 리프 노드에 도달
  // 부모 노드 설정
  newNode->parent = parentNode;

  // 부모의 키 값과 새 노드의 키 값 동일 시 종료
  if (parentNode == t->nil)
  {
    // 부모노드가 y라는 것은 루트 위치에 저장되어야 한다는 것
    t->root = newNode;
  }
  else if (newNode->key < parentNode->key)
  {
    // 왼쪽 자식으로 설정
    parentNode->left = newNode;
  }
  else
  {
    // 오른쪽 자식으로 설정
    parentNode->right = newNode;
  }

  // newNode 초기화
  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->color = RBTREE_RED;

  // 레드 블랙 트리
  RB_InsertFixup(t, newNode);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *target = t->root;
  while (target != t->nil)
  {
    if (target->key < key)
    {
      target = target->right;
    }
    else if (target->key > key)
    {
      target = target->left;
    }
    else if (target->key == key)
    {
      return target;
    }
  }
  return NULL;
}

node_t *subTree_min(rbtree *t, node_t *n)
{
  while (n->left != t->nil)
  {
    n = n->left;
  }
  return n;
}

node_t *subTree_max(rbtree *t, node_t *n)
{
  while (n->right != t->nil)
  {
    n = n->right;
  }
  return n;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *target = t->root; 
  while (target->left != t->nil)
  {
    target = target->left;
  }

  return target;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *target = t->root; 
  while (target->right != t->nil)
  {
    target = target->right;
  }

  return target;
}

void RB_DeleteFixup(rbtree *t, node_t *db)
{
  // db는 doubly Black
  node_t *bro;
  while (db != t->root && db->color == RBTREE_BLACK)
  {
    if (db == db->parent->left)
    {
      bro = db->parent->right;

      if (bro->color == RBTREE_RED)
      {
        bro->color = RBTREE_BLACK;
        db->parent->color = RBTREE_RED;
        leftRotation(t, db->parent);
        bro = db->parent->right;
        
      }

      if (bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK)
      {
        bro->color = RBTREE_RED;
        db = db->parent;
        
      }
      else{
        if (bro->right->color == RBTREE_BLACK)
        {
          bro->left->color = RBTREE_BLACK;
          bro->color = RBTREE_RED;
          rightRotation(t, bro);
          bro = db->parent->right;
        }
        bro->color = db->parent->color;
        db->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK;
        leftRotation(t, db->parent);
        db = t->root;
      }
    }
    else
    {
      bro = db->parent->left;

      if (bro->color == RBTREE_RED)
      {
        bro->color = RBTREE_BLACK;
        db->parent->color = RBTREE_RED;
        rightRotation(t, db->parent);
        bro = db->parent->left;
        
      }

      if (bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK)
      {
        bro->color = RBTREE_RED;
        db = db->parent;
        
      }
      else{
        if (bro->left->color == RBTREE_BLACK)
        {
          bro->right->color = RBTREE_BLACK;
          bro->color = RBTREE_RED;
          leftRotation(t, bro);
          bro = db->parent->left;
        }
        bro->color = db->parent->color;
        db->parent->color = RBTREE_BLACK;
        bro->left->color = RBTREE_BLACK;
        rightRotation(t, db->parent);
        db = t->root;
      }
    }
  }
  db->color = RBTREE_BLACK;
}

void RB_TransPlant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *rmNode)
{
  // TODO: implement erase
  node_t *tempNode = rmNode;
  color_t tempNodeOriginalColor = tempNode->color;
  node_t *subNode;

  if (rmNode->left == t->nil)
  {
    subNode = rmNode->right;
    RB_TransPlant(t, rmNode, rmNode->right);
  }
  else if (rmNode->right == t->nil)
  {
    subNode = rmNode->left;
    RB_TransPlant(t, rmNode, rmNode->left);
  }
  else
  {
    tempNode = subTree_min(t, rmNode->right);
    tempNodeOriginalColor = tempNode->color;
    subNode = tempNode->right;
    if (tempNode->parent == rmNode)
    {
      subNode->parent = tempNode;
    }
    else
    {
      RB_TransPlant(t, tempNode, tempNode->right);
      tempNode->right = rmNode->right;
      tempNode->right->parent = tempNode;
    }
    RB_TransPlant(t, rmNode, tempNode);
    tempNode->left = rmNode->left;
    tempNode->left->parent = tempNode;
    tempNode->color = rmNode->color;
  
    if (tempNodeOriginalColor == RBTREE_BLACK)
      RB_DeleteFixup(t, subNode);
  }

  free(rmNode);
  return 0;
}
void printTree(rbtree *t, node_t *cur, int level, int isLeft) {
    if (cur == t->nil) {
        return;
    }

    // 오른쪽 자식 노드 출력
    printTree(t, cur->right, level + 1, 0);

    // 현재 노드 출력
    for (int i = 0; i < level - 1; i++) {
        printf("    ");
    }
    if (level > 0) {
        printf(isLeft ? " \\_ " : " /⎺ ");  // 왼쪽 자식일 경우 "\\" 출력, 오른쪽 자식일 경우 "/" 출력
    }
    if (cur->color == RBTREE_RED)
    {
      printf("\x1b[31m%d\x1b[0m\n", cur->key);
    }
    else{
      printf("%d\n", cur->key);
    }

    // 왼쪽 자식 노드 출력
    printTree(t, cur->left, level + 1, 1);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  Stack* stack = (Stack*) calloc(1,sizeof(Stack));
  node_t* curNode = t->root;
  int i = 0;
  //printTree(t,t->root,0,0);
  while(stack->top != 0 || curNode != t->nil){
    while(curNode != t->nil){
      push(stack, curNode);
      curNode = curNode->left;
    }
    curNode = pop(stack);
    arr[i] = curNode->key;
    i++;
    if(i == n+1){
      return 0;
    }
    curNode = curNode->right;
  }
  free(stack);
  return 0;
}

void rbtree_to_print(node_t *t, node_t *nil)
{
    // TODO: implement to_print
    if (t == nil)
    {
        printf("노드 바닥이에용\n");
        return;
    }
    printf("key = %d, color = %d \n", t->key, t->color);
    rbtree_to_print(t->left, nil);
    rbtree_to_print(t->right, nil);
}