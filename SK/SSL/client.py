#!/usr/bin/env python3
# vim:ts=4:sts=4:sw=4:expandtab

import os
import re
import socket
import ssl
import sys

STUDENT = sys.argv[1]
STUDENT = STUDENT.translate(str.maketrans('ąćęłńóśźżĄĆĘŁŃÓŚŹŻ', 'acelnoszzACELNOSZZ'))
assert re.match(r'[A-Z][a-z]+ [A-Z][a-z]+', STUDENT)
PORT = int(sys.argv[2])
assert (2 ** 10 < PORT < 2 ** 16)
HOST = STUDENT.lower().replace(' ', '') + '.student.tcs.uj.edu.pl'

sock = socket.socket()
context = ssl.create_default_context()
# context.load_verify_locations(os.path.join(os.path.dirname(__file__), 'ca.crt'))
sock = context.wrap_socket(sock, server_hostname=HOST)

sock.connect((HOST, PORT))
sock.close()
