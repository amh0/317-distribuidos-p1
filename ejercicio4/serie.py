import multiprocessing as mp
from multiprocessing.pool import Pool
# Libreria: mpi
# Genera la serie: 2, 2, 5, 4, 10, 6, 17
# usando Pool() se dividen los calculos de la serie entre los procesadores
# el resultado se almacena en la lista serie

def calcSerieI(i):
    if (i % 2 == 0):
        var = (i//2) +1
        return (var ** 2) + 1
    else:
        return i+1

if __name__ == '__main__':
    procesadores = 8    # Numero de procesadores 
    pool = Pool(processes = procesadores)
    N = 10000           # Numero de elementos de la serie

    # Se envia un array de numeros del 0 a N a la funcion calcSerieI
    # usando pool.map que divide los calculos en los diferentes procesadores
    serie = pool.map(calcSerieI, [i for i in range(N)])

    # Se muestra el resultado de la serie almacenado en la lista
    print(serie)