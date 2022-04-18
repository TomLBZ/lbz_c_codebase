CC = gcc
FLAGS = -lm -g -Wall

all: api example

example: networking.o
	$(CC) -c _examples/app.c _examples/services.c -lncurses $(FLAGS)
	$(CC) app.o services.o networking.o $(FLAGS) -o bin/example

api: curses funclist networking strings

# independant modules
curses: curses.o strings.o
	$(CC) -c curses/test.c -lncurses $(FLAGS)
	$(CC) curses.o colors.o test.o strings.o -lncurses $(FLAGS) -o bin/curses

funclist: funclist.o
	$(CC) -c funclist/test.c $(FLAGS)
	$(CC) funclist.o test.o $(FLAGS) -o bin/funclist

networking: networking.o
	$(CC) -c networking/test.c $(FLAGS)
	$(CC) networking.o test.o $(FLAGS) -o bin/networking

strings: strings.o
	$(CC) -c strings/test.c $(FLAGS)
	$(CC) strings.o test.o $(FLAGS) -o bin/strings

#objects
curses.o:
	$(CC) -c curses/curses.c curses/colors.c -lncurses $(FLAGS)

funclist.o:
	$(CC) -c funclist/funclist.c $(FLAGS)

networking.o:
	$(CC) -c networking/networking.c $(FLAGS)

strings.o:
	$(CC) -c strings/strings.c $(FLAGS)

clean:
	rm -rf *.o
	echo Clean Done.