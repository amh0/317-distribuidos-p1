using System;
using System.Threading;
/* Libreria: Threading, Version dotnet: 7.0.401
 * Genera la serie: 2, 2, 5, 4, 10, 6, 17 ...
 *
 * El algoritmo divide los calculos equitativamente en intervalos y asigna cada intervalo
 * a un hilo (Thread), cada hilo se ejecuta y calcula los datos de la serie. El programa 
 * principal espera a que todos los hilos terminen thread.Join() para mostrar la serie 
 */
public class Serie
{
    class Program
    {
        static int N = 10000;                       // Cantidad de elementos de la serie
        public static int[] serie = new int[N];     // Array de la serie
        public static void Main()
        {
            int numHilos = 8;                       // Numero de hilos a usar
            int numElems = N / numHilos;            // Numero de elementos por intervalo

            if (numElems * numHilos < N)            // Si los hilos no son suficientes se añade uno
            {
                numHilos++;
            }

            for (int i = 0; i < numHilos; i++)
            {
                int[] intervalo = new int[2];
                intervalo[0] = numElems * i;        // Inicio de intervalo
                intervalo[1] = numElems * (i + 1);  // Fin de intervalo
                Thread thread = new Thread(CalculaSerie) { Name = i.ToString() };
                thread.Start(intervalo);            // Hilo calcula serie en intervalo [inicio, fin)
                thread.Join();                      // Espera a que los hilos finalicen
            }
            Console.WriteLine("{0} terminos de la serie:",N);
            for (int i = 0; i < N; i++)
            {
                Console.Write(serie[i] + " ");
            }
            Console.WriteLine("\n");
            Console.ReadKey();
        }
        static void CalculaSerie(Object parametro)
        {
            int[] intervalo = (int[])parametro;    // intervalo = {inicio, final}
            //Console.WriteLine($" Este es el hilo {Thread.CurrentThread.Name} : [{intervalo[0]}, {intervalo[1]})");
            
            // Calculo de la serie en intervalo
            for (int i = intervalo[0]; i < intervalo[1] && i < N; i++)
            {
                if (i % 2 == 0)
                {
                    int temp = (i / 2) + 1;
                    serie[i] = (temp * temp) + 1;
                }
                else
                {
                    serie[i] = i + 1;
                }
            }
        }
    }
}