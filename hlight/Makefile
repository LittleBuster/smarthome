CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith
LDFLAGS=-lpthread -lwiringPi


all: hlight

hlight: main.o tcpclient.o configs.o utils.o log.o tcpserver.o hlight.o
	$(CC) main.o tcpclient.o configs.o utils.o log.o tcpserver.o hlight.o -o hlight $(CFLAGS) $(LDFLAGS)

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

hlight.o: hlight.c
	$(CC) -c hlight.c $(CFLAGS)


install:
	-mkdir /usr/share/homelight/
	-cp hlight /usr/share/homelight/
	-cp hlight.conf /etc/
	-chmod +x /usr/share/homelight/hlight
	-cp hlight.service /etc/systemd/system/

remove:
	-rm -rf /usr/share/homelight
	-rm /etc/hlight.conf
	-rm /etc/systemd/system/hlight.service

clean:
	-rm hlight
	-rm *.o
