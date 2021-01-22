CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --libs --cflags glib-2.0`

all: a.out  #xed-highlight-mode-dialog.o xed-highlight-mode-selector.o

a.out: main.c xed-highlight-mode-dialog.o xed-highlight-mode-selector.o
	$(CC) $(GLIB) main.c xed-highlight-mode-dialog.o xed-highlight-mode-selector.o $(GTK) -o a.out

full_search.o: full_search.c
	$(CC) -c $(GLIB) full_search.c $(GTK) -o full_search.o

#glib_regex: glib_regex.c
#	$(CC) $(GLIB) glib_regex.c -o glib_regex -lpthread

xed-highlight-mode-dialog.o: xed-highlight-mode-dialog.c
	$(CC) -c $(GLIB) xed-highlight-mode-dialog.c $(GTK) -o xed-highlight-mode-dialog.o

xed-highlight-mode-selector.o: xed-highlight-mode-selector.c
	$(CC) -c $(GLIB) xed-highlight-mode-selector.c $(GTK) -o xed-highlight-mode-selector.o

clean:
	rm a.out *.o

