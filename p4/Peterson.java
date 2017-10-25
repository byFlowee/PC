import java.lang.Math;

public class Peterson extends Thread {
    static int n = 0;
    static volatile int C[] = {0,0};
    static int turn = 0;
    int id1;
    int id2;

    public void run() {
        try{
            for (int i = 0; i < 10000; i++) {

                C[id1] = 0;
                turn = id2;
                while(C[id2] == 1 && turn == id2);

                sleep((long)(1*Math.random()));
                n = n + 1;
                System.out.println("En hilo "+id1+", n = "+n);

                C[id1] = 0;;
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
    }
}
