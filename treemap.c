#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap *new = (TreeMap*)malloc(sizeof(TreeMap));
  new->root = NULL;
  new->current = NULL;
  new->lower_than = lower_than;
  return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode *new = createTreeNode(key,value);
  if(tree->root == NULL){
    tree->root = new;
    tree->current = new;
    return;
  }
  if(searchTreeMap(tree,key) == NULL){
    if(tree->lower_than(key,tree->current->pair->key) == 1){
      tree->current->left = new;
      new->parent = tree->current;
      tree->current = tree->current->left;
    }
    else if(tree->lower_than(tree->current->pair->key,key) == 1){
      tree->current->right = new;
      new->parent = tree->current;
      tree->current = tree->current->right;
    }
  }
}

TreeNode * minimum(TreeNode * x){
  if(x->left == NULL)return x;
  x = x->left;
  return minimum(x);
}


void removeNode(TreeMap * tree, TreeNode* node) {
  TreeNode *parent = node->parent;
  TreeNode *leftSon = node->left;
  TreeNode *rightSon = node->right;
  if(leftSon == NULL && rightSon == NULL){
    if(tree->lower_than(node->pair->key, parent->pair->key) == 1){
      parent->left = NULL;
    }
    else if(tree->lower_than(parent->pair->key, node->pair->key) == 1){
      parent->right = NULL;
    }
    node->left = NULL;
    node->pair = NULL;
    node->right = NULL;
    node->parent = NULL;
  }
  else if(leftSon != NULL && rightSon != NULL){
    TreeNode *min = minimum(rightSon);
    node->pair = min->pair;
    removeNode(tree,min);
  }
  else if(leftSon != NULL || rightSon !=NULL){
    TreeNode *Son =NULL;
    if(leftSon != NULL){
      Son = leftSon;
    }
    else{
      Son = rightSon;
    }
    node->pair = Son->pair;
    node->right = Son->right;
    node->left = Son->left;
  }
  return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode *aux = tree->root;
  while(aux != NULL){
    tree->current = aux;
    if(tree->lower_than(key,aux->pair->key) == 1){
      aux = aux->left;
    }
    else if(tree->lower_than(aux->pair->key,key) == 1){
      aux = aux->right;
    }
    else{
      if(is_equal(tree,aux->pair->key,key) == 1){
        return aux->pair;
      }
      else{
        break;
      }
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  tree->current = minimum(tree->root);
  return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree->current == NULL)return NULL;
  if(tree->current->right != NULL){
    tree->current = minimum(tree->current->right);
    return tree->current->pair;
  }
  else{
    if(tree->current->parent == tree->root ){
      tree->current = NULL;
      return NULL;
    }
  }
  if(tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) == 1){
    tree->current = tree->current->parent;
  }
  else{
    if(tree->current->parent->parent == NULL){
      tree->current = NULL;
      return NULL;
    }
    tree->current = tree->current->parent->parent;
  }
  return tree->current->pair;
}
