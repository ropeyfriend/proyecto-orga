#include <stdio.h>
#include "lista.h"
#include "lista.c"
#include "mapeo.c"
#include <string.h>

#define ERROR_ARCHIVO -1
#define ERROR_INVOCACION -2

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
  char *string1 = a;
  char *string2 = b;
  int toret = strcmp(string1, string2);
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
            char * c = (e)->clave;
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

void cargarPalabrasEnMapeo(tMapeo m, char file[]) {
   FILE *archivo = fopen(file,"r");

   if (archivo == NULL) {
       printf("El archivo no se pudo abrir correctamente \n");
       exit(ERROR_ARCHIVO);
   }
   char buffer[100];
   char delimitadores[] = " ,.¿?¡!\n\0";
   char * palabras_separadas;
   int * valor;
   char * linea = fgets(buffer, 100, archivo);

   while (linea != NULL) {
       palabras_separadas = strtok(linea, delimitadores);

       while(palabras_separadas != NULL) {
           char * palabra;
           palabra = strdup(palabras_separadas); //hace malloc?
           valor = m_recuperar(m, palabra);

           if (valor == NULL) {
               int * nuevo_valor = malloc(sizeof(int));
               *nuevo_valor = 1;
               m_insertar(m, palabra, nuevo_valor);
           } else {
               *valor = *(valor) + 1;
           }
           palabras_separadas = strtok(NULL, delimitadores);
       }
       linea = fgets(buffer, 100, archivo);
   }

   fclose(archivo);
}


int verificarComando(char comando[20]) {
    int corte = 0;
    if (strcmp(comando, "$ evaluador") != 0)
        corte = 1;
    else {
        printf("No se reconoce el comando \n");
        exit(ERROR_INVOCACION);
    }
    return corte;
}

void checkComando() {
    char comando[20];
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
}

int main() {
    checkComando();
    tMapeo M;
    crear_mapeo(&M,10, fHash, fComparador);
    cargarPalabrasEnMapeo(M, "C:\\UNS\\orga\\proyecto-orga\\headerlista.txt");
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





