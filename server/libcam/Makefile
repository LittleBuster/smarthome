CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith
LDFLAGS=-lcrypto


all: libcam

libcam: tcpclient.o configs.o utils.o log.o cam.o main.o filetransfer.o
	$(CC) tcpclient.o configs.o utils.o log.o cam.o main.o filetransfer.o -o libcam $(CFLAGS) $(LDFLAGS)
#	-ln -s libstlight.so libstlight.so.1.0.0

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

filetransfer.o: filetransfer.c
	$(CC) -c filetransfer.c $(CFLAGS)

cam.o: cam.c
	$(CC) -c cam.c $(CFLAGS)


install:
	-cp libstlight.* /usr/lib/
	-mkdir /etc/smarthome/
	-mkdir /usr/include/smarthome/
	-cp stlight.conf /etc/smarthome/
	-cp stlight.h /usr/include/smarthome/

remove:
	-rm -rf /usr/lib/libstlight.so*
	-rm /etc/smarthome/stlight.conf
	-rm /usr/include/smarthome/stlight.h

clean:
	-rm libcam*
	-rm *.o
