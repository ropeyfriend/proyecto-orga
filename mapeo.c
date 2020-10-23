#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"

#define MAP_ERROR_MEMORIA           100

void rehash(tMapeo m) {
    int longitud_vieja = (int) m->longitud_tabla;
    m->longitud_tabla = (int) m->longitud_tabla * 2;

    tLista * nueva = malloc(m->longitud_tabla * sizeof(tLista));
    for(int i = 0; i < m->longitud_tabla; i++) {
        crear_lista(nueva + i);
    }

    for(int i = 0; i < longitud_vieja; i++) {
        tLista lista = *(m->tabla_hash + i);
        tPosicion pos = l_primera(lista);
        while(pos != l_fin(lista)) {
            tEntrada e = (tEntrada) l_recuperar(lista, pos);
            int bucket = m->hash_code(e->clave) % m->longitud_tabla;
            l_insertar(*(nueva + bucket), l_fin(*(nueva+bucket)), e);
            pos = l_siguiente(lista, pos);
        }

        lista->siguiente = NULL;
        lista->elemento = NULL;
        lista = NULL;
        free(lista);
    }

    m->tabla_hash = nueva;
}

/**
 Inicializa un mapeo vac�o, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves ser� computado mediante la funci�n fHash.
 A todo efecto, la comparaci�n de claves se realizar� mediante la funci�n fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_mapeo(tMapeo *m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)) {
    //printf("------MALLOC-CREAR-MAPEO------- \n");
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

/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no exist�a en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
tValor m_insertar(tMapeo m, tClave c, tValor v) {
    tValor toret = NULL;
    int bucket = (m->hash_code(c)) % (m->longitud_tabla);
    int find = 0;

    tLista lista = *(m->tabla_hash + bucket);
    tPosicion pos = l_primera(lista);
    tPosicion pos_fin = l_fin(lista);

    while(pos != pos_fin && !find) {
        tEntrada entrada = (tEntrada) l_recuperar(lista, pos);
        int * clave = entrada->clave;
        int * valor = entrada->valor;

        //si las claves son iguales
        if (m->comparador(c, entrada->clave) == 0) {
            toret = entrada->valor;
            entrada->valor = v;
            find = 1;
        } else
            pos = l_siguiente(lista, pos);
    }

    if (!find) {
        if ((m->cantidad_elementos + 1.0 ) / m->longitud_tabla >= 0.75) {
            printf("RESIZE \n");
            rehash(m);
            bucket = m->hash_code(c) % m->longitud_tabla;
            lista = *(m->tabla_hash + bucket);
        }

        tEntrada entrada = (tEntrada) malloc(sizeof(struct entrada));
        if (entrada == NULL)
            exit(MAP_ERROR_MEMORIA);

        entrada->clave = c;
        entrada->valor = v;

        //printf("adding: [%i][%i] en bucket %i \n", *((int* ) c), *((int*) v), bucket);

        l_insertar(lista, l_fin(lista), entrada);

        m->cantidad_elementos++;
    }

    return toret;
}

void m_eliminar_entrada_aux(tElemento entrada) {
    entrada = NULL;
    free(entrada);
}

/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)) {
    int bucket = m->hash_code(c) % m->longitud_tabla;
    tLista lista = *(m->tabla_hash + bucket);
    int find = 0;
    tPosicion pos = l_primera(lista);
    while(pos != l_fin(lista) && !find) {
        tEntrada e = (tEntrada) l_recuperar(lista, pos);
        if (m->comparador(c, e->clave) == 0) {
            fEliminarC(e->clave);
            fEliminarV(e->valor);
            int * clave = e->clave;
            int * valor = e->valor;
            printf("removing: [%i , %i] \n", *clave, *valor);
            l_eliminar(lista, pos, m_eliminar_entrada_aux);
            m->cantidad_elementos--;
            find = 1;
        } else
            pos = l_siguiente(lista, pos);
    }
}

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)) {
    for (int i = 0; i < (*m)->longitud_tabla; i++) {
        tLista lista = *((*m)->tabla_hash + i);
        tPosicion pos = l_primera(lista);
        int final = 0;

        while(final == 0 && pos != l_fin(lista)) {
            tEntrada entrada = (tEntrada) l_recuperar(lista, pos);
            int * clave = entrada->clave;
            int * valor = entrada->valor;

            fEliminarC(entrada->clave);
            fEliminarV(entrada->valor);
            l_eliminar(lista, pos, m_eliminar_entrada_aux);

            (*m)->cantidad_elementos--;
            if (pos == l_fin(lista))
                final = 1;
        }
        lista->siguiente = NULL;
        lista->elemento = NULL;
        lista = NULL;
        free(lista);
        //printf("------FREE-DESTRUIR-LISTA-EN-MAPEO------- \n");
    }
}

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/
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

