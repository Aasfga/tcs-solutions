import socket
import sys
import ssl
from pprint import pprint

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

HOST = 'www.dominikgrybos.student.tcs.uj.edu.pl'
PORT = 4756
sock.bind((HOST, PORT))
sock.listen(2)
sock.settimeout(None)

context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
context.load_cert_chain(certfile="domain.crt", keyfile="domain.key")
sock = context.wrap_socket(sock, server_side=True)

while True:
    try:
        print("Waiting for connection...")
        connection, client_address = sock.accept()
        connection.settimeout(10.0)
        print("Client connected")
        try:
            while True:
                try:
                    data = connection.recv(4096)
                    if data:
                        pprint(data)
                        connection.sendall(b'EnCrYpTiOn RoCkS\n')
                    else:
                        break
                except Exception as e:
                    print(e)
                    break
        finally:
            print('Disconnected')
            connection.close()
    except KeyboardInterrupt:
        sock.close()
        break
