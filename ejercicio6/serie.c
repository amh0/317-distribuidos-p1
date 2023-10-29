#include"stdio.h"
#include "mpi.h"
/* Libreria: mpi, Version gcc: 11.2.0
 * Genera la serie: 2, 2, 5, 4, 10, 6, 17 ...
 * 
 * El algoritmo utiliza MPI_Scatter para dividir los calculos equitativemente entre todos los 
 * procesadores, cada procesador realiza los calculos de su region individualmente
 * Posteriormente se unen todos los resultados con MPI_Gather en el array 'serie'
 * 
 * EJECUCION: 
 *     Windows: mpiexec -n [nProcesadores] serie
 *     Linux: mpirun -np [nProcesadores] ./serie
 * nProcesadores es un numero que divide al numero de elementos de la serie numElem
*/
int main(int argc, char *argv[])
{
    int proc, nProc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    int procRaiz = 0;                   // Procesador raiz
    int numElem = 10000;                // Numero de elementos de la serie
    int elemsProc = numElem / nProc;    // Numero de elementos que se asigna a cada procesador

    if (elemsProc * nProc != numElem){  // Se verifica si es posible asignar elemsProc elementos a cada procesador
        printf("No es posible dividir los elementos entre los procesadores, ingrese otra cantidad de procesadores\n");
        MPI_Finalize();
        return 0;
    }
    int nums[numElem];                  // Se inicializa cada elemento del array nums con su indice
    for (int i = 0; i < numElem; i++){
        nums[i] = i;
    }
    int subSerie[elemsProc];            // Array local de cada procesador
    
    // Dividir el array nums entre los procesadores, cada procesador recibe elemsProc elementos
    // en el array subSerie y realiza los calculos de la serie en su seccion
    MPI_Scatter(nums, elemsProc, MPI_INT, subSerie, elemsProc, MPI_INT, procRaiz,MPI_COMM_WORLD);
    for (int i = 0; i < elemsProc; i++){
        if (subSerie[i] % 2 == 0){
            int temp = (subSerie[i]/2) + 1;
            subSerie[i] = (temp * temp) + 1;
        }
        else{
            subSerie[i] = subSerie[i] + 1;
        }
    }
    int serie[numElem];                 // Se inicializa el array resultado
    
    // Se reune todos los array subSerie de los procesadores en el array serie
    MPI_Gather(subSerie, elemsProc, MPI_INT, serie, elemsProc, MPI_INT, procRaiz, MPI_COMM_WORLD);
    
    if (proc == procRaiz){
        printf("%d elementos de la serie:\n",numElem);      // Se muestra el array de la serie
        for (int i = 0; i < numElem; i++){
            printf("%d ",serie[i]);
        }
        printf("\n");
    }    
    MPI_Finalize();
}