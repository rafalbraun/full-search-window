CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --libs --cflags glib-2.0`
PCRE=`pkg-config --cflags --libs libpcre` 
GLIB_COMPILE_RESOURCES = /usr/local/bin/glib-compile-resources
COMPONENTS=xed-resources.c xed-close-button.o 

all: a.out 

a.out : main.c $(COMPONENTS)
	$(CC) $(GLIB) main.c $(COMPONENTS) $(GTK) -o a.out 

xed-close-button.o: xed-close-button.c
	$(CC) -c $(GLIB) xed-close-button.c $(GTK) -o xed-close-button.o

xed-resources.c: ./resources/xed.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=./resources --generate-dependencies ./resources/xed.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) --target=xed-resources.c --sourcedir=./resources --generate-source ./resources/xed.gresource.xml

clean:
	rm -f xed-close-button.o 
	rm -f xed-resources.c 
	rm -f a.out 

