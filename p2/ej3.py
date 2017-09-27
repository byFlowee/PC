# -*- coding: utf-8 -*-
"""
Created on Thu Jul  7 12:15:14 2016

@author: Jorge
"""

#! /usr/bin/env python

import threading

THREADS = 5
MAX_COUNT = 250000

counter = 0

def thread():
    global counter
    for i in range(MAX_COUNT//THREADS):
        counter += 1

def main():
    threads = []

    for i in range(THREADS):
        t = threading.Thread(target=thread)
        threads.append(t)
        t.start() # start the thread

    # Wait for all threads to complete
    for t in threads:
        t.join()

    print("Counter value: {} Expected: {}".format(counter, MAX_COUNT))

if __name__ == "__main__":
    main()
