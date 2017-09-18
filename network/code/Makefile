client := peanut_cli.c
server := peanut_serv.c

all : peanut clean

peanut :
	gcc -o ../bin/peanut_cli $(client) -lpthread
	gcc -o ../bin/peanut_serv $(server) -lpthread

cli.o :
	gcc -c $(client)

serv.o :
	gcc -c $(server)


clean :
	rm -f *.o peanut_cli
	rm -f *.o peanut_serv
