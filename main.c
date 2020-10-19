#include <stdio.h>
#include "lista.h"
#include "lista.c"
#include "mapeo.c"

int main() {
    /*
    tLista lista;           //variable
    crear_lista(&lista);    //variable --> celda[elemento: null, siguiente: null]

    tElemento p1 = "tincho";
    tElemento p2 = "cache";
    tElemento p3 = "niko";


    l_insertar(lista, l_primera(lista), p3);
    l_insertar(lista, l_primera(lista), p2);
    l_insertar(lista, l_primera(lista), p1);

    printf("0: %s \n", lista->elemento);
    printf("1: %s \n", l_primera(lista)->siguiente->elemento);
    printf("2: %s \n", l_primera(lista)->siguiente->siguiente->elemento);
    printf("3: %s \n", l_primera(lista)->siguiente->siguiente->siguiente->elemento);
    */

    tMapeo mapardox;
    crear_mapeo(&mapardox,10,NULL,NULL);
    printf("%i \n",mapardox->cantidad_elementos);
    tClave c = "tincho";
    tValor v = "tincharda";
    m_insertar(mapardox,c,v);

    printf("%i \n",mapardox->cantidad_elementos);
    printf("%s \n",l_primera(mapardox->tabla_hash)->elemento);


    return 0;
}
