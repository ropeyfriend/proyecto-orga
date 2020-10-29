#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> //pasa string a lowercase
#include "lista.h"
#include "mapeo.h"

#define ERROR_ARCHIVO -1
#define ERROR_INVOCACION -2

/**
 * Dado un puntero a un caracter (String) devuelve un entero a partir de la suma de sus valores ASCII
 * @param p Un puntero a un caracter
 * @return Un entero
 */
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

/**
 * Compara dos punteros a caracteres (Strings) a y b,
 * y retorna un entero < 0 si a < b, un entero > 0 si a > b y retorna 0 si a = b
 * @param a Un puntero a un caracter
 * @param b Un puntero a un caracter
 * @return Un entero
 */
int fComparador(void * a, void * b) {
  char *string1 = a;
  char *string2 = b;
  int toret = strcmp(string1, string2);
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

void mostrarMapeo(tMapeo m) {

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

void cargarPalabrasEnMapeo(tMapeo m, char file[]) {
   FILE *archivo = fopen(file,"r");
   if (archivo == NULL) {
       printf("Error (-1): El archivo no se pudo abrir correctamente \n");
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
           palabra = strdup(palabras_separadas);
           valor = m_recuperar(m, palabra);

           for(int i = 0; palabra[i]; i++){
               palabra[i] = tolower(palabra[i]); // Pasa las mayusculas de las palabras a minusculas
           }

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

int cantApariciones(tMapeo m, char * palabra) {
    int toret = 0;
    tValor valor = m_recuperar(m, palabra);
    if (valor != NULL)
        toret = *((int*) valor);
    return toret;
}

int evaluador(char *path) {
    tMapeo m;
    crear_mapeo(&m,10, fHash, fComparador);
    cargarPalabrasEnMapeo(m, path);
    //mostrarMapeo(m);
    int opcion = 0;
    char * palabra;

    while(opcion != 2) {
        opcion = 0;
        printf("(1) Contar apariciones de una palabra \n");
        printf("(2) Salir \n");

        scanf("%i", &opcion);
        if (opcion != 1 && opcion != 2) {
            printf("Error (-2): opcion erronea \n");
            exit(ERROR_INVOCACION);
        }
        if(opcion == 1) {
            printf("Ingresar palabra ");
            palabra = (char *) malloc(30);
            scanf("%s",palabra);
            printf("\nLa palabra '%s' se encuentra %i veces en el archivo \n", palabra, cantApariciones(m, palabra));
            free(palabra);
            fflush(stdin);
        }

    }
    printf("\n");
    //mostrarMapeo(m);
    m_destruir(&m, &fEliminarC, &fEliminarV); //liberamos la memoria cuando salimos
    return 0;
}

int main(int args, char **path) {
    evaluador(*(path+1));
    return 0;
}