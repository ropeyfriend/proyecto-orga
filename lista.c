#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/**
 * Crea una lista con centinela
 * Si l es NULL interrumpe la ejecucion con un codigo LST_POSICION_INVALIDA (3)
 * Si la posicion en memoria a la que apunta l es NULL interrumpe la ejecucion con un codigo LST_ERROR_MEMORIA (4)
 * @param l un puntero a una lista
 */
void crear_lista(tLista * l) {
    (*l) = (tLista) malloc(sizeof(struct celda));

    if ((*l) == NULL)
        exit(LST_ERROR_MEMORIA);

    (*l)->elemento = NULL;
    (*l)->siguiente = NULL;
}

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

void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)) {
    if (l == NULL || p == NULL || p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    tPosicion eliminar = p->siguiente; // guardo la pos a eliminar

    p->siguiente = eliminar->siguiente;// seteo sig de p, al prox de eliminar
    fEliminar(eliminar->elemento); //llamo a eliminar del elemento
    eliminar->siguiente = NULL; //desligo al sig
    eliminar->elemento = NULL; //desligo al elem
    free(eliminar);
}

void l_destruirAux(tPosicion posicion,void (*fEliminar)(tElemento)) {
    if (posicion->siguiente == NULL) {
        fEliminar(posicion->elemento);
        posicion->elemento = NULL;
        free(posicion);
    } else {
        l_destruirAux(posicion->siguiente, fEliminar);
        posicion->siguiente = NULL;
        fEliminar(posicion->elemento);
        posicion->elemento = NULL;
        free(posicion);
    }
}

void l_destruir(tLista * l, void (*fEliminar)(tElemento)) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);
    tPosicion primera = (*l)->siguiente;
    if (primera != NULL)
        l_destruirAux(primera, fEliminar);

    (*l)->siguiente = NULL;
    (*l)->elemento = NULL;
    free((*l));

    (*l) = NULL;
}

tElemento l_recuperar(tLista l, tPosicion p) {
    if (l == NULL || p == NULL || p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    return p->siguiente->elemento;
}

tPosicion l_primera(tLista l) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);
    //devuelve el header
    return l;
}

tPosicion l_siguiente(tLista l, tPosicion p) {
    if (l == NULL || p == NULL || p->siguiente == NULL)
        exit(LST_NO_EXISTE_SIGUIENTE);
    return p->siguiente;
}

tPosicion l_anterior(tLista l, tPosicion p){
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

tPosicion l_fin(tLista l) {
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);

    tPosicion auxiliar = l;
    while(auxiliar->siguiente != NULL)
        auxiliar = auxiliar->siguiente;

    return auxiliar;
}

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