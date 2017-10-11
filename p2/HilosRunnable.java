public class HilosRunnable implements Runnable {
	
	String hilo;
	
	public HilosRunnable(String h) {
		this.hilo = h;
	}
	
	public void run() {
		for(int i = 0; i < 5; i++)
			System.out.println(hilo);
	}
	
	public static void main(String[] args) {
		HilosRunnable a = new HilosRunnable("a");
		HilosRunnable b = new HilosRunnable("b");
		
		Thread t1 = new Thread(a);
		Thread t2 = new Thread(b);
		
		t1.start();
		t2.start();
		System.out.println("Fin hilo principal");
		
		//una vez un hilo empieza tiene la ejecucion hasta que acaba
	}
}

