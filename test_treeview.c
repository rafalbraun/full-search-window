#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#include "xed-highlight-mode-selector.h"
#include "xed-highlight-mode-dialog.h"
#include "xed-full-search-window.h"
#include "xed-tree-view.h"

#define EXPANDED_ROWS_FILE "/home/rafal/IdeaProjects/gtksourceview-my-ide/application/opened_tabs.txt"

int main (int argc, char *argv[]) 
{

  GtkWidget *window;
  XedTreeView *treeview;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Treeview");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  
  treeview = xed_tree_view_new ();
  
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(treeview));  

  gtk_widget_show_all(GTK_WIDGET(window));
  
  g_signal_connect (G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL); 
  
  g_signal_connect (G_OBJECT (treeview->treeview), "button-press-event", 
      G_CALLBACK (on_button_pressed), window);
  
  g_signal_connect (G_OBJECT (treeview->treeview), "key-press-event", 
      G_CALLBACK (key_pressed_treeview), window);


  gtk_main();
  
  return 0;
}
