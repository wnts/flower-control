import os
import threading, select

class GPIO:

    RISING = "rising"
    FALLING = "falling"
    NONE = "none"
    BOTH = "both"

    INPUT = "in"
    OUTPUT = "out"

    def __init__(self, pin, dir):
        self.pin = pin
        self.edge = self.NONE
        if not os.path.isfile("/sys/class/gpio/gpio%d/value" % self.pin):
            self.export()
        self.fd_edge = open("/sys/class/gpio/gpio%d/edge" % self.pin, "r+", 0)
        self.fd_value = open("/sys/class/gpio/gpio%d/value" % self.pin, "r+", 0)
        self.fd_direction = open("/sys/class/gpio/gpio%d/direction" % self.pin, "r+", 0)
        self.lock = threading.RLock()
        self.thread = None
        self.set_direction(dir)


    def export(self):
        f = open("/sys/class/gpio/export", "w", 0)
        f.write(str(self.pin))

    def set_direction(self, dir):
        self.fd_direction.write(dir)


    def get_value(self):
        return self.fd_value.read(-1)

    def set_high(self):
        self.fd_value.seek(0)
        self.fd_value.write("1")

    def set_low(self):
        self.fd_value.seek(0)
        self.fd_value.write("0")

    def set_callback(self, callback):
        self.lock.acquire()
        self.callback = callback
        self.lock.release()

    def enable_edge_detect(self):
        if self.thread is None:
            self.thread = threading.Thread(target=self.edge_detect)
        self.thread.start()

    def disable_edge_detect(self):
        self.thread.stop()

    def edge_detect(self):
        epoll = select.epoll()
        epoll.register(self.fd_value, select.EPOLLIN | select.EPOLLET)
        epoll.poll()
        while True:
            epoll.poll()
            self.callback()

    def set_edge(self, edge):
        self.edge = edge
        self.fd_edge.write(self.edge)
















