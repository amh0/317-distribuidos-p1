using System;
using System.Threading;
/* Libreria: Threading, Version dotnet: 7.0.401
 * Calcula el numero pi mediante la serie de Nilakantha:
 *          (pi-3)/4 = 1/(2*3*4) - 1/(4*5*6) + 1/(6*7*8) - .... 
 * 
 * El algoritmo divide los calculos equitativamente en intervalos y asigna cada intervalo
 * a un hilo (Thread), cada hilo se ejecuta y calcula los terminos de la serie. El programa 
 * principal espera a que todos los hilos terminen thread.Join() para calcular el valor 
 * de Pi y mostrarlo. Al final se imprime el valor real de pi para comparacion 
 */
public class sharp3
{
    class Program
    {
        static int N = 1000000;                     // Cantidad de iteraciones
        public static Decimal sumaSerie;            // Variable de la serie
                                                    // Decimal almacena mas digitos que double
        public static int acc = 0;
        public static void Main()
        {
            int numHilos = 8;                       // Numero de hilos a usar
            int numElems = N / numHilos;            // Numero de elementos por intervalo

            if (numElems * numHilos < N)            // Si los hilos no son suficientes se añade uno adicional
            {
                numHilos++;
            }

            for (int i = 0; i < numHilos; i++)
            {
                int[] intervalo = new int[2];
                intervalo[0] = numElems * i;        // Inicio de intervalo
                intervalo[1] = numElems * (i + 1);  // Fin de intervalo
                Thread thread = new Thread(CalculoSeriePI) { Name = i.ToString() };
                thread.Start(intervalo);            // Hilo calcula terminos en intervalo [inicio, fin)
                thread.Join();                      // Espera a que los hilos finalicen
            }
            // Valor calculado mediante la serie
            Decimal pi = 3 + sumaSerie;
            Console.WriteLine("Pi calculo paralelo ({0} iteraciones):\t {1}", N, pi);
            // Valor de pi constante del lenguaje
            Decimal realPi = (Decimal)Math.PI;
            Console.WriteLine("Pi valor constante: \t\t\t\t {0}", realPi);
            Console.ReadKey();
        }
        public static void CalculoSeriePI(Object parametro)
        {
            int[] intervalo = (int[])parametro;     // intervalo = {inicio, final}
            //Console.WriteLine($" Este es el hilo {Thread.CurrentThread.Name} : [{intervalo[0]}, {intervalo[1]})");
            
            // Calculo de los terminos del intervalo
            for (int i = intervalo[0]; i < intervalo[1] && i < N; i++)
            {
                Decimal k = (Decimal)(i + 1.0) * 2;
                Decimal signo = 1;
                if (i % 2 != 0)
                    signo = -1;
                sumaSerie += signo * 4 / (Decimal)(k * (k + 1) * (k + 2));
            }
        }
    }
}