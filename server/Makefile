CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -Wno-pointer-arith -Wno-discarded-qualifiers
LDFLAGS=-lpthread -lstlight -lhouse -lhlight


all: wserver

wserver: main.o tcpclient.o configs.o utils.o log.o tcpserver.o webserver.o hstlight.o cstlight.o hhlight.o chlight.o htermo.o hmeteo.o hsecurity.o hcam.o chouse.o
	$(CC) main.o tcpclient.o configs.o utils.o log.o tcpserver.o webserver.o hstlight.o cstlight.o hhlight.o chlight.o htermo.o hmeteo.o hsecurity.o hcam.o chouse.o -o wserver $(CFLAGS) $(LDFLAGS)

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

hstlight.o: handlers/hstlight.c
	$(CC) -c handlers/hstlight.c $(CFLAGS)

cstlight.o: clients/cstlight.c
	$(CC) -c clients/cstlight.c $(CFLAGS)

hhlight.o: handlers/hhlight.c
	$(CC) -c handlers/hhlight.c $(CFLAGS)

chlight.o: clients/chlight.c
	$(CC) -c clients/chlight.c $(CFLAGS)

htermo.o: handlers/htermo.c
	$(CC) -c handlers/htermo.c $(CFLAGS)

hmeteo.o: handlers/hmeteo.c
	$(CC) -c handlers/hmeteo.c $(CFLAGS)

hsecurity.o: handlers/hsecurity.c
	$(CC) -c handlers/hsecurity.c $(CFLAGS)

hcam.o: handlers/hcam.c
	$(CC) -c handlers/hcam.c $(CFLAGS)

chouse.o: clients/chouse.c
	$(CC) -c clients/chouse.c $(CFLAGS)


install:
	-mkdir /usr/share/smarthome/
	-mkdir /etc/smarthome/
	-cp wserver /usr/share/smarthome/
	-cp wserver.conf /etc/
	-chmod +x /usr/share/smarthome/wserver

remove:
	rm -rf /usr/share/smarthome
	rm /etc/wserver.conf

clean:
	-rm wserver
	-rm *.o
