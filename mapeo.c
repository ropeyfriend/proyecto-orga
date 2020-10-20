#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"

#define MAP_ERROR_MEMORIA           100

/* EXPLICACION FEDE:
 * Este campo del mapeo define un ARREGLO de listas. Noten que aquí NO se reserva memoria para
 * cada componente del arreglo, esto se deberá hacer cuando se crea el mapeo.
 * tLista * tabla_hash;
 * Esta definición permite manipular mi_arreglo_enteros y mi_arreglo_enteros_2 de forma similar
 * Tener en cuenta que la reservación de memoria NO es equivalente.
 * int [10] mi_arreglo_enteros;
 * int * mi_arreglo_enteros_2 = malloc(10 * sizeof(int))
 *
 * Ejemplos de la equivalencia en el manejo de las variables
 * mi_arreglo_enteros[0] = 10;
 * mi_arreglo_enteros_2 = 10;
 * mi_arreglo_enteros[5] = 50;
 *
 * *(mi_arreglo_enteros_2 +5) = 50;
 * mi_arreglo_enteros[i] = x; es equivalente a *(mi_arreglo_enteros_2+i) = x;
 */

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

    for (int i =0; i < (*m)->longitud_tabla; i++) {
        crear_lista((*m)->tabla_hash + i);
    }
}

/* EXPLICACION FEDE 2:
 * La signatura de la función recuperar de la lista es la siguiente:
 * 1) tElemento l_recuperar(tLista l, tPosicion p);
 * 2) void * l_recuperar(struct celda * l, struct celda * p);
 * Desde el cliente:
 * tLista mi_lista;
 * tElemento mi_elemento;
 * int mi_entero = 10;
 *
 * crear_lista(&mi_lista);
 * l_insertar(mi_lista, l_primera(mi_lista), &mi_entero);
 *
 * mi_elemento = l_recuperar(mi_lista, l_primera(mi_lista));
 * int * otro_entero = (int *) l_recuperar(mi_lista, l_primera(mi_lista));
 *                    unico casteo ever!!
*/
tValor m_insertar(tMapeo m, tClave c, tValor v) {
    if (m->cantidad_elementos / m->longitud_tabla > 0.75 * m->longitud_tabla) {
        printf("is rompido (>factor de carga)\n");
    } else {
        tEntrada entrada = (tEntrada) malloc(sizeof(struct entrada));
        entrada->clave = c;
        entrada->valor = v;

        //para acceder a una lista del "arreglo" tabla_hash habria que hacer:
        // (*(m->tabla_hash + i)) = celda { elemento , siguiente }
        l_insertar(*(m->tabla_hash), l_primera(*(m->tabla_hash)), entrada);
        tEntrada * e = l_recuperar(*(m->tabla_hash), l_primera(*(m->tabla_hash)));

        int * r1 = (*e)->clave;
        printf("gas");

        printf("%i \n", *r1);

        m->cantidad_elementos = m->cantidad_elementos + 1;
    }
    return 0;
}

tValor m_recuperar(tMapeo m, tClave c) {

}