CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`

all: a.out

a.out: main.c full_search.o
	$(CC) $(GLIB) main.c full_search.o $(GTK) -o a.out

full_search.o: full_search.c
	$(CC) -c $(GLIB) full_search.c $(GTK) -o full_search.o

#glib_regex: glib_regex.c
#	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

clean:
	rm a.out *.o

