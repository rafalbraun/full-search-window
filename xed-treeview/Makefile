CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --libs --cflags glib-2.0`
PCRE=`pkg-config --cflags --libs libpcre` 
GLIB_COMPILE_RESOURCES = /usr/local/bin/glib-compile-resources
COMPONENTS=xed-resources.c xed-tree-view.o 

all: a.out 

a.out : main.c $(COMPONENTS)
	$(CC) $(GLIB) main.c $(COMPONENTS) $(GTK) -o a.out 

xed-tree-view.o: xed-tree-view.c
	$(CC) -c $(GLIB) xed-tree-view.c $(GTK) -o xed-tree-view.o

xed-resources.c: ./resources/xed.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=./resources --generate-dependencies ./resources/xed.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) --target=xed-resources.c --sourcedir=./resources --generate-source ./resources/xed.gresource.xml

clean:
	rm -f xed-tree-view.o 
	rm -f xed-resources.c 
	rm -f a.out 

