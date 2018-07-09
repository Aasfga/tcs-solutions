from socks import Socks, print_all


socks = Socks(('localhost', 4567), 'dominik')
connection = socks.create_connection(("probably_wrong_address.com", 80))
connection.send(b"wrong protocol")
print_all(connection)
