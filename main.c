#include <stdio.h>
#include "lista.h"
#include "lista.c"
#include "mapeo.c"

int main() {
    /*
    tLista listarda;
    crear_lista(&listarda);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    l_insertar(listarda, listarda, &a);
    l_insertar(listarda, listarda->siguiente, &b);
    l_insertar(listarda, listarda->siguiente->siguiente, &c);
    l_insertar(listarda, listarda->siguiente->siguiente->siguiente, &d);

    int * r1 = listarda->siguiente->elemento;
    int * r2 = listarda->siguiente->siguiente->elemento;
    int * r3 = listarda->siguiente->siguiente->siguiente->elemento;
    int * r4 = listarda->siguiente->siguiente->siguiente->siguiente->elemento;

    printf("%i \n", *r1);
    printf("%i \n", *r2);
    printf("%i \n", *r3);
    printf("%i \n", *r4);

    int * primera = l_primera(listarda)->siguiente->elemento;


    printf("\n");
    printf("%i \n", *primera);
    */

    tMapeo map;
    crear_mapeo(&map, 10, NULL, NULL);

    printf("cant elementos: %i \n", map->cantidad_elementos);

    int c = 1;
    int v = 10;

    tClave clave;
    clave = &c;

    tValor valor = malloc(sizeof(tValor));
    valor = &v;

    m_insertar(map, clave, valor);

    tEntrada * recuperado = (*(map->tabla_hash))->elemento;

    return 0;
}
