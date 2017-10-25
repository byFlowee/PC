import java.lang.Math;

/*
* Hyman tiene el problema de no garantizar la exclusión mutua, ejecutando en modo multiprocesador
*/
public class Hyman extends Thread {
    static int count = 10000;
    static int n = 0;
    static volatile int C[] = {0,0};
    static int turn = 0;
    int id1;
    int id2;

    public void run() {
        try{
            for (int i = 0; i < count; i++) {
                sleep((long)(2*Math.random()));
                C[id1] = 1;                               // Thread 1 quiere entrar
                while(turn != id1){                       // Mientras no sea su turno
                  while(C[id2] == 1);                     // mientras el otro hilo quiera entrar espero
                  turn = id1;                             // cuando el otro hilo ya no quiera entrar, el turno es mio
                }

                n = n + 1;                                // accedo a la s.c

                C[id1] = 0;                               // Ya no quiero entrar
            }
        }catch( InterruptedException e ){return;}
    }

    Hyman(int id){
        this.id1 = id;
        this.id2 = (id == 1)? 0 : 1;
    }

    public static void main(String args[]) {
        Thread thr1 = new Hyman(0);
        Thread thr2 = new Hyman(1);
        thr1.start();
        thr2.start();

        try{
          thr1.join();                                    //esperamos a que los threads acaben para mostrar por pantalla al final el resultado obtenido
          thr2.join();
        }catch(InterruptedException e) {
          return;
        }

        System.out.println("Actual: " + n + " Expected: " + count*2);
    }
}
