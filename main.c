#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
//#include "full_search.c"

#include "full_search.h"

int main(int argc, char *argv[]) {

	GtkWidget *window;

	gtk_init(&argc, &argv);

	//window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	window = GTK_WIDGET(full_search_window_new());

	gtk_widget_show(window);

	g_signal_connect(window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);  

	gtk_main();

	return 0;
}

/*
int
main (int   argc, char *argv[])
{
  GObject *window;

  gtk_init (&argc, &argv);

  window = full_search_window_new();

  gtk_main ();

  return 0;
}
*/