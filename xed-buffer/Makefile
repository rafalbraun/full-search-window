CC=gcc
LIBS=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-3.0`
FLAGS=-Wall
DEPS=`pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`

all: srcview multicursor

srcview: srcview.c
	$(CC) $(FLAGS) -o srcview srcview.c $(LIBS)

multicursor: multicursor.c
	$(CC) multicursor.c -o multicursor $(DEPS)

clean:
	rm srcview multicursor

