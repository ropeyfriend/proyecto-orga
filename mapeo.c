#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"

#define MAP_ERROR_MEMORIA           100

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)) {
    (*m) = (tMapeo) malloc(sizeof(struct entrada));

    if(*m == NULL) {
        exit(MAP_ERROR_MEMORIA);
    }

    crear_lista((*m)->tabla_hash);

    if(ci < 10) {
        (*m)->longitud_tabla = 10;
    } else {
        (*m)->longitud_tabla = ci;
    }
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
}

tValor m_insertar(tMapeo m, tClave c, tValor v) {
    if (m->cantidad_elementos/ m->longitud_tabla > 0.75 * m->longitud_tabla) {
        printf("is rompido (>factor de carga)\n");
    } else {
        l_insertar(*m->tabla_hash, l_primera(*m->tabla_hash), v);
        m->cantidad_elementos = m->cantidad_elementos + 1;
    }
}