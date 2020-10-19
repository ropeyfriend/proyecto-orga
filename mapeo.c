#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"

#define MAP_ERROR_MEMORIA           100

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)) {
    (*m) = (tMapeo)malloc(sizeof(struct entrada));

    if(*m == NULL) {
        exit(MAP_ERROR_MEMORIA);
    }

    (*m)->tabla_hash = (tLista) malloc(sizeof(struct celda));

    if(ci < 10) {
        (*m)->longitud_tabla = 10;
    } else {
        (*m)->longitud_tabla = ci;
    }

    (*m)->cantidad_elementos = 0;

    (*m)->hash_code = fHash;

    (*m)->comparador = fComparacion;
}