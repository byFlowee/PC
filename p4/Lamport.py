import threading

choosing = []                               # Vector que almacena el estado de el hilo, 1 si está eligiendo, 0 en otro caso
number = []                                 # Vector que contiene el numero que ha sacado cada hilo

THREADS = 4
MAX_COUNT = 1000

counter = 0

def entry_critical_section(i):
    global chhoosing
    global number

    choosing[i] = True;                     # Hilo i le toca elegir número
    number[i] = 1 + max(number)             # Elige el mayor numero del vector number + 1
    choosing[i] = False                     # Hilo i ya ha elegido numero

    for j in range(0,THREADS):              # Para cada hilo
        while choosing[j]:                  # Si el hilo está eligiendo numero, espero
            pass
        while number[j] > 0 and (number[j] < number[i] or (number[j] == number[i] and j < i)):      # Si el hilo j ha elegido numero (es mayor que 0) AND es menor que el de i
                                                                                                    # si el número es igual, entonces si j lo ha sacado antes (j < i), Espero
            pass


def critical_section(i):
    global counter
    counter += 1
    print("Hilo {} : I = {}".format(i, counter))

def exit_critical_section(i):
    global number
    number[i] = 0

def thread(i):
    for j in range(MAX_COUNT//THREADS):
        entry_critical_section(i)
        critical_section(i)
        exit_critical_section(i)

def main():
    threads = []

    # Inicializamos los vectores choosing y number
    for i in range(THREADS):
        choosing.append(False)
        number.append(0)

    for i in range(THREADS):
        t = threading.Thread(target=thread, args=(i,))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    print("Counter value: {} Expected: {}\n".format(counter, MAX_COUNT))

if __name__ == "__main__":
    main()
