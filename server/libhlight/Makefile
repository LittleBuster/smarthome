CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith


all: libhlight

libhlight: tcpclient.o configs.o utils.o log.o hlight.o
	$(CC) tcpclient.o configs.o utils.o log.o hlight.o -shared -o libhlight.so $(CFLAGS)
	-ln -s libhlight.so libhlight.so.1.0.0

main.o: main.c
	$(CC) -c -fPIC main.c $(CFLAGS)

configs.o: configs.c
	$(CC) -c -fPIC configs.c $(CFLAGS)

tcpclient.o: tcpclient.c
	$(CC) -c -fPIC tcpclient.c $(CFLAGS)

log.o: log.c
	$(CC) -c -fPIC log.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c -fPIC utils.c $(CFLAGS)

hlight.o: hlight.c
	$(CC) -c -fPIC hlight.c $(CFLAGS)


install:
	-cp libhlight.* /usr/lib/
	-mkdir /etc/smarthome/
	-mkdir /var/log/smarthome/
	-mkdir /usr/include/smarthome/
	-cp hlight.conf /etc/smarthome/
	-cp hlight.h /usr/include/smarthome/

remove:
	-rm -rf /usr/lib/libhlight.so*
	-rm /etc/smarthome/hlight.conf
	-rm /usr/include/smarthome/hlight.h

clean:
	-rm libhlight.so*
	-rm *.o
