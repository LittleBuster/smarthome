CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith


all: libcam

libcam: tcpclient.o configs.o utils.o log.o cam.o
	$(CC) tcpclient.o configs.o utils.o log.o cam.o -shared -o libcam.so $(CFLAGS) $(LDFLAGS)
	-ln -s libcam.so libcam.so.1.0.0

configs.o: configs.c
	$(CC) -c -fPIC configs.c $(CFLAGS)

tcpclient.o: tcpclient.c
	$(CC) -c -fPIC tcpclient.c $(CFLAGS)

log.o: log.c
	$(CC) -c -fPIC log.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c -fPIC utils.c $(CFLAGS)

cam.o: cam.c
	$(CC) -c -fPIC cam.c $(CFLAGS)


install:
	-cp libcam.* /usr/lib/
	-mkdir /etc/smarthome/
	-mkdir /usr/include/smarthome/
	-cp cam.conf /etc/smarthome/
	-cp cam.h /usr/include/smarthome/

remove:
	-rm -rf /usr/lib/libcam.so*
	-rm /etc/smarthome/cam.conf
	-rm /usr/include/smarthome/cam.h

clean:
	-rm libcam*
	-rm *.o
