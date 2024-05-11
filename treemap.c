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
    /*Pair *parEliminar = searchTreeMap(tree, node->pair->key);
    if(parEliminar == NULL)
        return;*/
    TreeNode *nodoEliminar = node;
    TreeNode *nodoPadre = nodoEliminar->parent;
    TreeNode *nodoHijo = NULL;

    //CASO 0: EL NODO A ELIMINAR ES EL ROOT
    //if(nodoEliminar == tree->root)
        //tree->root = NULL;
    
    //CASO 1: EL NODO SIN HIJOS
    if(nodoEliminar->left == NULL && nodoEliminar->right == NULL)
    {
        if(nodoPadre->left == nodoEliminar)
            nodoPadre->left = NULL;
        else
            nodoPadre->right = NULL;
    }

    //CASO 2: EL NODO TIENE UN HIJO
    else if(nodoEliminar->left == NULL || nodoEliminar->right == NULL)
    {
        if(nodoEliminar->left == NULL)
            nodoHijo = nodoEliminar->right;  
        else
            nodoHijo = nodoEliminar->left;

        if(nodoPadre->left == nodoEliminar)
            nodoPadre->left = nodoHijo;
        else
            nodoPadre->right = nodoHijo;
        nodoHijo->parent = nodoPadre;
    }
    //CASO 3: EL NODO TIENE DOS HIJOS
    else
    {
        TreeNode *auxiliar = minimum(nodoEliminar->right);
        removeNode(tree, auxiliar);
        nodoEliminar->pair = auxiliar->pair;
        
    }
    
    
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
    TreeNode *aux = tree->root;
    Pair *upperBound;
    while(aux != NULL)
    {
        Pair *par = aux->pair;
        void *keyAux = par->key;
        if( !tree->lower_than(keyAux, key) ) //si la key que busco es menor la guardo y avanzo al hijo de la izquierda
        {
            upperBound = par;
            aux = aux->left;
        }
        else //si la key que busco es mayor voy al hijo de la derecha
            aux = aux->right;
    }
    return upperBound;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *nodo = minimum(tree->root);
    tree->current = nodo;
    return nodo->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux = tree->current;
    if(aux->right != NULL)
    {
        TreeNode *minimo = minimum(aux->right);
        tree->current = minimo;
        return minimo->pair;
    }
    else
    {
        while(aux->parent != NULL && aux->parent->right == aux)
            aux = aux->parent;
        
        if(aux->parent != NULL)
        {
            tree->current = aux->parent;
            return aux->parent->pair;
        }
    }
    return NULL;
}


/*while(aux->parent != NULL)
        {
            Pair *auxPar = aux->pair;
            Pair *padrePar = aux->parent->pair;
            if(tree->lower_than(auxPar->key, padrePar->key) == 1)
            {
                tree->current = aux->parent;
                return padrePar;
            }
            aux = aux->parent;
        }*/