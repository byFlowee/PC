public class HilosRunnablePriority implements Runnable {
	
	String hilo;
	
	public HilosRunnablePriority(String h) {
		this.hilo = h;
	}
	
	public void run() {
		for(int i = 0; i < 5; i++)
			System.out.println(hilo);
	}
	
	public static void main(String[] args) {
		HilosRunnablePriority a = new HilosRunnablePriority("a");
		HilosRunnablePriority b = new HilosRunnablePriority("b");
		
		Thread t1 = new Thread(a);
		Thread t2 = new Thread(b);
		
		t1.setPriority(Thread.MAX_PRIORITY);
		
		t1.start();
		t2.start();
		System.out.println("Fin hilo principal");
	}
}

