import random
import threading
import time

AVIONES = 25
K_HANGARES = 10

class Avion(threading.Thread):
    hangar = threading.Semaphore(K_HANGARES) #Semaforo que controla los hangares disponibles (inicializado a K_HANGARES)
    pista = threading.Lock()                 #Mutex que controla el acceso a la pista

    def __init__(self, id):
        super(Avion, self).__init__()
        self.id = id

    def aterrizar(self):
        Avion.hangar.acquire();     #reservamos el hangar antes de intentar reservar la pista
        print("El avion {} reserva hangar".format(self.id))
        with Avion.pista:           #con el hangar reservado, aterrizamos una vez la pista este disponible
            time.sleep(0.25);
            print("El avion {} aterriza".format(self.id))

    def cargar(self):
        time.sleep(0.1);
        print("El avion {} carga".format(self.id))

    def descargar(self):
        time.sleep(0.1);
        print("El avion {} descarga".format(self.id))
        Avion.pista.acquire()
        Avion.hangar.release()
        print("El avion {} deja el hangar".format(self.id))

    def despegar(self):
        time.sleep(0.25);
        print("El avion {} despega".format(self.id))
        Avion.pista.release()

    def run(self):
        self.aterrizar()
        self.cargar()
        self.descargar()
        self.despegar()

def main():

    aviones = []

    for i in range(AVIONES):
        aviones.append(Avion(i))

    for a in aviones:
        time.sleep(random.uniform(0.1,0.2)); #esperamos un tiempo aleatorio para la llegada de los aviones
        a.start()

    for a in aviones:
        a.join()


if __name__ == "__main__":
    main()
