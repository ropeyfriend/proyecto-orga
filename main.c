#include <stdio.h>
#include "lista.h"
#include "lista.c"
#include "mapeo.c"
#include <string.h>
#include <ctype.h>

int fHash(void * p) {
    char * palabra = (char *) p;
    int letraAscii;
    int sumaAscii = 0;

    for(int i=0; i<strlen(palabra); i++) {
        letraAscii = (int) palabra[i];
        sumaAscii += letraAscii + i;
    }

    return sumaAscii;
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
            char * c = (char *) (e)->clave;
            int * v = (int *) (e)->valor;
            printf("[c:%s v:%i] | ", c, *v);

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

int sumarAscii(char palabra[20]);
void contarPalabras(tMapeo M, char ruta[20]);
int verificarComando(char comando[20]);

int sumarAscii(char palabra[20]) {


}

void contarPalabras(tMapeo M, char ruta[20]) {
    char palabra[20];
    char rutaCompleta [60];
    char ruta2 [] = "D:\\OLC\\proyecto-orga\\";
    strcpy(rutaCompleta, ruta2);
    strcat(rutaCompleta, ruta);

    FILE * archivo = fopen(rutaCompleta, "r");

    fflush(stdin);
    if (archivo == NULL) {
        printf("Error de apertura del archivo. \n");
    } else {
        char c;
        memset(palabra, 0, strlen(palabra));
        while (!feof(archivo)) {
            int cont = 0;
            while((c = fgetc(archivo)) != EOF) {

                if(c == ' ' || c == '.' || c == '\n') {
                    cont = 0;

                    for(int i = 0; palabra[i]; i++){
                        palabra[i] = tolower(palabra[i]); // Pasa las mayusculas de las palabras a minusculas
                    }

                    tClave clave = malloc(20 * sizeof(char));
                    clave = palabra;
                    printf("Clave : %s\n",clave);

                    if(m_recuperar(M,&clave) == NULL) {
                        printf("ESTOY ADENTRO DEL IFARDO DEL NULL\n");
                        int valor = 1;
                        m_insertar(M,&clave,(tValor) &valor); // Si la palabra no estaba, ingresa al mapeo con cantidad 1
                        printf("Despues del insertar\n");
                    } else {
                        printf("ESTOY ADENTRO DEL IFARDO DEL ELSE\n");
                        int * nuevoValor = (int *) m_recuperar(M,&clave); // Si la palabra ya estaba, incrementa la cantidad en 1
                        m_insertar(M,&clave,(tValor) nuevoValor+1);
                    }

                    memset(palabra, 0, strlen(palabra)); // Borra la palabra
                } else {
                    palabra[cont] = c;
                    cont++;
                }
            }
        }
    }
    fclose(archivo);
}

int verificarComando(char comando[20]) {
    int corte = 0;
    if(comando[0] != '$' || comando[1] != ' ' ||comando[2] != 'e' || comando[3] != 'v'
       || comando[4] != 'a' || comando[5] != 'l'
       || comando[6] != 'u' || comando[7] != 'a' || comando[8] != 'd' || comando[9] != 'o'
       || comando[10] != 'r' || comando[11] != ' ') {
        corte = 1;
    }
    return corte;
}

int main() {
    //list_test();
    //map_test();

    int cantidadApariciones = 0;
    int operacion = 0;
    char comando[20];
    char palabra[20];
    char ruta [20];
    fflush(stdin);
    printf("Ingrese comando del sistema. \n");
    scanf("%[^\n]", comando);

    int valido = verificarComando(comando);

    int corte = 0;
    int j = 0;
    for(int i=12; corte != 1; i++) {
        ruta[j] = comando[i];
        if(comando[i] == '.') {
            corte = 1;
        }
        j++;
    }

    ruta[j] = 't';
    ruta[j+1] = 'x';
    ruta[j+2] = 't';

    tMapeo M;
    crear_mapeo(&M,10, fHash, fComparador);
    contarPalabras(M, ruta);

    // printf("Ingrese la palabra que desea buscar : \n");
    // scanf("%[^\n]", palabra);
    // printf("%s\n", palabra);

    mostrarBuckets(M);

    //int clave = sumarAscii(palabra);
    //printf("%i\n",clave);

    //int * valor = (int *) m_recuperar(M,&clave);

    //printf("Cantidad apariciones : %i\n",*valor);


    /*
    if(valido == 1){
        printf("Comando invalido. \n");
    } else {
        printf("Ha ingresado correctamente al sistema. \n");
        printf("Ingrese la operacion a realizar. \n");
        printf("1: Contar apariciones \n2: Salir \n");
        scanf("%i", &operacion);

        if(operacion == 1) {
            printf("Ingrese la palabra que desea buscar : \n");
            scanf("%s", palabra);

            cantidadApariciones = contarApariciones(palabra);
            printf("La palabra %s se encuentra %i veces en el archivo %s", palabra, cantidadApariciones, ruta);

        } else {
            if (operacion == 2) {
                printf("Cerrando sistema. \n");
            } else {
                printf("Operacion invalida. \n");
            }
        }
    }
    */

    return 0;
}





