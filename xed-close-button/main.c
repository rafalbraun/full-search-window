#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
//#include "full_search.c"

//#include "xed-highlight-mode-selector.h"
//#include "xed-highlight-mode-dialog.h"
//#include "xed-full-search-window.h"

#include "xed-close-button.h"

void print_msg(GtkWidget *widget, gpointer window) {

  g_print("Button clicked\n");

}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *selector;
  GtkWidget *halign;
  GtkWidget *full_search;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Mnemonic");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  
  button = xed_close_button_new();
  //selector = GTK_WIDGET(xed_highlight_mode_selector_new ());
  //full_search = GTK_WIDGET(xed_full_search_window_new ());

  g_signal_connect(button, "clicked", 
      G_CALLBACK(print_msg), NULL);  
  
  halign = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); //gtk_alignment_new(0, 0, 0, 0);
  //gtk_container_add(GTK_CONTAINER(halign), selector);
  gtk_container_add(GTK_CONTAINER(halign), button);
  gtk_container_add(GTK_CONTAINER(window), halign);  

  gtk_widget_show_all(window);
  
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 

  gtk_main();

  return 0;
}


