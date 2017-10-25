import threading

choosing = []
number = []

THREADS = 4
MAX_COUNT = 1000

counter = 0

def entry_critical_section(i):
    global chhoosing
    global number

    choosing[i] = True;
    number[i] = 1 + max(number)
    choosing[i] = False

    for j in range(0,THREADS):
        while choosing[j]:
            pass
        while number[j] > 0 and (number[j] < number[i] or (number[j] == number[i] and j < i)):
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
