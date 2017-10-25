import threading

states = [False, False]
turn = 0

THREADS = 2
MAX_COUNT = 10000

counter = 0

def entry_critical_section(i):
    global states
    global turn
    states[i] = True                          # Hilo 1 quiere entra
    while states[(i+1)%2]:                    # Mientras el hilo 2 quiera entrar
        if turn == ((i+1)%2):                 # Si el turno es del hilo 2
            states[i] = False                 # No quiero entrar
            while turn != i:                  # Mientras el turno no sea del Hilo 1, espero
                pass
            states[i] = True                  # Hilo 1 quiere entrar

def critical_section(i):
    global counter
    counter += 1
    print("Hilo {} : I = {}".format(i, counter))

def exit_critical_section(i):
    global states
    global turn
    turn = ((i+1)%2)
    states[i] = False

def thread(i):
    for j in range(MAX_COUNT//THREADS):
        entry_critical_section(i)
        critical_section(i)
        exit_critical_section(i)

def main():
    threads = []

    for i in range(THREADS):
        t = threading.Thread(target=thread, args=(i,))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    print("Counter value: {} Expected: {}\n".format(counter, MAX_COUNT))

if __name__ == "__main__":
    main()
