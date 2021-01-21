#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

gboolean 
_main_loop (gpointer treeview);

GObject* 
full_search_window_new();



gchar* 
extract_filename(gchar* filepath);

gchar* 
read_file(gchar* filename);

gchar* 
escape_ampersands(gchar* str);

int 
search_process(gchar* needle);

int 
count_lines(gchar* contents, int len);

void 
add_to_list(GObject* widget, gchar *str0, gchar *str1, gchar* str2, gchar* str3, gchar* str4, gchar* str5);

void 
clear_list(GObject* widget);

void
preedit_changed (GtkEntry *widget, gpointer treeview);

void 
on_changed_full_search(GtkTreeSelection *widget, gpointer textbufferscroll);

void
adjustment_changed (GtkAdjustment *adjustment, gpointer userdata);

void
row_activated (GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);

void 
combo_selected(GtkWidget *widget, gpointer user_data);
