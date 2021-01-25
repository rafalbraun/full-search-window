#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "xed-tree-view.h"

#include <gdk/gdkkeysyms.h>
#include <glib/gi18n.h>
#include <string.h>

struct _XedTreeView
{
	GtkTreeView parent_instance;


};

G_DEFINE_TYPE (XedTreeView, xed_tree_view, GTK_TYPE_TREE_VIEW)

static void
xed_tree_view_class_init (XedTreeViewClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	
	/* Bind class to template */
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/org/x/editor/ui/xed-tree-view.ui");

}

static void
xed_tree_view_init (XedTreeView *treeview)
{

	treeview = xed_tree_view_get_instance_private (treeview);

	gtk_widget_init_template (GTK_WIDGET (treeview));
}

XedTreeView *
xed_tree_view_new ()
{
	return g_object_new (XED_TYPE_TREE_VIEW, NULL);
}

