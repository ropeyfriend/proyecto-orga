#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"

#define MAP_ERROR_MEMORIA           100

//Prototipos de funciones globales
void (*fEliminar_clave_global)(void *);
void (*fEliminar_valor_global)(void *);
void fNoEliminar(void * p) {}

/**
 * Dada una entrada elimina sus claves y valores haciendo uso de las funciones específicas para eliminarlos
 * @param entrada
 */
void fEliminar_entrada(tElemento entrada) {
    tEntrada e = (tEntrada) entrada;

    fEliminar_clave_global(e->clave);
    fEliminar_valor_global(e->valor);
    
    free(entrada);
    entrada = NULL;
}

/**
 * Multiplica por 2 el tamaño de la tabla hash del mapeo
 * @param m El mapeo
 */
void rehash(tMapeo m) {
    int longitud_vieja = (int) m->longitud_tabla;
    m->longitud_tabla = (int) m->longitud_tabla * 2; //actualizo el valor
    tLista lista;
    tPosicion pos, fin;
    int bucket;
    tEntrada e;

    tLista * nuevaTabla = malloc(m->longitud_tabla * sizeof(tLista));
    if (*nuevaTabla == NULL)
        exit(MAP_ERROR_MEMORIA);

    for(int i = 0; i < m->longitud_tabla; i++) {
        crear_lista(nuevaTabla + i);
    }

    for(int i = 0; i < longitud_vieja; i++) {
        lista = *(m->tabla_hash + i);
        pos = l_primera(lista);
        fin = l_fin(lista);
        while(pos != fin) {
            e = (tEntrada) l_recuperar(lista, pos);
            bucket = m->hash_code(e->clave) % m->longitud_tabla;
            l_insertar(*(nuevaTabla + bucket), l_primera(*(nuevaTabla+bucket)), e);
            pos = l_siguiente(lista, pos);
        }
        l_destruir(&lista, &fNoEliminar);
    }

    free(m->tabla_hash);
    m->tabla_hash = nuevaTabla;
}

/**
 Inicializa un mapeo vac�o, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves ser� computado mediante la funci�n fHash.
 A todo efecto, la comparaci�n de claves se realizar� mediante la funci�n fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_mapeo(tMapeo *m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)) {
    (*m) = (tMapeo) malloc(sizeof(struct mapeo));
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
    if(*(*m)->tabla_hash == NULL)
        exit(MAP_ERROR_MEMORIA);
    for (int i =0; i < (*m)->longitud_tabla; i++)
        crear_lista((*m)->tabla_hash + i);
}

/**
 * Inserta una entrada con clave y valor en el mapeo, si ya existe una entrada con esa clave entonces reemplaza su
 * valor y devuelve el valor antiguo, sino retorna null
 * @param m El mapeo
 * @param c Una clave a insertar
 * @param v Un valor a insertar
 * @return El valor antiguo de la entrada, si no habia entrada con esa clave retorna null
 */
tValor m_insertar(tMapeo m, tClave c, tValor v) {
    tValor toret = NULL;
    tEntrada entrada;
    int bucket = (m->hash_code(c)) % (m->longitud_tabla);
    int find = 0;

    tLista lista = *(m->tabla_hash + bucket);
    tPosicion pos = l_primera(lista);
    tPosicion fin = l_fin(lista);

    while(pos != fin && !find) {
        entrada = (tEntrada) l_recuperar(lista, pos);

        //si las claves son iguales
        if (m->comparador(c, entrada->clave) == 0) {
            toret = entrada->valor;
            entrada->valor = v;
            find = 1;
        } else
            pos = l_siguiente(lista, pos);
    }

    if (!find) {
        entrada = (tEntrada) malloc(sizeof(struct entrada));
        if (entrada == NULL)
            exit(MAP_ERROR_MEMORIA);

        entrada->clave = c;
        entrada->valor = v;

        l_insertar(lista, l_primera(lista), entrada);
        m->cantidad_elementos++;

        if ((float) (m->cantidad_elementos) / m->longitud_tabla >= 0.75)
            rehash(m);
    }

    return toret;
}

/**
 * Dada una clave elimina la entrada que lo contenga
 * @param m El mapeo
 * @param c La clave de la entrada a eliminar
 * @param fEliminarC La función escpecífica para eliminar claves
 * @param fEliminarV La función escpecífica para eliminar valores
 */
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)) {
    if (c == NULL)
        exit(MAP_ERROR_MEMORIA);

    int bucket = m->hash_code(c) % m->longitud_tabla;
    int find = 0;
    tLista lista = *(m->tabla_hash + bucket);
    tPosicion pos = l_primera(lista);
    tPosicion fin = l_fin(lista);
    tEntrada e;

    //seteo los eliminar clave y valor globales a los que me pasan por parametro
    fEliminar_clave_global = fEliminarC;
    fEliminar_valor_global = fEliminarV;

    while(pos != fin && !find) {
        e = (tEntrada) l_recuperar(lista, pos);
        if (m->comparador(c, e->clave) == 0) {
            l_eliminar(lista, pos, &fEliminar_entrada);
            m->cantidad_elementos--;
            find = 1;
        } else
            pos = l_siguiente(lista, pos);
    }
}

/**
 * Destruye el mapeo eliminando todas sus entradas
 * @param m El mapeo
 * @param fEliminarC La función escpecífica para eliminar claves
 * @param fEliminarV La función escpecífica para eliminar valores
 */
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)) {
    int longitud = (*m)->longitud_tabla;
    tLista lista;

    //seteo los eliminar clave y valor globales a los que me pasan por parametro
    fEliminar_clave_global = fEliminarC;
    fEliminar_valor_global = fEliminarV;

    for (int i = 0; i < longitud; i++) {
        lista = (tLista) *((*m)->tabla_hash+i);
        l_destruir(&lista, &fEliminar_entrada);
    }
    
    //Invalidar campos simples del mapeo
    (*m)-> cantidad_elementos = 0;
    (*m)->comparador = NULL;
    (*m)->longitud_tabla = 0;

    //Liberar espacios de los campos del tipo struct dentro del mapeo
    free((*m)->tabla_hash);
    (*m)->tabla_hash = NULL;

    //Liberar espacio del struct mapeo
    free((*m));

    //Modificar la variable
    (*m) = NULL;
}

/**
 * Dada una clave, devuelve el valor asociado a la misma
 * @param m El mapeo
 * @param c La clave
 * @return El valor asociado a la clave
 */
tValor m_recuperar(tMapeo m, tClave c) {
    int bucket = m->hash_code(c) % m->longitud_tabla;
    tValor toret = NULL;
    int find = 0;
    tEntrada e;

    tLista lista = *(m->tabla_hash+bucket);
    tPosicion pos = l_primera(lista);
    tPosicion fin = l_fin(lista);

    while(pos != fin && !find) {
        e = (tEntrada) l_recuperar(lista, pos);
        if (m->comparador(c, e->clave) == 0) {
            toret = e->valor;
            find = 1;
        } else {
            pos = l_siguiente(lista, pos);
        }
    }

    return toret;
}

