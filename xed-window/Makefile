CC=gcc
GTK=`pkg-config --cflags --libs gtk+-3.0 gtksourceview-4`
GLIB=`pkg-config --libs --cflags glib-2.0`
PCRE=`pkg-config --cflags --libs libpcre` 
GLIB_COMPILE_RESOURCES = /usr/local/bin/glib-compile-resources

OBJ=xed-window.o 
SRC=xed-window.c 

LIBS=-L/usr/local/lib/x86_64-linux-gnu -lxml2 -lgthread-2.0 -pthread -lgtksourceview-3.0 -lpeas-gtk-1.0 -lpeas-1.0 -Wl,--export-dynamic -lgmodule-2.0 -pthread -lgirepository-1.0 -lxapp -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lcairo -lX11 -L/usr/local/lib/x86_64-linux-gnu -lgirepository-1.0 -lgobject-2.0 -lglib-2.0 -lm 

DEPS=-Wno-deprecated-declarations -w -DHAVE_CONFIG_H -I. -I..  -I.. -I. -pthread -I/usr/local/include -I/usr/local/include/gio-unix-2.0 -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gtksourceview-3.0 -I/usr/include/libpeas-1.0 -I/usr/include/gobject-introspection-1.0 -I/usr/include/xapp -I/usr/include/libxml2 -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/cairo -I/usr/include/libdrm -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16  -Wall -Wstrict-prototypes -Wnested-externs -Werror=missing-prototypes -Werror=implicit-function-declaration -Werror=pointer-arith -Werror=init-self -Werror=format=2 -Werror=missing-include-dirs -Werror=return-type  -pthread -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gobject-introspection-1.0  

DIRS=-DHAVE_CONFIG_H -I. -I..  -I.. -I. -pthread -I/usr/local/include -I/usr/local/include/gio-unix-2.0 -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gtksourceview-3.0 -I/usr/include/libpeas-1.0 -I/usr/include/gobject-introspection-1.0 -I/usr/include/xapp -I/usr/include/libxml2 -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/cairo -I/usr/include/libdrm -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16  -Wall -Wstrict-prototypes -Wnested-externs -Werror=missing-prototypes -Werror=implicit-function-declaration -Werror=pointer-arith -Werror=init-self -Werror=format=2 -Werror=missing-include-dirs -Werror=return-type  -pthread -I/usr/local/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gobject-introspection-1.0 


COMPONENTS=xed-resources.c $(OBJ)

all: a.out 

a.out : main.c $(COMPONENTS)
	$(CC) $(DIRS) $(GLIB) main.c $(COMPONENTS) $(GTK) $(DEPS)  -o a.out $(LIBS)


$(OBJ): $(SRC)
	$(CC) $(DIRS) -c $(GLIB) $(SRC) $(GTK) $(DEPS)  -o $(OBJ) $(LIBS)


xed-resources.c: ./resources/xed.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=./resources --generate-dependencies ./resources/xed.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) --target=xed-resources.c --sourcedir=./resources --generate-source ./resources/xed.gresource.xml

clean:
	rm -f $(OBJ)
	rm -f xed-resources.c 
	rm -f a.out 

