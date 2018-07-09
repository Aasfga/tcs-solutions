
from socks import Socks, print_all

socks = Socks(('localhost', 4567), 'dominik')
connection = socks.create_connection(('bot.whatismyipaddress.com', 80))
connection.send(b'GET / HTTP/1.1\r\nHost: bot.whatismyipaddress.com\r\n\r\n')
print_all(connection)

