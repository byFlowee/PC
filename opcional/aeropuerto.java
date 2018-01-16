import java.util.concurrent.Semaphore;

public class Avion extends Thread {

    private int id;

    private static final int K_HANGARES = 10;
    private static final int AVIONES = 30;
    private static Semaphore hangar;
    private static Semaphore pista;

    public Avion(int id) {
        this.id = id;
    }

    public void aterrizar() {
        hangar.aquire();
        pista.aquire();

        Thread.sleep(1000);
        System.out.println("El avion " + this.id + " aterriza");

        pista.release();
    }

    public void cargar() {
        Thread.sleep(500);
        System.out.println("El avion " + this.id + " carga");
    }

    public void descargar() {
        Thread.sleep(500);
        System.out.println("El avion " + this.id + " descarga");

        pista.aquire();
        hangar.release();
    }
    public void despegar() {
        Thread.sleep(1000);
        System.out.println("El avion " + this.id + " despega");

        pista.release();
    }


    public void run() {
        aterrizar();
        cargar();
        descargar();
        despegar();
    }

    public static void main(String args[]) {
        this.hangar = new Semaphore(K_HANGARES);
        this.pista = new Semaphore(1);
        ArrayList<Thread> threads = new ArrayList<Thread>();

        for (int i = 0; i < aviones; i++) {
            Thread thr = new Avion(i);
            thr.start();
            threads.add(thr);
        }

        try{
            for (int i = 0; i < aviones; i++) {
                Thread thr = threads.get(i);
                thr.join();
            }
        }catch(InterruptedException e) {
            return;
        }
    }
}
