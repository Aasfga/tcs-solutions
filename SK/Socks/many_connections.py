from socks import Socks, print_all


socks = Socks(('localhost', 4567), 'dominik')
c1 = socks.create_connection(('bot.whatismyipaddress.com', 80))
c2 = socks.create_connection(('bot.whatismyipaddress.com', 80))
c3 = socks.create_connection(('bot.whatismyipaddress.com', 80))
c1.send(b'GET / HTTP/1.1\r\nHost: bot.whatismyipaddress.com\r\n\r\n')
c2.send(b'GET / HTTP/1.1\r\nHost: bot.whatismyipaddress.com\r\n\r\n')
c3.send(b'GET / HTTP/1.1\r\nHost: bot.whatismyipaddress.com\r\n\r\n')
print_all(c1)
print_all(c2)
print_all(c3)

