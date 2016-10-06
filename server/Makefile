CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -Wno-pointer-arith -Wno-discarded-qualifiers
LDFLAGS=-lpthread -lstlight -lhouse -lhlight


all: wserver

wserver: main.o tcpclient.o configs.o utils.o log.o tcpserver.o webserver.o
	$(CC) main.o tcpclient.o configs.o utils.o log.o tcpserver.o webserver.o -o wserver $(CFLAGS) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

configs.o: configs.c
	$(CC) -c configs.c $(CFLAGS)

tcpclient.o: tcpclient.c
	$(CC) -c tcpclient.c $(CFLAGS)

log.o: log.c
	$(CC) -c log.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c utils.c $(CFLAGS)

tcpserver.o: tcpserver.c
	$(CC) -c tcpserver.c $(CFLAGS)

webserver.o: webserver.c
	$(CC) -c webserver.c $(CFLAGS)


install:
	-mkdir /usr/share/smarthome/
	-mkdir /etc/smarthome/
	-cp wserver /usr/share/smarthome/
	-cp wserver.conf /etc/smarthome/
	-chmod +x /usr/share/smarthome/wserver

remove:
	rm -rf /usr/share/smarthome
	rm /etc/smarthome/wserver.conf

clean:
	-rm wserver
	-rm *.o
