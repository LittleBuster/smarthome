CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith
LDFLAGS=-lpthread -lopencv_core -lopencv_highgui -lopencv_imgproc -lftp -lwiringPi
OUTLIST=main.o tcpclient.o configs.o utils.o log.o tcpserver.o house.o meteo.o dht22.o gpio.o cam.o security.o termo.o termotemp.o


all: house

house: $(OUTLIST)
	$(CC) $(OUTLIST) -o house $(CFLAGS) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

configs.o: configs.c
	$(CC) -c configs.c $(CFLAGS)

tcpclient.o: tcpclient.c
	$(CC) -c tcpclient.c $(CFLAGS)

tcpserver.o: tcpserver.c
	$(CC) -c tcpserver.c $(CFLAGS)

house.o: house.c
	$(CC) -c house.c $(CFLAGS)

log.o: log.c
	$(CC) -c log.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c utils.c $(CFLAGS)

meteo.o: meteo.c
	$(CC) -c meteo.c $(CFLAGS)

dht22.o: dht22.c
	$(CC) -c dht22.c $(CFLAGS)

gpio.o: gpio.c
	$(CC) -c gpio.c $(CFLAGS)

cam.o: cam.c
	$(CC) -c cam.c $(CFLAGS)

security.o: security.c
	$(CC) -c security.c $(CFLAGS)

termo.o: termo.c
	$(CC) -c termo.c $(CFLAGS)

termotemp.o: termotemp.c
	$(CC) -c termotemp.c $(CFLAGS)

install:
	cp house.conf /etc/
	cp termo.conf /etc/

remove:
	/etc/house.conf
	/etc/termo.conf

clean:
	-rm house
	-rm *.o
	-rm *.jpg
