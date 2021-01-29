#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "xed-tree-view.h"

#include <gdk/gdkkeysyms.h>
#include <glib/gi18n.h>
#include <string.h>

#define SIZE 1000

struct _XedTreeView
{
	GtkScrolledWindow parent_instance;

	GtkTreeView     * treeview;
	GtkTreeStore    * treestore;

	GList* expanded_rows_list;
	gchar* filepath;
};

enum {
    COLUMN = 0,
    NUM_COLS
};

G_DEFINE_TYPE (XedTreeView, xed_tree_view, GTK_TYPE_SCROLLED_WINDOW)

static void
xed_tree_view_class_init (XedTreeViewClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	
	/* Bind class to template */
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/org/x/editor/ui/xed-tree-view.ui");

	gtk_widget_class_bind_template_child (widget_class, XedTreeView, treeview);
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

// http://scentric.net/tutorial/sec-sorting.html
gint
sort_iter_compare_func (GtkTreeModel *model,
                      GtkTreeIter  *a,
                      GtkTreeIter  *b,
                      gpointer      userdata)
{
	gint ret;
    gchar *name1, *name2;

    gtk_tree_model_get(model, a, COLUMN, &name1, -1);
    gtk_tree_model_get(model, b, COLUMN, &name2, -1);

    if (name1 == NULL || name2 == NULL)
    {
      if (name1 == NULL && name2 == NULL)
        return 0; /* both equal => ret = 0 */

      ret = (name1 == NULL) ? -1 : 1;
    }
    else
    {
      ret = g_utf8_collate(name1,name2);
    }

    g_free(name1);
    g_free(name2);

    return ret;
}

gboolean
key_pressed_treeview(GtkWidget *treeview, GdkEventKey *event, XedTreeView *window) 
{
    GtkTreeModel     *tree_model;
    GtkTreeSelection *selection;
    GtkTreePath      *treepath;
    GtkTreeIter       parent;
    GList            *rows;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    if (gtk_tree_selection_count_selected_rows (selection) == 1) 
    {
        if (event->keyval == GDK_KEY_Return) 
        {
            tree_model = gtk_tree_view_get_model (GTK_TREE_VIEW(treeview));
            gtk_tree_selection_get_selected (selection, &tree_model, &parent);

            if (gtk_tree_model_iter_has_child (tree_model, &parent)) 
            {
                gtk_tree_selection_select_path (selection, treepath);
                rows = gtk_tree_selection_get_selected_rows (selection, &tree_model);
                treepath = (GtkTreePath*) g_list_first (rows)->data;

                if (gtk_tree_view_row_expanded (GTK_TREE_VIEW(treeview), treepath)) 
                {
                    gtk_tree_view_collapse_row (GTK_TREE_VIEW(treeview), treepath);
                } else 
                {
                    gtk_tree_view_expand_row (GTK_TREE_VIEW(treeview), treepath, FALSE);
                }
            }
        }
    }

    return FALSE;
}

gchar* translate_gtk_iter_to_string (GtkTreeModel *model, GtkTreeIter* iter) 
{
    GtkTreeIter       child, parent;
    gboolean          hasParent;
    gchar            *name, *parent_name, *path;

    parent_name = "";
    path = "";
    child = *iter;

    gtk_tree_model_get (model, &child, COLUMN, &name, -1);

    while ( (hasParent = gtk_tree_model_iter_parent(model, &parent, &child)) == TRUE ) 
    {
        if ( hasParent == TRUE ) 
        {
            gtk_tree_model_get (model, &parent, COLUMN, &parent_name, -1);
            path = g_strconcat(parent_name, "/", path, NULL);
            g_free(parent_name);
            child = parent;
        }
    }

    path = g_strconcat(path, name, NULL);

    g_free(name);

    return path;    
}

void validate_file(GtkTreeModel *model, GtkTreeSelection *selection, XedTreeView *window)
{
    GtkTreeIter child;
    gchar* path;

    gtk_tree_selection_get_selected(selection, &model, &child);
    path = translate_gtk_iter_to_string(model, &child);
    g_print("%s \n", path);
	
	g_free(path);
}

void
popup_menu(GtkWidget *treeview, GdkEventButton *event, XedTreeView *window) 
{

}

gboolean
on_button_pressed(GtkWidget *treeview, GdkEventButton *event, XedTreeView *window) 
{
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       child;
    gchar            *path;

    path = "";

    if (event->type == GDK_2BUTTON_PRESS) 
    {
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

        if (gtk_tree_selection_count_selected_rows(selection) == 1) 
        {

            model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

            validate_file (model, selection, window);

            //g_free(path);
        }

        return TRUE;
    } else if (event->type == GDK_BUTTON_PRESS && event->button == 3) 
    {
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
        if (gtk_tree_selection_count_selected_rows(selection) <= 1) 
        {
            GtkTreePath * path;

            if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview), event->x, event->y, &path, NULL, NULL, NULL)) 
            {
                gtk_tree_selection_unselect_all(selection);
                gtk_tree_selection_select_path(selection, path);
                gtk_tree_path_free(path);
            }
        }
        popup_menu(treeview, event, window);
        return TRUE;
    }
    return FALSE;
}

void
populate_tree_view(XedTreeView *window) 
{
    GtkTreeIter      toplevel;
    GtkTreeView     *treeview;
    gchar           *pathname;

    pathname = window->filepath;
    treeview = window->treeview;

    gtk_tree_store_append(window->treestore, &toplevel, NULL);
    gtk_tree_store_set(window->treestore, &toplevel, COLUMN, pathname, -1);

    populate_tree_store(pathname, GTK_TREE_VIEW(treeview), toplevel, window);

    g_signal_connect (G_OBJECT (treeview), "key-press-event", G_CALLBACK (key_pressed_treeview), window);
    g_signal_connect (G_OBJECT (treeview), "button-press-event", G_CALLBACK (on_button_pressed), window);

	gtk_tree_view_expand_all (treeview);



    GtkTreeSortable *sortable = GTK_TREE_SORTABLE(window->treestore);
    gtk_tree_sortable_set_sort_func(sortable, COLUMN, sort_iter_compare_func,
                                    NULL, NULL);
    gtk_tree_sortable_set_sort_column_id(sortable, COLUMN, GTK_SORT_ASCENDING);

}

void
populate_tree_store(const gchar * filepath, GtkTreeView * tree_view, GtkTreeIter toplevel, XedTreeView *treeview) 
{
    DIR             *dir;
    gchar            path[SIZE];
    struct dirent   *entry;

    GtkTreeIter      child;
    GtkTreeModel 	*tree_model = gtk_tree_view_get_model(tree_view);
    GtkTreeStore 	*tree_store = GTK_TREE_STORE(tree_model);

    if (!(dir = opendir(filepath))) 
    {
        return;
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        if ( (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0) && (entry->d_name[0] != '.') ) 
        {
            snprintf(path, sizeof(path), "%s/%s", filepath, entry->d_name);
            gtk_tree_store_append(tree_store, &child, &toplevel);
            gtk_tree_store_set(tree_store, &child, COLUMN, entry->d_name, -1);

        	if (entry->d_type == DT_DIR) 
        	{
        		populate_tree_store(path, GTK_TREE_VIEW(tree_view), child, treeview);
        	}
        	else if (entry->d_type == DT_REG) 
        	{
        		// nothing
        	}
        }
    }

    closedir(dir);
}
