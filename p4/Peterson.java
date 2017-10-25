import java.lang.Math;

public class Peterson extends Thread {
    static int count = 10000;
    static int n = 0;
    static volatile int C[] = {0,0};
    static int turn = 0;
    int id1;
    int id2;

    public void run() {
        try{
            for (int i = 0; i < count; i++) {
                C[id1] = 1;                                     // Thread 1 quiere entrar
                turn = id2;                                     // Cede el turno al otro hilo
                while(C[id2] == 1 && turn == id2);              // Mientras que el otro hilo quiera entrar Y sea su turno, espero

                sleep((long)(1*Math.random()));
                n = n + 1;                                      // accedo a la s.c
                System.out.println("En hilo "+id1+", n = "+n);

                C[id1] = 0;                                     // Ya no quiero entrar
            }
        }catch( InterruptedException e ){return;}
    }

    Peterson(int id){
        this.id1 = id;
        this.id2 = (id == 1)? 0 : 1;
    }

    public static void main(String args[]) {
        Thread thr1 = new Peterson(0);
        Thread thr2 = new Peterson(1);
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
