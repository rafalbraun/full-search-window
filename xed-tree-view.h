#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef XED_TREE_VIEW_H
#define XED_TREE_VIEW_H

#include <glib-object.h>
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define XED_TYPE_TREE_VIEW (xed_tree_view_get_type ())

G_DECLARE_FINAL_TYPE (XedTreeView, xed_tree_view, XED, TREE_VIEW, GtkScrolledWindow)

XedTreeView *xed_tree_view_new             ();

void
populate_tree_view(XedTreeView *treeview) ;

void
populate_tree_store(const gchar * filepath, GtkTreeView * tree_view, GtkTreeIter toplevel, XedTreeView *treeview) ;

gboolean
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, XedTreeView *window);

gboolean
key_pressed_treeview(GtkWidget *treeview, GdkEventKey *event, XedTreeView *window);

typedef enum pm {
	COLLAPSE,
	EXPAND
} PopupMenu;

enum {
    COLUMN = 0,
    NUM_COLS
};


struct _XedTreeView
{
	GtkScrolledWindow parent_instance;

	GtkTreeView     * treeview;
	GtkTreeStore    * treestore;

	GList* expanded_rows_list;
	gchar* filepath;
};


G_END_DECLS

#endif /* XED_TREE_VIEW_H */
