# Unix-Socket-Example
Unix Socket Programming

* files:

1.server.c: TCP server (contain port number and hostname)

2.client.c: Client that sends request to server

3.query.txt: data on the server

* compilation:

`gcc server.c -o server`

`gcc client.c -o client`

* execution:

`./server` on a window

`./client` on the other window

* example:

`./client`
`What’ s your requirement? 1.DNS 2.QUERY 3.QUIT : 1`
`Input URL address : www.google.com`
`address get from domain name : 173.194.72.106`
`What’ s your requirement? 1.DNS 2.QUERY 3.QUIT : 2`
`Input student ID : 99123`
`Email get from server : …`

* reference:

http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
http://www.tutorialspoint.com/unix_sockets/socket_client_example.htm
