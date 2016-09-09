CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith


all: libhouse

libhouse: tcpclient.o configs.o utils.o log.o house.o
	$(CC) tcpclient.o configs.o utils.o log.o house.o -shared -o libhouse.so $(CFLAGS) $(LDFLAGS)
	-ln -s libhouse.so libhouse.so.1.0.0

configs.o: configs.c
	$(CC) -c -fPIC configs.c $(CFLAGS)

tcpclient.o: tcpclient.c
	$(CC) -c -fPIC tcpclient.c $(CFLAGS)

log.o: log.c
	$(CC) -c -fPIC log.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c -fPIC utils.c $(CFLAGS)

house.o: house.c
	$(CC) -c -fPIC house.c $(CFLAGS)


install:
	-cp libhouse.* /usr/lib/
	-mkdir /etc/smarthome/
	-mkdir /var/log/smarthome/
	-mkdir /usr/include/smarthome/
	-cp house.conf /etc/smarthome/
	-cp house.h /usr/include/smarthome/

remove:
	-rm -rf /usr/lib/libhouse.so*
	-rm /etc/smarthome/house.conf
	-rm /usr/include/smarthome/house.h

clean:
	-rm libhouse*
	-rm *.o
