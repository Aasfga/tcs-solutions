#!/usr/bin/env python3
# vim:ts=4:sts=4:sw=4:expandtab

import socket


def print_all(connection):
    while True:
        msg = connection.recv(4096).decode("utf-8")
        if msg == "":
            break
        else:
            print(msg)

class Socks:
    def __init__(self, addr, user):
        self.addr = addr
        self.user = user

    def create_connection(self, addr):
        conn = socket.create_connection(self.addr)
        conn.send(bytes('CONNECT ' + str(addr[0]) + ' ' + str(addr[1]) + ' ' + self.user + "\r\n", 'utf8'))
        return conn
