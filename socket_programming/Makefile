CC=gcc
CFLAGS= -g

tcpserver: tcp_server.c
	$(CC) $(CFLAGS) -o tcpserver tcpserver.c
	
tcpclient: tcp_client.c
	$(CC) $(CFLAGS) -o tcpclient tcpclient.c


clean:
	rm -rf tcpserver tcpclient
