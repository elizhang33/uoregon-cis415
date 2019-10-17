all: main.o command.o shell.o
	gcc -g main.o command.o shell.o -o pseudo-shell

main.o: main.c shell.h
	gcc -g -c main.c -o main.o

command.o: command.c command.h
	gcc -g -c command.c -o command.o

shell.o: shell.c shell.h command.h
	gcc -g -c shell.c -o shell.o

clean:
	rm command.o shell.o main.o pseudo-shell