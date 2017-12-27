public class Avion extends Thread {

    int hangar = 10;
    Semaphore p;

    public void run() {

    }

    public static void main(String args[]) {
        p = new Semaphore(1, true);
        Thread thr1 = new Avion(0);
        Thread thr2 = new Avion(1);
        thr1.start();
        thr2.start();

        try{
            thr1.join();                                    //esperamos a que los threads acaben para mostrar por pantalla al final el resultado obtenido
            thr2.join();
        }catch(InterruptedException e) {
            return;
        }
    }
}
