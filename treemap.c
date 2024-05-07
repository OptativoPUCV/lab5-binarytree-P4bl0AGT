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

    TreeMap *tree = (TreeMap *) malloc(sizeof(TreeMap));
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    
    TreeNode *node = createTreeNode(key, value);
    if(tree->root == NULL) //si mi arbol no tiene elemento entonce hago lo siguiente
    {
        tree->root = node; //mi root es igual a mi nodo creado
        tree->current = node;//actualizo mi current
        return;
    }
    
    TreeNode *aux = tree->root;
    while(1)
    {
        Pair *par = aux->pair;
        void *keyAux = par->key;
        if(is_equal(tree, keyAux, key))//si son iguales no hace nada
            return;

        if(aux->left == NULL || aux->right == NULL)//si algun lado es NULL significa que se puede insertar ahi
            break;
        
        if(tree->lower_than(keyAux, key)) //si la key que busco es mayor, avanzo a la derecha
            aux = aux->right;
        else // si la key que busco es menor, avanzo a la izquierda
            aux = aux->left;
    }
    
    if(aux->left == NULL) //si el lado izquierdo es null inserto mi elemento ahi
        aux->left = node;
    else //si el lado derecho es null inserto mi elemento ahi
        aux->right = node;

    node->parent = aux;//acutalizao mi nodo padre
    tree->current = node;//actualizo mi current

}

TreeNode * minimum(TreeNode * x){
    TreeNode *aux = x;
    while(aux->left != NULL)
        aux = aux->left;
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    while(aux != NULL)
    {
        Pair *par = aux->pair;
        void *keyAxu = par->key; 
        if(is_equal(tree , keyAxu, key)) //si las key son iguales se actualiza el current y se retorna la pareja (key, data)
        {
            tree->current = aux;
            return par;
        }
        if(tree->lower_than(keyAxu, key)) //si la key que busco es mayor, avanzo a la derecha
            aux = aux->right;
        else //si la key que busco es menor avanzo a la izquierda
            aux = aux->left;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    minimum(tree->root);
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
