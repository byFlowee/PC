# Dekker algorithm
from threading import Thread

THREADS_AMOUNT = 2
MAX_COUNT_AMOUNT = 10000000
COUNT_PER_THREAD = MAX_COUNT_AMOUNT/THREADS_AMOUNT

count = 0


class Worker(Thread):

    turn = 0
    want_status = [False, False]

    def __init__(self, id):
        self.me = id
        self.him = (id + 1) % 2
        Thread.__init__(self)

    def run(self):
        for count in range(COUNT_PER_THREAD):
            self.pre_protocol()
            self.critical_section()
            self.post_protocol()

    def pre_protocol(self):
        Worker.want_status[self.me] = True
        while Worker.want_status[self.him]:
            if Worker.turn == self.him:
                Worker.want_status[self.me] = False
                while Worker.turn != self.me:
                    pass
                Worker.want_status[self.me] = True

    def critical_section(self):
        global count
        count += 1

    def post_protocol(self):
        Worker.turn = self.him
        Worker.want_status[self.me] = False


threads = []


def main():
    create_threads()
    start_threads()
    join_threads()
    output_result()


def create_threads():
    for id in range(THREADS_AMOUNT):
        new_thread = Worker(id)
        threads.append(new_thread)


def start_threads():
    for thread in threads:
        thread.start()


def join_threads():
    for thread in threads:
        thread.join()


def output_result():
    print("Counter value: {} -- Expected: {}".format(count, MAX_COUNT_AMOUNT))

if __name__ == "__main__":
    main()
