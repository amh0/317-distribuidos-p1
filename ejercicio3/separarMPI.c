#include "stdio.h"
#include "mpi.h"
#include "stdlib.h"
#include "string.h"
/* Libreria: MPI, Version gcc:  11.2.0
 * Programa que separa las palabras de una frase en dos usando la libreria mpi
 *
 * Utiliza multiples procesadores para encontrar las palabras de la frase. Se designa como raiz al procesador
 * 0, el resto de los procesadores encontraran las palabras de la frase y almacenaran en una array aquellas 
 * que les corresponda procesar, luego enviaran todas las palabras que  encontraron al procesador raiz, la 
 * raiz luego se encarga de ordenar las palabras para almacenarlos en un array de palabras pares o impares. 
 * 
 * Se determina que palabras encuentra un procesador, dependiendo del numero de procesadores.
 * Ejemplo: Para nProcesador = 5:
 *      El procesador 1, encontrara las palabras: 0, 4, 8 ... 
 *      El procesador 2, encontrará las palabras: 1, 5, 9 ...
 * 
 * Usa el mismo algoritmo para encontrar palabras que el programa separar.c
 * 
 * COMPILACION: Dependiendo del SO  gcc (Windows) o mpicc (Linux)
 * EJECUCION: 
 *      Windows: mpiexec -n [nProcesadores] separarMPI
 *      Linux: mpirun -np [nProcesadores] ./separarMPI
 * nProcesadores es impar y mayor a 1
 */
void printStringArray(char **a, int n){
    for (int i = 0; i < n; ++i){
        printf("%s ",a[i]);
    }
    printf("\n");
}
int main(int argc, char *argv[])
{
    char frase[] = {"tres tristes tigres trigaban trigo por culpa del bolivar"};
    int lenFrase = (int)sizeof(frase) / sizeof(frase[0]);
    int procesador, nProcesador;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &procesador);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesador);

    int procRaiz = 0;                   // Procesador raiz
    int MAX_PALABRAS = 100;

    char *par[MAX_PALABRAS];
    char *impar[MAX_PALABRAS];

    if (procesador != procRaiz){        // Si no es el procesador raiz se procesan las cadenas
        int lenPalabra = 0;             // Longitud de la palabra actual
        int indPalabra = 0;             // Indice palabras
        int cantPalabras = 0;
        char *arrPalabras[MAX_PALABRAS];
        for (int i = 0; i < lenFrase + 1; ++i){
            // Asignamos frase[i] si i esta dentro de los limites de la frase, ' ' en otro caso
            char c = (i < lenFrase) ? frase[i] : ' ';
            if (c != ' '){
                lenPalabra++;
            }
            else{
                // Dependiendo del procesador y nProcesador se procesan diferentes cadenas
                if (cantPalabras % (nProcesador - 1) == (procesador - 1)){
                    int j = i - lenPalabra;     // Indice inicial de la palabra
                    char palabra[lenPalabra + 1];
                    for (int k = 0; k < lenPalabra; ++k){
                        palabra[k] = frase[j];
                        j++;
                    }
                    palabra[lenPalabra] = '\0'; // Adicionamos la terminacion de string
                    if (lenPalabra == 0)        // Si la palabra es vacia no la agregamos
                        continue;
                    arrPalabras[indPalabra] = strdup(palabra); // Reservamos espacio de memoria para palabra
                    indPalabra++;               
                }
                lenPalabra = 0;
                cantPalabras++;
            }
        }
        // Se envia la cantidad total de palabras al procesador raiz
        MPI_Send(&indPalabra, 1, MPI_INT, procRaiz, 0, MPI_COMM_WORLD);
        // Se envian las cadenas del array de palabras una a la vez
        for (int i = 0; i < indPalabra; i++){
            int longPalabra = strlen(arrPalabras[i]);
            MPI_Send(&longPalabra, 1, MPI_INT, procRaiz, 0, MPI_COMM_WORLD);
            MPI_Send(arrPalabras[i], longPalabra, MPI_CHAR, procRaiz, 0, MPI_COMM_WORLD);
        }
    }
    else{
        int procOrigen;                     // Procesador origen del cual se recibe el mensaje
        int inc = (nProcesador - 1) / 2;    // Incremento de indice
        int cantPar = 0, cantImpar = 0;     // Cantidad palabras pares, impares
        for (int i = 1; i < nProcesador; i++){
            procOrigen = i;
            int contPalabras;
            char *pal[MAX_PALABRAS];
            // Se recibe el numero de palabras enviado por el procesador origen
            MPI_Recv(&contPalabras, 1, MPI_INT, procOrigen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int indPalabra = ((procOrigen + 1) / 2) - 1; // indPalabra oficial
            // Se reciben las palabras una a la vez
            for (int i = 0; i < contPalabras; i++){
                int longPalabra;
                // Se recibe la longitud de la cadena actual
                MPI_Recv(&longPalabra, 1, MPI_INT, procOrigen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                char *palabra = (char *)malloc((longPalabra + 1) * sizeof(char));   // Reserva de memoria
                // Se recibe y asigna la cadena
                MPI_Recv(palabra, longPalabra, MPI_CHAR, procOrigen, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                palabra[longPalabra] = '\0';    // Terminacion de string
                // Se agregan las palabras al array de pares o impares dependiendo del procesador
                if (procOrigen % 2 != 0){
                    par[indPalabra] = strdup(palabra);
                    cantPar++;
                }
                else{
                    impar[indPalabra] = strdup(palabra);
                    cantImpar++;
                }
                indPalabra += inc;
            }
        }
        printf("Frase original: %s\n",frase);
        printf("Division en dos...\n\n");
        // Se muestra el array de todas las palabras separadas
        printf("Frase 1:\n");
        printStringArray(par, cantPar);
        printf("\nFrase 2:\n");
        printStringArray(impar, cantImpar);
    }
    MPI_Finalize();
}
