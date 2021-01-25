#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>
//#include "full_search.c"

#include "xed-highlight-mode-selector.h"
#include "xed-highlight-mode-dialog.h"
#include "xed-full-search-window.h"
#include "xed-tree-view.h"

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *text_view;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Mnemonic");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  
  text_view = GTK_WIDGET(xed_tree_view_new ());
  
  gtk_container_add(GTK_CONTAINER(window), text_view);  

  gtk_widget_show_all(window);
  
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 

  gtk_main();

  return 0;
}




/*
void print_msg(GtkWidget *widget, gpointer window) {
  GtkWidget* w = GTK_WIDGET(xed_full_search_window_new (window));
  gtk_widget_show (w);
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *halign;
  GtkWidget *full_search;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Mnemonic");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  
  button = gtk_button_new_with_mnemonic("_Button");

  g_signal_connect(button, "clicked", 
      G_CALLBACK(print_msg), NULL);  
  
  halign = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  gtk_container_add(GTK_CONTAINER(halign), button);
  gtk_container_add(GTK_CONTAINER(window), halign);  

  gtk_widget_show_all(window);
  
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 

  gtk_main();

  return 0;
}
*/




/*
void print_msg(GtkWidget *widget, gpointer window) {
  GtkWidget* w = xed_highlight_mode_dialog_new (window);
  gtk_widget_show (w);
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
  
  button = gtk_button_new_with_mnemonic("_Button");
  selector = GTK_WIDGET(xed_highlight_mode_selector_new ());

  g_signal_connect(button, "clicked", 
      G_CALLBACK(print_msg), NULL);  
  
  halign = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(halign), button);
  gtk_container_add(GTK_CONTAINER(window), halign);  

  gtk_widget_show_all(window);
  
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 

  gtk_main();

  return 0;
}
*/

