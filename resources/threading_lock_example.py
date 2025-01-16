#!/usr/bin/env python3

import random
import threading
import time


class SharedObject:
    def __init__(self):
        self.value = 0
        self.lock = threading.Lock()

    def update(self):
        thread_name = threading.current_thread().name
        print(f"{thread_name} updating...")

        # copy shared, heap-based value into the thread's local variable stack
        self.lock.acquire()
        print(f"{thread_name} has lock...")
        local_value = self.value
        local_value += 1

        # simulate the thread being swapped out by the os or blocking
        # (e.g. on a socket call or database call)
        wait_time = random.randint(0, 3)
        print(f"{thread_name} waiting for {wait_time} seconds")
        time.sleep(wait_time)
        self.value = local_value
        print(f"{thread_name} releasing lock...")
        self.lock.release()

        print(f"{thread_name} finished updating...")


def main():
    obj = SharedObject()
    print(f"Start value:\t{obj.value}")
    t1 = threading.Thread(target=obj.update)
    t2 = threading.Thread(target=obj.update)
    t1.start()
    t2.start()

    t1.join()
    t2.join()

    print(f"End value:\t{obj.value}, should be 2")


if __name__ == '__main__':
    main()
