all: server client other clean

server: server.o utils.o structure.o auth.o command.o prison_break.o
	gcc -Wall server.o utils.o structure.o auth.o command.o prison_break.o -o server 

client: client.o utils.o 
	gcc -Wall client.o -o client

other: client.o utils.o 
	gcc -Wall client.o -o other

client.o: client.c
	gcc -Wall -g -c -std=c89 client.c -o client.o

server.o: server.c
	gcc -Wall -g -c -std=c89 server.c -o server.o

auth.o: include/auth.c include/auth.h
	gcc -Wall -g -c -std=c89 include/auth.c -o auth.o

structure.o: include/structure.c include/structure.h 
	gcc -Wall -g -c -std=c89 include/structure.c -o structure.o

utils.o: include/utils.c include/utils.h
	gcc -Wall -g -c -std=c89 include/utils.c -o utils.o

command.o: include/command.c include/command.h
	gcc -Wall -g -c -std=c89 include/command.c -o command.o

prison_break.o: scenari/prison_break/prison_break.c scenari/prison_break/prison_break.h
	gcc -Wall -g -c -std=c89 scenari/prison_break/prison_break.c -o prison_break.o

.PHONY: clean

clean: 
	rm *.o