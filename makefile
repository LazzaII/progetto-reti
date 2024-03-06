all: server client other clean

server: server.o utils.o structure.o auth.o command.o prison_break.o
	gcc -Wall server.o utils.o structure.o auth.o command.o prison_break.o -o server 

client: client.o include/utils.o 
	gcc -Wall client.o -o client

other: client.o include/utils.o 
	gcc -Wall client.o -o other

client.o: client.c
	gcc -Wall -g -c -std=c89 client.c -o client.o

server.o: server.c
	gcc -Wall -g -c -std=c89 server.c -o server.o

utils.o: utils.c utils.h 
	gcc -Wall -g -c -std=c89 utils.c -o utils.o

structure.o: structure.c structure.h 
	gcc -Wall -g -c -std=c89 structure.c -o structure.o

auth.o: include/auth.c include/auth.h
	gcc -Wall -g -c -std=c89 include/auth.c -o auth.o

command.o: include/command.c include/command.h
	gcc -Wall -g -c -std=c89 include/command.c -o command.o

prison_break.o: scenari/prison_break.c scenari/prison_break.h
	gcc -Wall -g -c -std=c89 scenari/prison_breakc -o prison_break.o

.PHONY: clean

clean: 
	rm *.o