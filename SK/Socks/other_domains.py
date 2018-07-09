from socks import Socks, print_all


socks = Socks(('localhost', 4567), 'dominik')
address = ['www.w3schools.com', 'www.pizzeria-wieliczka.pl', "www.abcd.pl"]
i = 1
connection = socks.create_connection((address[i], 80))
request = "GET /index.html HTTP/1.1\r\nhost: " + address[i] + "\r\n\r\n"
connection.send(request.encode("utf-8"))
print_all(connection)
