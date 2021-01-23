#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "xed-full-search-window.h"

#include <gdk/gdkkeysyms.h>
#include <glib/gi18n.h>
#include <string.h>

struct _XedFullSearchWindow
{
	GtkDialog parent_instance;

	GtkWidget *treeview;
	GtkListStore *liststore;
	GtkTreeSelection *treeview_selection;
	GtkWidget *entry;
	GtkCheckButton *regex;
	GtkCheckButton *match_case;
	GtkCheckButton *file_mask;
	GtkComboBox *file_type;
	GtkSourceBuffer *source_buffer;
	GtkSourceView *source_view;

};

G_DEFINE_TYPE (XedFullSearchWindow, xed_full_search_window, GTK_TYPE_DIALOG)

static void
xed_full_search_window_class_init (XedFullSearchWindowClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	
	/* Bind class to template */
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/org/x/editor/ui/xed-full-search-window.ui");
	
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, treeview);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, liststore);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, treeview_selection);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, regex);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, match_case);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, file_mask);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, file_type);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, source_buffer);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, source_view);

}

static void
xed_full_search_window_init (XedFullSearchWindow *window) {

	gtk_widget_init_template (GTK_WIDGET (window));
	
}

XedFullSearchWindow *
xed_full_search_window_new ()
{
	return g_object_new (XED_TYPE_FULL_SEARCH_WINDOW, NULL);
}

