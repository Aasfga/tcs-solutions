import socket
import time
import math
from tftp.packet import *


class Worker:
    UDP_SIZE = 65527
    START_TIME = 0.03
    MAX_TIME = 4
    CHANGE = 2
    ROUNDS = 1

    def prepare(self, filename):
        raise NotImplementedError

    def block_end(self):
        return self.block_add(self.window_size)

    def __init__(self, target, block_size, window_size):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.block = None
        self.block_size = block_size
        self.window_size = window_size
        if target[0] == "localhost":
            target = "127.0.0.1", target[1]
        self.target = target

    def block_add(self, x):
        return (self.block + x) % 2 ** 16

    def receive_packet(self, wait_time):
        self.sock.settimeout(wait_time)
        begin = time.time()
        packet = self.sock.recvfrom(Worker.UDP_SIZE)
        return packet, wait_time - (time.time() - begin)

    def send_error(self, code, msg):
        self.sock.sendto(error_packet(code, msg), self.target)


if Worker.CHANGE == 1:
    Worker.ROUNDS = int(Worker.MAX_TIME / Worker.START_TIME)
else:
    Worker.ROUNDS = int(round(math.log(Worker.MAX_TIME / Worker.START_TIME + 1, 2) - 1))
