#include "stdio.h"
#include "omp.h"
#include "string.h"
/* Libreria: omp, Version gcc:  11.2.0
 * Programa que separa las palabras de una frase en dos con la libreria omp
 * Utiliza procesamiento paralelo (2 procesadores) para dividir las palabras, un procesador se encarga de 
 * procesar y añadir las palabras pares a su respectivo array, y el otro procesador las impares
 *
 * Se procesan los chars de la frase en busca de palabras, mientras no se encuentre un espacio ' '
 * se incrementa lenPalabra que determina la cantidad de chars que tiene la palabra actual.
 *
 * Una vez que se encuentra un separador, se pregunta si el procesador esta encargado de la palabra, de ser
 * asi, se recorre desde el inicio de la palabra (indiceActual - lenPalabra) hasta el final de la palabra agregando
 * los chars a un 'string' para luego añadirlo al array de palabras pares o impares segun corresponda
 *
 * COMPILACION: En Windows y Linux
 * gcc separar.c -o separar -fopenmp 
 */
void printStringArray(char **a, int n){
    for (int i = 0; i < n; ++i){
        printf("%s ",a[i]);
    }
    printf("\n");
}
int main(){
    int MAX_PALABRAS = 100;
    int procesador, cantProc = 2;
    // tres tristes tigres trigaban trigo por culpa del bolivar
    char frase[] = {"tres tristes tigres trigaban trigo por culpa del bolivar"};
    int lenFrase = (int) sizeof(frase)/sizeof(frase[0]);

    char *impar[MAX_PALABRAS];  // Array de punteros a arrays de chars
    char *par[MAX_PALABRAS];

    int indParPalabra;          // Indice de palabras pares
    int indImparPalabra;        // Indice de palabras impares
    // Habilita el procesamiento paralelo
    #pragma omp parallel num_threads(cantProc) private(procesador)
    {
        procesador = omp_get_thread_num();
        indParPalabra = 0;
        indImparPalabra = 0;
        int lenPalabra = 0;     // Longitud de la palabra actual
        int contPalabras = 0;
        #pragma omp parallel for
        for (int i = 0; i < lenFrase +1; ++i){
            // Asignamos frase[i] si i esta dentro de los limites de la frase, ' ' en otro caso
            char c = (i < lenFrase) ? frase[i] : ' '; 
            if (c!=' '){
                lenPalabra++;
            } 
            else {
                if (contPalabras % 2 == procesador){    // Dependiendo del procesador se seleccionan palabras impares o pares
                    int j = i-lenPalabra;               // Indice inicial de la palabra
                    char palabra[lenPalabra+1];     
                    for (int k = 0; k < lenPalabra; ++k){
                        palabra[k] = frase[j];
                        j++;
                    }
                    palabra[lenPalabra] = '\0';         // Adicionamos la terminacion de string
                    if (lenPalabra==0)                  // Si la palabra es vacia no la agregamos
                        continue;
                    if (contPalabras%2 == 0){
                        par[indParPalabra] = strdup(palabra); // Reservamos espacio de memoria para palabra
                        indParPalabra++; 
                    }
                    else{
                        impar[indImparPalabra] = strdup(palabra); 
                        indImparPalabra++; 
                    }
                }
                lenPalabra = 0;  
                contPalabras++;
            }
        }
    }
    printf("Frase original: %s\n",frase);
    printf("Division en dos...\n\n");
    printf("Frase 1: \n");
    printStringArray(par, indParPalabra);
    printf("\nFrase 2: \n");
    printStringArray(impar, indImparPalabra);
    return 0;
}
