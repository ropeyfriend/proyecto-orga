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

/**
 * Dada una clave la elimina
 * @param c La clave a eliminar
 */
void fEliminarC(tClave c) {
    free(c);
    c = NULL;
}
/**
 * Dado un valor lo elimina
 * @param v El valor a eliminar
 */
void fEliminarV(tValor v) {
    free(v);
    v = NULL;
}



/**
 * Dado un mapeo y el nombre de un archivo de texto, selecciona todas las palabras del mapeo y genera entradas, donde:
 * las claves son las palabras y los valores la cantidad de apariciones de la misma en el archivo de texto
 *
 * Si el nombre del archivo no pertenece a un archivo existente en el directorio del ejecutable retorna un error (-1)
 *
 * @param m El mapeo a operar
 * @param file El archivo donde se tomarán las palabras
 */
void cargarPalabrasEnMapeo(tMapeo m, char file[]) {
   FILE *archivo = fopen(file,"r");
   if (archivo == NULL) {
       printf("Error (-1): El archivo no se pudo abrir correctamente \n");
       exit(ERROR_ARCHIVO);
   }

   char buffer[100];
   //char * buffer = "";

   char delimitadores[] = " ,.¿?¡!\n\0";
   char * palabras_separadas;
   int * valor;
   char * linea = fgets(buffer, 100, archivo);
   char * palabra;
   int * nuevo_valor;

   while (linea != NULL) {
       palabras_separadas = strtok(linea, delimitadores);

       while(palabras_separadas != NULL) {
           palabra = strdup(palabras_separadas);
           valor = m_recuperar(m, palabra);

           for(int i = 0; palabra[i]; i++){
               palabra[i] = tolower(palabra[i]); // Pasa las mayusculas de las palabras a minusculas
           }

           if (valor == NULL) {
               nuevo_valor = malloc(sizeof(int));
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

/**
 * Dado un mapeo de String|Entero y un String, devuelve el valor de la entrada
 * @param m El mapeo
 * @param palabra El string palabra
 * @return Un entero que representa el valor de la entrada
 */
int cantApariciones(tMapeo m, char * palabra) {
    int toret = 0;
    tValor valor = m_recuperar(m, palabra);
    if (valor != NULL)
        toret = *((int*) valor);
    return toret;
}

/**
 * Dado el nombre de un archivo, inicializa un mapeo con entradas palabra|cantidad de apariciones. Luego puede recibir
 * un numero (1) o (2) por consola para realizar las siguientes acciones:
 *  (1) Solicita una palabra y devuelve la cantidad de apariciones de esa palabra en el archivo de texto
 *  (2) Salir del programa
 *
 *  Si la opción ingresada por el usuario es invalida, termina la ejecución con el código de error (-2)
 *
 * @param path La ruta de un archivo de texto
 */
void evaluador(char *path) {
    tMapeo m;
    crear_mapeo(&m,10, fHash, fComparador);
    cargarPalabrasEnMapeo(m, path);
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
    m_destruir(&m, &fEliminarC, &fEliminarV); //liberamos la memoria cuando salimos
}


/**
 * Ejecuta la función evaluador que inicializa el programa. Recibe por comando el nombre de un archivo de texto
 * @param args
 * @param path
 * @return
 */
int main(int args, char **path) {
    evaluador(*(path+1));
    return 0;
}