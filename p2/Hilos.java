public class Hilos extends Thread {
	
	String hilo;
	
	public Hilos(String h) {
		this.hilo = h;
	}
	
	public void run() {
		System.out.println(hilo);
	}
	
	public static void main(String[] args) {
		Thread a = new Hilos("a");
		Thread b = new Hilos("b");
		
		a.start();
		b.start();
		System.out.println("Fin hilo principal");
		
		//Las salidas de los hilos se intercalan
	}
}
