CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --libs --cflags glib-2.0`
PCRE=`pkg-config --cflags --libs libpcre` 
GLIB_COMPILE_RESOURCES = /usr/local/bin/glib-compile-resources

OBJ=xed-tree-view.o 
SRC=xed-tree-view.c 

COMPONENTS=xed-resources.c $(OBJ)

all: a.out 

a.out : main.c $(COMPONENTS)
	$(CC) $(GLIB) main.c $(COMPONENTS) $(GTK) -o a.out 

$(OBJ): $(SRC)
	$(CC) -c $(GLIB) $(SRC) $(GTK) -o $(OBJ)

xed-resources.c: ./resources/xed.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=./resources --generate-dependencies ./resources/xed.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) --target=xed-resources.c --sourcedir=./resources --generate-source ./resources/xed.gresource.xml

clean:
	rm -f $(OBJ)
	rm -f xed-resources.c 
	rm -f a.out 

