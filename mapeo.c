#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"

#define MAP_ERROR_MEMORIA           100

//HAY QUE LIBERAR MEMORIA?????????????????
void resize(tMapeo m) {
    int longitud_vieja = (int) m->longitud_tabla;
    m->longitud_tabla = (int) m->longitud_tabla * 2;

    tLista * nueva = malloc(m->longitud_tabla * sizeof(tLista));
    for(int i = 0; i < m->longitud_tabla; i++)
        crear_lista(m->tabla_hash + i);

    for(int i = 0; i < longitud_vieja; i++) {
        tLista lista = *(m->tabla_hash) + i;
        tPosicion pos = l_primera(lista);

        while(pos != l_fin(lista)) {
            tEntrada e = (tEntrada) l_recuperar(lista, pos);
            int bucket = m->hash_code(e->clave);
            l_insertar(*(nueva + bucket), l_primera(*(nueva+bucket)), e);
        }
    }

    m->tabla_hash = nueva;
}

void crear_mapeo(tMapeo *m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)) {
    (*m) = (tMapeo) malloc(sizeof(struct entrada));
    if (*m == NULL)
        exit(MAP_ERROR_MEMORIA);

    if (ci < 10)
        (*m)->longitud_tabla = 10;
    else
        (*m)->longitud_tabla = ci;

    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;

    (*m)->tabla_hash = malloc((*m)->longitud_tabla * sizeof(tLista));

    for (int i =0; i < (*m)->longitud_tabla; i++)
        crear_lista((*m)->tabla_hash + i);
}

tValor m_insertar(tMapeo m, tClave c, tValor v) {
    tValor toret = NULL;
    int bucket = m->hash_code(c) % m->longitud_tabla;
    int find = 0;

    tLista lista = *(m->tabla_hash + bucket);
    tPosicion pos = l_primera(lista);
    tPosicion pos_fin = l_fin(lista);

    while(pos != pos_fin && !find) {
        tEntrada entrada = (tEntrada) l_recuperar(lista, pos);
        int * clave = entrada->clave;
        int * valor = entrada->valor;
        printf("clave: %i valor: %i \n", *clave, *valor);

        //si las claves son iguales
        if (m->comparador(c, entrada->clave) == 0) {
            toret = entrada->valor;
            entrada->valor = v;
            find = 1;
        } else
            pos = l_siguiente(lista, pos);
    }

    if (!find) {
        if (m->cantidad_elementos / m->longitud_tabla >= 0.75) {
            //resize()
        }
        tEntrada entrada = (tEntrada) malloc(sizeof(struct entrada));
        if (entrada == NULL)
            exit(MAP_ERROR_MEMORIA);

        entrada->clave = c;
        entrada->valor = v;

        l_insertar(lista, l_primera(lista), entrada);
        m->cantidad_elementos++;
    }

    return toret;
}

tValor m_recuperar(tMapeo m, tClave c) {
    int bucket = m->hash_code(c) % m->longitud_tabla;
    tValor toret = NULL;
    int find = 0;

    tLista lista = *(m->tabla_hash+bucket);
    tPosicion pos = l_primera(lista);

    while(pos != l_fin(lista) && !find) {
        tEntrada e = (tEntrada) l_recuperar(lista, pos);
        if (m->comparador(c, e->clave) == 0) {
            toret = e->valor;
            find = 1;
        } else {
            pos = l_siguiente(lista, pos);
        }
    }

    return toret;
}

void mostrarBuckets(tMapeo m) {
    for (int bucket = 0; bucket < m->longitud_tabla; bucket++) {
        tLista lista = *(m->tabla_hash + bucket);

        tPosicion pos = l_primera(lista);
        while (pos != l_fin(lista)) {
            tEntrada e = (tEntrada) l_recuperar(lista, pos);

            int * c = (int *) (e)->clave;
            int * v = (int *) (e)->valor;
            printf("c:%i v:%i |", *c, *v);

            pos = l_siguiente(lista, pos);
        }
        printf("-eolist \n");
    }
}