#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "xed-tree-view.h"

#include <gdk/gdkkeysyms.h>
#include <glib/gi18n.h>
#include <string.h>

#define SIZE 1000

struct _XedTreeView
{
	GtkTreeView parent_instance;

	GtkTreeStore    * treestore;

	GList* expanded_rows_list;
	gchar* filepath;
};

enum {
    COLUMN = 0,
    NUM_COLS
};

G_DEFINE_TYPE (XedTreeView, xed_tree_view, GTK_TYPE_TREE_VIEW)

static void
xed_tree_view_class_init (XedTreeViewClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	
	/* Bind class to template */
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/org/x/editor/ui/xed-tree-view.ui");

	gtk_widget_class_bind_template_child (widget_class, XedTreeView, treestore);

}

static void
xed_tree_view_init (XedTreeView *treeview)
{

	treeview = xed_tree_view_get_instance_private (treeview);

	gtk_widget_init_template (GTK_WIDGET (treeview));

	treeview->filepath = "/home/rafal/IdeaProjects/gtksourceview-my-ide/application";
	
	populate_tree_view (treeview);

}

XedTreeView *
xed_tree_view_new ()
{
	return g_object_new (XED_TYPE_TREE_VIEW, NULL);
}

void
populate_tree_view(XedTreeView *treeview) 
{
    //GtkTreeStore    *treestore;
    GtkTreeIter      toplevel;
    gchar           *pathname;
    //GtkTreePath     *treepath;
    //GtkTreeView     *treeview;

    pathname = treeview->filepath;
    //treestore = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

    gtk_tree_store_append(treeview->treestore, &toplevel, NULL);
    gtk_tree_store_set(treeview->treestore, &toplevel, COLUMN, pathname, -1);

    //load_expanded_rows_from_file (user_data);
    populate_tree_store(pathname, GTK_TREE_VIEW(treeview), toplevel, treeview);

    //g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), user_data);
    //g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), user_data);

}

int cmpfunc (const void *a, const void *b) 
{
    return g_strcmp0(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name);
}

void
populate_tree_store(const gchar * filepath, GtkTreeView * tree_view, GtkTreeIter toplevel, XedTreeView *treeview) 
{
    DIR             *dir;
    GtkTreeIter      child;
    struct dirent   *entry;
    struct dirent   *entries;
    int              count = 0, i = 0;
    gchar            path[SIZE];

    GtkTreeModel *tree_model = gtk_tree_view_get_model(tree_view);
    GtkTreeStore *tree_store = GTK_TREE_STORE(tree_model);

    if (!(dir = opendir(filepath))) 
    {
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        //if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            count++;
        //}
    }
    closedir(dir);

    dir = opendir(filepath); /* There should be error handling after this */
    entries = g_new0 (struct dirent, count);

    while ((entry = readdir(dir)) != NULL) 
    {
        // HERE ADD FILTER FUNCTION
        if ( (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0) && (entry->d_name[0] != '.') ) 
        {
            entries[i++] = *entry;
        }
    }
    qsort (entries, count, sizeof(struct dirent), cmpfunc);

    for (int i=0; i<count; i++) 
    {
        entry = &(entries[i]);
        if (entry->d_type == DT_DIR) 
        {
            snprintf(path, sizeof(path), "%s/%s", filepath, entry->d_name);
            gtk_tree_store_append(tree_store, &child, &toplevel);
            gtk_tree_store_set(tree_store, &child, COLUMN, entry->d_name, -1);
            populate_tree_store(path, GTK_TREE_VIEW(tree_view), child, treeview);

            GList * expanded_rows_list = treeview->expanded_rows_list;

            guint len = g_list_length(expanded_rows_list);
            if (len == 0) 
            {
                break;
            }
            for (int i=0; i<len; i++) 
            {
                gchar* element = g_list_nth_data(expanded_rows_list, i);
                if (strcmp(element, path) == 0) 
                {
                    GtkTreePath* tree_path = gtk_tree_model_get_path (tree_model, &child);
                    gtk_tree_view_expand_to_path (tree_view, tree_path);
                }
            }
        }
        if (entry->d_type == DT_REG) 
        {
            gtk_tree_store_append(tree_store, &child, &toplevel);
            gtk_tree_store_set(tree_store, &child, COLUMN, entry->d_name, -1);
        }
    }

    g_free (entries);
    closedir(dir);
}
