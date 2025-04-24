#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t* nil = (node_t*)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->nil = nil;
  p->root = nil;
  return p;
}



void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  // 해당 tree가 사용했던 메모리를 전부 반환
  // 후위 순회 : left->right->root
  post_order(t, t->root);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* x = t->root;
  node_t* y = t->nil;
  node_t* z = (node_t*)malloc(sizeof(node_t));
  z->key = key;

  //BST와 같은 방법으로 삽입할 자리 찾기
  while(x != t->nil){
    y = x; //부모를 저장
    if(z->key < x->key){
      x = x->left;
    }else{
      x = x->right;
    }
  }

  z->parent = y;
  
  if(y==t->nil){ //삽입할 자리의 부모가 nil이면 
    z->color = RBTREE_BLACK;
    t->root = z; //root에 z 삽입
  }else if(z->key < y->key){ //부모의 왼쪽 자식으로 삽입
    y->left = z;
  }else{ //부모의 오른쪽 자식으로 삽입
    y->right = z;
  }
  
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  
  rbtree_insert_fixup(t, z);
  return z;
}

void rbtree_insert_fixup(rbtree* t, node_t* z){
  node_t* y; //uncle을 저장할 노드

  while(z->parent->color==RBTREE_RED){ //red-red 충돌이 발생하는 동안 다음 과정을 반복
    
    if(z->parent == z->parent->parent->left){ //z의 부모가 z의 조부모의 왼쪽 자식인 경우 -> 이 부분 분기된 이유 : uncle이 조부모의 오른쪽 자식인지 왼쪽 자식인지 판단해서 넣어줘야 하므로 + left-left roation인지 right-right 로테이션인지도 이 조건에 따라 달라진다!  
      y = z->parent->parent->right; //y에 z의 uncle을 저장
      
      if(y->color == RBTREE_RED){ //uncle이 red인 경우
        z->parent->color = RBTREE_BLACK; //부모의 색을 black으로
        y->color = RBTREE_BLACK; //uncle의 색을 black으로
        z->parent->parent->color = RBTREE_RED; //조부모의 색을 red로
        z = z->parent->parent; //z를 z의 조부모로 갱신

      }else{ //uncle이 black인 경우

        if(z == z->parent->right){ //z가 내부 자식이면
          z = z->parent;
          rotate_left(t,z); //부모 노드 기준으로 왼쪽 회전해서 left-left로 만들어주기
        }

        z->parent->color = RBTREE_BLACK; //부모의 색을 black으로
        z->parent->parent->color = RBTREE_RED; //조부모의 색을 red로
        rotate_right(t, z->parent->parent); //조부모 기준으로 오른쪽 회전
      }

    }else{ //z의 부모가 z의 조부모의 오른쪽 자식인 경우
      y = z->parent->parent->left; //y에 z의 uncle을 저장
      
      if(y->color == RBTREE_RED){ //uncle이 red인 경우
        z->parent->color = RBTREE_BLACK; //부모의 색을 black으로
        y->color = RBTREE_BLACK; //uncle의 색을 black으로
        z->parent->parent->color = RBTREE_RED; //조부모의 색을 red로
        z = z->parent->parent; //z를 z의 조부모로 갱신

      }else{ //uncle이 black인 경우

        if(z == z->parent->left){ //z가 내부 자식이면
          z = z->parent;
          rotate_right(t, z); //부모 노드 기준으로 오른쪽 회전해서 right-right로 만들어주기
        }

        z->parent->color = RBTREE_BLACK; //부모의 색을 black으로
        z->parent->parent->color = RBTREE_RED; //조부모의 색을 red로
        rotate_left(t, z->parent->parent); //조부모 기준으로 왼쪽 회전
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  // 노드 x를 root로 설정하고 key값을 비교
  node_t* x = t->root;

  if(x == t->nil){
    return NULL; //트리가 비어있음 -> 해당 노드가 없다는 의미이므로 t->nil 반환
  }

  while(x != t->nil){
    if(x->key == key){ //값을 찾으면
      return x; //찾은 값을 반환
    }
    if(x->key > key){ //찾는 값이 더 작으면
      x = x->left; //x의 left를 탐색
    }else{ //찾는 값이 더 크면
      x = x->right; //x의 right를 탐색
    }
  }

  return NULL; //값을 찾지 못 했으면 t->nil 반환
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* x = t->root;
  node_t* ret = (node_t*)calloc(1, sizeof(node_t));
  ret = x;

  if(x == t->nil){
    return t->nil; //트리가 비어있음 -> 해당 노드가 없다는 의미이므로 t->nil 반환
  }

  while(x != t->nil){
    ret = x;
    x = x->left; //가장 왼쪽에 위치한 노드가 최솟값
  }

  return ret;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* x = t->root;
  node_t* ret = (node_t*)calloc(1, sizeof(node_t));
  ret = x;

  if(x == t->nil){
    return t->nil; //트리가 비어있음 -> 해당 노드가 없다는 의미이므로 t->nil 반환
  }

  while(x != t->nil){
    ret = x;
    x = x->right; //가장 오른쪽에 위치한 노드가 최댓값
  }

  return ret;
  
}

void rb_transplant(rbtree* t, node_t* u, node_t* v){ //노드 u가 루트인 서브 트리를 노드 v가 루트인 서브 트리로 교체
  if(u->parent == t->nil){ //u의 부모가 nil이면 u가 root라는 의미
    t->root = v; //v를 root로 만들기
  }else if(u == u->parent->left){ //u가 왼쪽 자식이면
    u->parent->left = v; //u의 부모의 왼쪽 자식을 v로 만들기
  }else{ //u가 오른쪽 자식이면
    u->parent->right = v; //u의 부모의 오른쪽 자식을 v로 만들기
  }
  v->parent = u->parent; //v의 부모를 u의 부모로 만들기
}

void rb_delete_fixup(rbtree* t, node_t* x){ //노드 삭제 시 rb트리 위반된 부분 재조정
  while(x != t->root && x->color == RBTREE_BLACK){ //x가 루트가 아니고 x

    if(x == x->parent->left){
      node_t* w = x->parent->right;
      
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_left(t, x->parent);
        w = x->parent->right;
      }

      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_right(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rotate_left(t, x->parent);
        x = t->root;
      }
    } else {
      node_t* w = x->parent->left;
      
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_right(t, x->parent);
        w = x->parent->left;
      }

      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_left(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

node_t* successor(rbtree* t, node_t* x){ //삭제할 노드의 후계자 찾기
  while(x->left != t->nil){
    x = x->left;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *z) { //노드 z를 삭제
  // TODO: implement erase
  node_t* x; //successor의 색 (실제로 삭제된 노드의 색 -> 얘가 black이면 delete fixup이 실행되고 재조정해야 됨 왜냐면 black depth가 바뀌었으니까. 근데 red였으면 재조정 필요 없음)
  node_t* y = z; //y는 z가 삭제되고 z를 대체할 노드
  color_t y_original_color = y->color; //y의 원래 색상 저장 -> 필요성 : 원래 색이 black인 경우 y를 삭제하거나 이동하면 rbtree의 특성이 위반될 수 있고 이를 rb-delete-fixup을 호출해 수정하기 위해
  
  if(z->left == t->nil){ //삭제될 z의 왼쪽 자식 노드가 없음 -> 오른쪽 자식 노드도 없다면 nil이 올라오는 것이므로 z가 그냥 삭제되는 것 / 오른쪽 자식 노드가 있다면 z가 삭제되고 그 자리가 오른쪽 자식 노드로 대체되는 것
    x = z->right; 
    rb_transplant(t, z, z->right); //z를 오른쪽 자식으로 교체

  }else if(z->right == t->nil){ //삭제될 z의 오른쪽 자식 노드가 없음
    x = z->left;
    rb_transplant(t, z, z->left); //z를 왼쪽 자식으로 교체

  }else{ //삭제될 z의 오른쪽, 왼쪽 자식이 모두 있음
    y = successor(t, z->right); //후계자 (오른쪽 서브트리 중 최솟값)
    y_original_color = y->color; //후계자의 색
    x = y->right; //x를 왜 y의 오른쪽 자식 노드로 교체??

    if(y != z->right){ //후계자가 z의 자식이 아닌 경우
      rb_transplant(t, y, y->right); //후계자 제거하고 그 자리에 후계자 오른쪽 자식이 올라옴
      y->right = z->right; //z의 오른쪽 자식을 후계자에게 붙이기
      y->right->parent = y; 

    }else{
      x->parent = y;
    }

    rb_transplant(t, z, y); //z를 y로 교체
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if(y_original_color == RBTREE_BLACK){ //삭제된 노드의 원래 색이 black이라면 rbtree의 5번 속성을 위반한 것이므로 트리를 재조정
    rb_delete_fixup(t, x);
  }
  return 0;
}

int in_order(const rbtree* t, node_t* root, int idx, key_t *arr, size_t n){
  if(t->root == t->nil || n < idx){ //트리가 비었거나 배열을 다 채웠으면 리턴
    return 0;
  }
  //트리 노드 수가 n보다 적으면 리턴
  if(root != t->nil){
    idx = in_order(t, root->left, idx, arr, n);
    arr[idx] = root->key;
    idx=idx+1;
    idx = in_order(t, root->right, idx, arr, n);
  }
  return idx;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  // rbtree(bst)를 중위 순회로 읽으면 key값을 오름차순으로 읽을 수 있음
  in_order(t, t->root, 0, arr, n);
  return 0;
}

void rotate_left(rbtree* t, node_t* node){ //node 중심으로 왼쪽 회전
  node_t* y = node->right; //node의 오른쪽 자식을 y로 정의
  node_t* z = y->left; //y의 왼쪽 자식을 z로 정의

  node->right = z; //node의 오른쪽 자식에 y의 왼쪽 자식을 연결 (자식을 연결할 때는 nil인지 확인 안 해도 되는 걸 보니 값이 있으면 그 값을 자식으로 붙이는 거고 nil이면 nil을 자식으로 붙이는 듯)
  if(z != t->nil){ //nil이 아닐 때 부모를 설정해주는 작업은 꼭 필요함. 자식 노드에서 부모 노드를 참조할 수 있어야 하니까. y의 왼쪽 자식이 nil이면 부모 설정해주는 건 안 해도 댐 (내 생각임)
    z->parent = node; 
  }

  y->parent = node->parent; //y가 x자리로 올라가서 루트가 되어야 하니까 y의 부모를 x의 부모로 만들어줌
  
  if(node->parent == t->nil){  //근데 x의 부모가 nil 즉 x가 루트인 경우
    t->root = y; //y가 x자리로 올라가야 하므로 y를 루트로 만들어주기
  } else if(node == node->parent->left){ //만약 x가 왼쪽 자식이면 
    node->parent->left = y; //y를 x의 부모의 왼쪽 자식으로 만들어주기
  } else{ //x가 오른쪽 자식이면
    node->parent->right = y; //y를 x의 부모의 오른쪽 자식으로 만들어주기
  }
  
  y->left = node; //x를 y의 왼쪽 자식으로 만들어줌
  node->parent = y; //x의 부모를 y로 만들어줌
}

void rotate_right(rbtree* t, node_t* node){
  node_t* y = node->left;
  node_t* z = y->right;

  node->left = z;
  if(z!= t->nil){
    z->parent = node;
  }

  y->parent = node->parent;

  if(node->parent == t->nil){  
    t->root = y; 
  } else if(node == node->parent->left){ 
    node->parent->left = y; 
  } else{ 
    node->parent->right = y; 
  }

  y->right = node;
  node->parent = y;
}
