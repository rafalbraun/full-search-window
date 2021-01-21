CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --cflags --libs glib-2.0`
PROGRAMS=glib_regex full_search

all: $(PROGRAMS)

full_search: full_search.c glib_regex.c
	$(CC) $(GLIB) full_search.c $(GTK) -o full_search

glib_regex: glib_regex.c
	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

clean:
	rm $(PROGRAMS)

