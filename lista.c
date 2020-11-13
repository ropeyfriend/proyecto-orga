#include <stdio.h>
#include <stdlib.h>

#include "lista.h"

/**
 * Inicializa una lista vacía con posición indirecta y celda centinela (header)
 * @param l es un puntero a la lista
 */
void crear_lista(tLista * l) {
    (*l) = (tLista) malloc(sizeof(struct celda));

    if ((*l) == NULL)
        exit(LST_ERROR_MEMORIA);

    (*l)->elemento = NULL;
    (*l)->siguiente = NULL;
}

/**
 * Inserta un elemento a la lista en una posición específica
 * @param l una lista
 * @param p una posición a insertar el elemento
 * @param e un elemento a insertar
 */
void l_insertar(tLista l, tPosicion p, tElemento e) {
    if (l == NULL || p == NULL) {
        exit(LST_POSICION_INVALIDA);
    }

    tPosicion nuevo = (tPosicion) malloc(sizeof(struct celda));
    if (nuevo == NULL)
        exit(LST_ERROR_MEMORIA);

    nuevo->elemento = e;
    nuevo->siguiente = p->siguiente;
    p->siguiente = nuevo;
}

/**
 * Dada una posicion elimina el elemento de la lista en esa posición
 * @param l Una lista
 * @param p Una posición de la lista
 * @param fEliminar La función escpecífica para eliminar elementos de la lista
 */
void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)) {
    if (l == NULL || p == NULL || p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);

    tPosicion posEliminar = p->siguiente;

    p->siguiente = posEliminar->siguiente;
    fEliminar(posEliminar->elemento);

    posEliminar->siguiente = NULL;
    posEliminar->elemento = NULL;
    
    free(posEliminar);
    posEliminar = NULL;
}

/**
 * Función auxiliar recursiva que dada una posición se encarga de eliminar todos los elementos de una lista
 * @param pos La primer posición de una lista
 * @param fEliminar La función escpecífica para eliminar elementos de la lista
 */
void destruir_aux(tPosicion pos, void (*fEliminar)(tElemento)) {
    if (pos->siguiente != NULL)
        destruir_aux(pos->siguiente, fEliminar);
    fEliminar(pos->elemento);
    pos->elemento = NULL;
    pos->siguiente = NULL;
    free(pos);
}

/**
 * Destruye la lista, eliminando todas sus posiciones.
 * @param l La lista a eliminar
 * @param fEliminar La función escpecífica para eliminar elementos de la lista
 */
void l_destruir(tLista * l, void (*fEliminar)(tElemento)) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);

    tPosicion pos = (*l);
    if (pos->siguiente != NULL)
        destruir_aux(pos->siguiente, fEliminar);

    //header
    (*l)->siguiente = NULL;
    (*l)->elemento = NULL;
    free(*l);
    (*l) = NULL;
}

/**
 * Devuelve el elemento de la posición indicada en la lista.
 * @param l La lista que contiene la posición
 * @param p Una posición de la lista
 * @return El elemento en la posición
 */
tElemento l_recuperar(tLista l, tPosicion p) {
    if (l == NULL || p == NULL || p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    return p->siguiente->elemento;
}

/**
 * Devuelve la primer posición de la lista
 * @param l La lista
 * @return La primer posición
 */
tPosicion l_primera(tLista l) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);
    //devuelve el header
    return l;
}

/**
 * Devuelve la posición que le sigue a la indicada por parámetro
 * @param l La lista
 * @param p La posición
 * @return La siguiente posición a la indicada por parámetro
 */
tPosicion l_siguiente(tLista l, tPosicion p) {
    if (l == NULL || p == NULL || p->siguiente == NULL)
        exit(LST_NO_EXISTE_SIGUIENTE);
    return p->siguiente;
}

/**
 * Devuelve la posición anterior a la indicada por parámetro
 * @param l La lista
 * @param p La posición
 * @return La posición anterior a la indicada por parámetro
 */
tPosicion l_anterior(tLista l, tPosicion p) {
    int find = 0;
    tPosicion pos = l;
    if (p == pos)
        exit(LST_NO_EXISTE_ANTERIOR);

    while(pos->siguiente != NULL && !find) {
        if (pos->siguiente == p)
            find = 1;
        else
            pos = pos->siguiente;
    }
    return pos;
}

/**
 * Devuelve la última posición de la lista
 * @param l La lista
 * @return La última posición
 */
tPosicion l_ultima(tLista l) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);

    tPosicion auxiliar = l;
    if (l->siguiente != NULL) {
        while(auxiliar->siguiente->siguiente != NULL)
            auxiliar = auxiliar->siguiente;
    }
    return auxiliar;
}

/**
 * Devuelve la posición final de la lista
 * @param l La lista
 * @return La posición final
 */
tPosicion l_fin(tLista l) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);

    tPosicion auxiliar = l;
    while(auxiliar->siguiente != NULL)
        auxiliar = auxiliar->siguiente;

    return auxiliar;
}
/**
 * Devuelve la longitud de la lista
 * @param l La lista
 * @return La longitud de la lista
 */
int l_longitud(tLista l) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);
    int size=0;
    if (l->siguiente != NULL) {
        tPosicion auxiliar = l->siguiente;
        size = 1;
        while(auxiliar->siguiente != NULL){
            auxiliar=auxiliar->siguiente;
            size=size+1;
        }
    }
    return size;
}
