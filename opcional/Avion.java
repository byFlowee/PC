import java.util.concurrent.Semaphore;

public class Avion extends Thread {

    private int id;

    private static final int K_HANGARES = 10;
    private static final int AVIONES = 20;
    private static Semaphore hangar;
    private static Semaphore pista;

    public Avion(int id) {
        this.id = id;
    }

    public void aterrizar() {
        try{
            hangar.acquire();
            System.out.println("El avion " + this.id + " reserva hangar. Hangares libres: " + hangar.availablePermits());
            pista.acquire();

            Thread.sleep(250);
            System.out.println("El avion " + this.id + " aterriza");

            pista.release();
        }catch(InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void cargar() {
        try{
            Thread.sleep(100);
            System.out.println("El avion " + this.id + " carga");
        }catch(InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void descargar() {
        try{
            Thread.sleep(100);
            System.out.println("El avion " + this.id + " descarga");

            pista.acquire();
            hangar.release();
            System.out.println("El avion " + this.id + " deja el hangar. Hangares libres: " + hangar.availablePermits());
        }catch(InterruptedException e) {
            e.printStackTrace();
        }
    }
    public void despegar() {
        try{
            Thread.sleep(250);
            System.out.println("El avion " + this.id + " despega");

            pista.release();
        }catch(InterruptedException e) {
            e.printStackTrace();
        }
    }


    public void run() {
        aterrizar();
        cargar();
        descargar();
        despegar();
    }

    public static void main(String args[]) {
        hangar = new Semaphore(K_HANGARES);
        pista = new Semaphore(1);
        Thread threads [] = new Thread[AVIONES];

        for (int i = 0; i < AVIONES; i++) {
            try{
                Thread.sleep((int)Math.random()*200 + 100);
                Thread thr = new Avion(i);
                thr.start();
                threads[i] = new Thread(thr);
            }catch(InterruptedException e) {
                e.printStackTrace();
            }
        }

        try{
            for (int i = 0; i < AVIONES; i++) {
                threads[i].join();
            }
        }catch(InterruptedException e) {
            return;
        }
    }
}
