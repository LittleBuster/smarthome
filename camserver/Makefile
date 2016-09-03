CC=gcc
CFLAGS=-Wall -I. -std=gnu99 -pedantic -march=native -O2 -Wno-pointer-arith
LDFLAGS=-lpthread -lcrypto -lopencv_core -lopencv_highgui -l opencv_imgproc
OUTLIST=main.o tcpclient.o configs.o utils.o log.o tcpserver.o camsrv.o filetransfer.o


all: camsrv

camsrv: $(OUTLIST)
	$(CC) $(OUTLIST) -o camsrv $(CFLAGS) $(LDFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

configs.o: configs.c
	$(CC) -c configs.c $(CFLAGS)

tcpclient.o: tcpclient.c
	$(CC) -c tcpclient.c $(CFLAGS)

tcpserver.o: tcpserver.c
	$(CC) -c tcpserver.c $(CFLAGS)

camsrv.o: camsrv.c
	$(CC) -c camsrv.c $(CFLAGS)

log.o: log.c
	$(CC) -c log.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c utils.c $(CFLAGS)

filetransfer.o: filetransfer.c
	$(CC) -c filetransfer.c $(CFLAGS)


install:
	cp camsrv.conf /etc/

remove:
	/etc/camsrv.conf

clean:
	-rm camsrv
	-rm *.o
