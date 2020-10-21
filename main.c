#include <stdio.h>
#include "lista.h"
#include "lista.c"
#include "mapeo.c"

int fHash(void * p){
    int * entero = p;
    int toret = 0;

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
    free(c);
}

void fEliminarV(tValor v) {
    v = NULL;
    free(v);
}

void list_test() {
    tLista lista;
    crear_lista(&lista);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    l_insertar(lista, l_primera(lista), &d);
    l_insertar(lista, l_primera(lista), &c);
    l_insertar(lista, l_primera(lista), &b);
    l_insertar(lista, l_primera(lista), &a);

    printf("---------------------\n");
    tPosicion pos = l_siguiente(lista, l_primera(lista));
    int * entero = pos->elemento;
    printf("%i \n", *entero);
    printf("---------------------\n");

    pos = l_primera(lista);
    while (pos != l_fin(lista)) {
        int * x = l_recuperar(lista, pos);
        printf("%i \n", *x);
        pos = l_siguiente(lista, pos);
    }

    printf("\n");

    l_eliminar(lista, l_siguiente(lista, l_primera(lista)), fEliminarV);

    pos = l_primera(lista);
    while (pos != l_fin(lista)) {
        int * x = l_recuperar(lista, pos);
        printf("%i \n", *x);
        pos = l_siguiente(lista, pos);
    }
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

    m_insertar(map, &c1, &v1);
    m_insertar(map, &c2, &v2);
    m_insertar(map, &c3, &v3);
    m_insertar(map, &c4, &v4);
    m_insertar(map, &c5, &v5);

    printf("elementos insertados \n");

    printf("cant elementos: %i \n", map->cantidad_elementos);

    printf("\n");

    mostrarBuckets(map);

    printf("\n");
    int * valorovich = m_recuperar(map, &c5);
    printf("valorovich: %i \n", *valorovich);

    printf("\n");
    m_eliminar(map, &c4, fEliminarC, fEliminarV);
    m_eliminar(map, &c1, fEliminarC, fEliminarV);

    mostrarBuckets(map);

}

int main() {
    list_test();
    //map_test();

    return 0;
}
