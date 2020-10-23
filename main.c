#include <stdio.h>
#include "lista.h"
#include "lista.c"
#include "mapeo.c"

int fHash(void * p) {
    int * entero = p;
    int toret;

    if (*entero >= 10)
        toret = (*entero) / 10;
    else
        toret = *entero;
    return toret;
}

int fComparador(void * a, void * b) {
    int * pa = a;
    int * pb = b;
    int toret = 0;

    if (*pa > *pb)
        toret = 1;
    else if (*pa < *pb)
        toret = -1;

    return toret;
}

void fEliminarC(tClave c) {
    c = NULL;
    //printf("------FREE-ELIMINAR-CLAVE------- \n");
    free(c);
}

void fEliminarV(tValor v) {
    v = NULL;
    //printf("------FREE-ELIMINAR-VALOR------- \n");
    free(v);
}

void fEliminarEntero(void * entero) {
    entero = NULL;
    printf("------FREE-ELIMINAR-ENTERO------- \n");
    free(entero);
}

void mostrarBuckets(tMapeo m) {
    for (int bucket = 0; bucket < m->longitud_tabla; bucket++) {
        tLista lista = *(m->tabla_hash + bucket);

        tPosicion pos = l_primera(lista);
        while (pos != l_fin(lista)) {
            tEntrada e = (tEntrada) l_recuperar(lista, pos);
            int * c = (int *) (e)->clave;
            int * v = (int *) (e)->valor;
            printf("[c:%i v:%i] | ", *c, *v);

            pos = l_siguiente(lista, pos);
        }
        printf("-eolist \n");
    }
}

void list_test() {
    tLista lista;
    crear_lista(&lista);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    int *d = malloc(sizeof(int));
    //printf("------MALLOC-ENTERO------- \n");
    //printf("------MALLOC-ENTERO------- \n");
    //printf("------MALLOC-ENTERO------- \n");
    //printf("------MALLOC-ENTERO------- \n");

    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    l_insertar(lista, l_fin(lista), a);
    l_insertar(lista, l_fin(lista), b);
    l_insertar(lista, l_fin(lista), c);
    l_insertar(lista, l_fin(lista), d);

    printf("\n");
    printf("----------------------- \n");
    tPosicion pos = l_primera(lista);
    while (pos != l_fin(lista)) {
        int * x = l_recuperar(lista, pos);
        printf("%i ", *x);
        pos = l_siguiente(lista, pos);
    }
    printf("-eolist\n");
    printf("------------------------ \n");
    printf("\n");

    l_destruir(&lista, fEliminarEntero);
    printf("l_destruir() \n");

    printf("\n");
    printf("\n ----------------------- \n");
    pos = l_primera(lista);
    while (pos != l_fin(lista)) {
        int * x = l_recuperar(lista, pos);
        printf("%i ", *x);
        pos = l_siguiente(lista, pos);
    }
    printf("-eolist\n");
    printf("----------------------- \n");
    printf("\n");
}

void map_test() {
    tMapeo map;
    crear_mapeo(&map, 10, fHash, fComparador);

    printf("cant elementos: %i \n", map->cantidad_elementos);

    int c1 = 1;
    int v1 = 1;

    int c2 = 11;
    int v2 = 11;

    int c3 = 21;
    int v3 = 21;

    int c4 = 100;
    int v4 = 100;

    int c5 = 10;
    int v5 = 10;

    int c6 = 1000;
    int v6 = 101;

    int c7 = 10000;
    int v7 = 102;

    int c8 = 10000; //repetido
    int v8 = 1480;

    int c9 = 1500;
    int v9 = 150;


    m_insertar(map, &c1, &v1);
    m_insertar(map, &c2, &v2);
    m_insertar(map, &c3, &v3);
    m_insertar(map, &c4, &v4);
    m_insertar(map, &c5, &v5);
    m_insertar(map, &c6, &v6);
    m_insertar(map, &c7, &v7);
    m_insertar(map, &c8, &v8);
    m_insertar(map, &c9, &v9);

    printf("cant elementos: %i \n", map->cantidad_elementos);
    printf("longitud tabla: %i \n", map->longitud_tabla);
    printf("\n");
    mostrarBuckets(map);
    printf("cant elementos: %i \n", map->cantidad_elementos);
    printf("\n");

    /*
    printf("\n");
    m_eliminar(map, &c4, fEliminarC, fEliminarV);
    m_eliminar(map, &c1, fEliminarC, fEliminarV);
    */

    m_destruir(&map, fEliminarC, fEliminarV);
    printf("m_destruir() \n");
    printf("\n");
    mostrarBuckets(map);
    printf("cant elementos: %i \n", map->cantidad_elementos);

}

int main() {
    //list_test();
    map_test();
    return 0;
}
