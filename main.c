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

int main() {
    tMapeo map;
    crear_mapeo(&map, 10, fHash, fComparador);

    printf("cant elementos: %i \n", map->cantidad_elementos);

    int c1 = 1;
    int v1 = 10;

    int c2 = 2;
    int v2 = 20;

    int c3 = 3;
    int v3 = 30;

    int c4 = 10;
    int v4 = 11;

    m_insertar(map, &c1, &v1);
    m_insertar(map, &c2, &v2);
    m_insertar(map, &c3, &v3);
    m_insertar(map, &c4, &v4);

    tValor recuperado1 = m_recuperar(map, (tClave) &c1);
    tValor recuperado2 = m_recuperar(map, (tClave) &c2);
    tValor recuperado3 = m_recuperar(map, (tClave) &c3);

    int * p1 = recuperado1;
    int * p2 = recuperado2;
    int * p3 = recuperado3;

    printf("%i \n", *p1);
    printf("%i \n", *p2);
    printf("%i \n", *p3);

    printf("hc1 %i \n", fHash(&c1));

    printf("cant elementos: %i \n", map->cantidad_elementos);

    printf("\n");

    mostrarBuckets(map);

    return 0;
}
