CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith
LDFLAGS=-lpthread -lwiringPi


all: stlight

stlight: main.o tcpclient.o configs.o utils.o log.o tcpserver.o stlight.o movetime.o
	$(CC) main.o tcpclient.o configs.o utils.o log.o tcpserver.o stlight.o movetime.o -o stlight $(CFLAGS) $(LDFLAGS)

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

stlight.o: stlight.c
	$(CC) -c stlight.c $(CFLAGS)

movetime.o: movetime.c
	$(CC) -c movetime.c $(CFLAGS)


install:
	-mkdir /usr/share/streetlight/
	-cp stlight /usr/share/streetlight/
	-cp stlight.conf /etc/
	-cp mtime.conf /etc/
	-chmod +x /usr/share/streetlight/stlight

remove:
	-rm -rf /usr/share/streetlight
	-rm /etc/stlight.conf
	-rm /etc/mtime.conf

clean:
	-rm stlight
	-rm *.o
