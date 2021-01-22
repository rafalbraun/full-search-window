#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
//#include "full_search.c"

#include "xed-highlight-mode-dialog.h"
#include "xed-highlight-mode-selector.h"

void print_msg(GtkWidget *widget, gpointer window) {

  g_print("Button clicked\n");

  GtkWidget* w = xed_highlight_mode_dialog_new (window);

  gtk_widget_show (w);
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *halign;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Mnemonic");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  
  button = gtk_button_new_with_mnemonic("_Button");
  
  g_signal_connect(button, "clicked", 
      G_CALLBACK(print_msg), NULL);  
  
  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), button);
  gtk_container_add(GTK_CONTAINER(window), halign);  

  gtk_widget_show_all(window);
  
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 

  gtk_main();

  return 0;
}


/*
int main(int argc, char *argv[]) {

	GtkWidget *window;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//window = GTK_WIDGET(full_search_window_new());

	// xed_highlight_mode_dialog_new

	gtk_widget_show(window);

	g_signal_connect(window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);  

	gtk_main();

	return 0;
}
*/
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