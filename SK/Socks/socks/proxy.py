#!/usr/bin/env python3
# vim:ts=4:sts=4:sw=4:expandtab

import socket
import threading
from select import select
import logging


class Worker(threading.Thread):
    def __init__(self, client, ):
        super().__init__()
        self.server = None
        self.message = ""
        self.client = client

    def set_server(self):
        first_msg = self.client.recv(4096).decode("utf-8")
        connect_msg, self.message = first_msg.split("\r\n", 1)
        connect_msg = connect_msg[8:]
        server_address, server_port, user = connect_msg.split()
        try:
            self.server = socket.create_connection((server_address, server_port), timeout=1)
        except Exception as ex:
            raise ValueError(server_address)

    def prepare_to_run(self):
        try:
            self.set_server()
        except ValueError as ex:
            logging.error("Wrong server address: {}".format(ex.args[0]))
            self.client.close()
            exit(1)
        if self.message is not "":
            self.server.send(bytes(self.message, "utf-8"))

    def run(self):
        self.prepare_to_run()
        logging.info("Starting connection between: {} and {}".format(self.client.getpeername(), self.server.getpeername()))

        while True:
            read, write, err = select([self.client, self.server], [], [])
            crr_reader = read[0]
            _msg = crr_reader.recv(4096)
            if _msg == b"":
                break
            if crr_reader == self.client:
                logging.info("Message from {}".format(self.client.getpeername()))
                self.server.send(_msg)
            else:
                logging.info("Message from {}".format(self.server.getpeername()))
                self.client.send(_msg)

        logging.info("Closing connection between: {} and {}".format(self.client.getpeername(), self.server.getpeername()))
        self.server.close()
        self.client.close()


def main_loop(socket_):
    while True:
        client, address = socket_.accept()
        worker = Worker(client)
        worker.start()


logging.basicConfig(level=logging.INFO)
HOST = ''
PORT = 4567
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind((HOST, PORT))
sock.listen(1)
main_loop(sock)


