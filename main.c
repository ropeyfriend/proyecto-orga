#include <stdio.h>
#include "lista.h"
#include "lista.c"

int main() {
    tLista lista;           //variable
    crear_lista(&lista);    //variable --> celda[elemento: null, siguiente: null]
    tElemento p1 = "tincho";
    tElemento p2 = "cache";
    tElemento p3 = "niko";

    l_insertar(lista, l_primera(lista), p3);
    l_insertar(lista, l_primera(lista), p2);
    l_insertar(lista, l_primera(lista), p1);

    printf("%s \n", l_primera(lista)->elemento);
    printf("%s \n", l_primera(lista)->siguiente->elemento);
    printf("%s \n", l_primera(lista)->siguiente->siguiente->elemento);

    return 0;
}
