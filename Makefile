CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --libs --cflags glib-2.0`
GLIB_COMPILE_RESOURCES = /usr/local/bin/glib-compile-resources
COMPONENTS=xed-resources.c xed-highlight-mode-dialog.o xed-highlight-mode-selector.o xed-full-search-window.o xed-tree-view.o

all: test_treeview test_search 

#a.out: main.c $(COMPONENTS)
#	$(CC) $(GLIB) main.c $(COMPONENTS) $(GTK) -o a.out

test_search: test_search.c $(COMPONENTS)
	$(CC) $(GLIB) test_search.c $(COMPONENTS) $(GTK) -o test_search

test_treeview: test_treeview.c $(COMPONENTS)
	$(CC) $(GLIB) test_treeview.c $(COMPONENTS) $(GTK) -o test_treeview

full_search.o: full_search.c
	$(CC) -c $(GLIB) full_search.c $(GTK) -o full_search.o

xed-highlight-mode-dialog.o: xed-highlight-mode-dialog.c
	$(CC) -c $(GLIB) xed-highlight-mode-dialog.c $(GTK) -o xed-highlight-mode-dialog.o

xed-highlight-mode-selector.o: xed-highlight-mode-selector.c
	$(CC) -c $(GLIB) xed-highlight-mode-selector.c $(GTK) -o xed-highlight-mode-selector.o

xed-full-search-window.o: xed-full-search-window.c
	$(CC) -c $(GLIB) xed-full-search-window.c $(GTK) -o xed-full-search-window.o

xed-tree-view.o: xed-tree-view.c
	$(CC) -c $(GLIB) xed-tree-view.c $(GTK) -o xed-tree-view.o

xed-resources.c: ./resources/xed.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=./resources --generate-dependencies ./resources/xed.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) --target=xed-resources.c --sourcedir=./resources --generate-source ./resources/xed.gresource.xml

clean:
	rm a.out xed-resources.c *.o test_treeview



