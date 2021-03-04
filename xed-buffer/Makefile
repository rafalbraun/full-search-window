CC=gcc
LIBS=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-3.0`
FLAGS=-Wall

all: srcview

srcview: srcview.c
	$(CC) $(FLAGS) -o srcview srcview.c $(LIBS)

clean:
	rm srcview

