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
    if (l == NULL)
        exit(LST_POSICION_INVALIDA);

    (*l) = (tLista) malloc(sizeof(struct celda));

    if ((*l) == NULL)
        exit(LST_ERROR_MEMORIA);

    (*l)->elemento = NULL;
    (*l)->siguiente = NULL;
}

void l_insertar(tLista l, tPosicion p, tElemento e) {
    if (l->siguiente != NULL && p == NULL) {
        exit(LST_POSICION_INVALIDA);
    }

    tPosicion nuevo = (tPosicion) malloc(sizeof(struct celda));

    if (nuevo == NULL)
        exit(LST_ERROR_MEMORIA);

    nuevo->elemento = e;
    nuevo->siguiente = p->siguiente;
    p->siguiente = nuevo;
}

tPosicion l_primera(tLista l) {
    if(l == NULL)
        exit(LST_POSICION_INVALIDA);

    tPosicion toret = l->siguiente;

    if (l->siguiente == NULL)
        toret = l;

    return toret;
}