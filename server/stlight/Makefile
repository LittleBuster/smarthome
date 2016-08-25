CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith


all: libstlight

libstlight: tcpclient.o configs.o utils.o log.o stlight.o
	$(CC) tcpclient.o configs.o utils.o log.o stlight.o -shared -o libstlight.so $(CFLAGS)
	-ln -s libstlight.so libstlight.so.1.0.0

main.o: main.c
	$(CC) -c -fPIC main.c $(CFLAGS)

configs.o: configs.c
	$(CC) -c -fPIC configs.c $(CFLAGS)

tcpclient.o: ../tcpclient.c
	$(CC) -c -fPIC ../tcpclient.c $(CFLAGS)

log.o: ../log.c
	$(CC) -c -fPIC ../log.c $(CFLAGS)

utils.o: ../utils.c
	$(CC) -c -fPIC ../utils.c $(CFLAGS)

stlight.o: stlight.c
	$(CC) -c -fPIC stlight.c $(CFLAGS)


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
	-rm libstlight.so*
	-rm *.o