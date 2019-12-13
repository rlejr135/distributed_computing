BINDIR =	/usr/local/sbin
MANDIR =	/usr/local/man/man8
CC =		cc
CFLAGS =	-O -ansi -pedantic -U__STRICT_ANSI__ -Wall -Wpointer-arith -Wshadow -Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wno-long-long
#SYSVLIBS =	-lnsl -lsocket
LDFLAGS =	-s $(SYSVLIBS)

all:		server	client

server:		server.o
	$(CC) server.o $(LDFLAGS) -o server -lpthread

server.o:	server.c	
	$(CC) $(CFLAGS) -c server.c -lpthread


client:		client.o
	$(CC) client.o $(LDFLAGS) -o client -lpthread

client.o:	client.c	
	$(CC) $(CFLAGS) -c client.c -lpthread




#all:		micro_httpd
#micro_httpd:	micro_httpd.o
#	$(CC) micro_httpd.o $(LDFLAGS) -o micro_httpd

#micro_httpd.o:	micro_httpd.c
#	$(CC) $(CFLAGS) -c micro_httpd.c

install:	all
	rm -f $(BINDIR)/server
	cp server $(BINDIR)/server

clean:
	rm -f server client *.o core core.* *.core
