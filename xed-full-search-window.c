#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include "xed-full-search-window.h"

#include <glib.h>
#include <glib/gstdio.h>

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
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, entry);

}

/////////////////////////////////////////////////////////////////

#define BUFFER  500

static void 
add_to_list(GtkListStore *liststore, const char *occurrence, const char *line, gint lnum, gint start, gint end, const gchar* filepath) 
{
  //GtkTreeView *treeview = NULL;
  //GtkTreeModel *model = NULL;
  //GtkListStore *liststore = NULL;
  GtkTreeIter iter;

  //treeview = GTK_TREE_VIEW(widget);
  //model = gtk_tree_view_get_model(treeview);
  //liststore = GTK_LIST_STORE(model);

  gtk_list_store_append(liststore, &iter);

  // NULL on two last ones because they are not for show, they are start/end indicators for highlight
  gtk_list_store_set(liststore, &iter, 0, occurrence, 1, line, 2, lnum, 3, start, 4, end, -1, filepath, -1);
}

static void 
clear_list(GObject* widget) 
{
  GtkTreeView *treeview = NULL;
  GtkTreeModel *model = NULL;
  GtkListStore *liststore = NULL;

  treeview = GTK_TREE_VIEW(widget);
  model = gtk_tree_view_get_model(treeview);
  liststore = GTK_LIST_STORE(model);

  gtk_list_store_clear(liststore);
}

static gchar* 
extract_filename(const gchar* filepath) 
{
  gchar*  filename;
  gchar** tokens;
  int     tokennum;

  tokens = g_strsplit(filepath, "/", -1);
  tokennum = g_strv_length(tokens);
  filename = g_strdup(tokens[tokennum-1]);
  g_strfreev(tokens);

  return filename;
}

static void 
scan_file(char const* const filename, char const* const pattern, XedFullSearchWindow* window) {
    FILE* file = fopen(filename, "r");    /* should check the result */
    int linenum = 0;
    char line[BUFFER];
    GMatchInfo *match_info = NULL;
    gint match_num, start_pos, end_pos;
    GRegex *regex;
    GError *err = NULL;

    regex = g_regex_new (pattern, 0, 0, &err);

    while (fgets(line, sizeof(line), file)) {

        g_regex_match (regex, line, 0, &match_info);
        while (g_match_info_matches (match_info))
        {
            gchar *word = g_match_info_fetch (match_info, 0);

            g_match_info_fetch_pos(match_info, 0, &start_pos, &end_pos);
            //g_print ("%s \t\t %d \t\t %d \t\t %d \t\t %s ", filename, linenum, start_pos, end_pos, line);

            gchar* aaa = extract_filename (filename);
            add_to_list (window->liststore, line, aaa, linenum, start_pos, end_pos, filename);

            g_free (word);
            g_match_info_next (match_info, NULL);
        }
        g_match_info_free (match_info);
        linenum++;
    }

    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    g_regex_unref (regex);
    fclose(file);
}

static gboolean
is_hidden (const char* entry_name) {
	//g_strcmp0(dp->d_name, ".") != 0 && g_strcmp0(dp->d_name, "..") != 0 && (dp->d_name[0]!='.') {}
	
	if (g_strcmp0(entry_name, ".")  != 0 && // omit current entry
		g_strcmp0(entry_name, "..") != 0 && // omit upper entry
		(entry_name[0]!='.')				// omit entry with dot 
	) {
		return TRUE;
	}
	return FALSE;
}

static int 
list_directory (char* dirname, const char* pattern, XedFullSearchWindow* window) {

    gchar path[BUFFER];
    struct dirent *dp;
    DIR *dir = opendir(dirname);

    // Unable to open directory stream
    if (!dir) {
        return 0;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (is_hidden (dp->d_name))
        {
            snprintf(path, BUFFER, "%s/%s", dirname, dp->d_name); // create name of subdirectory

	        if (dp->d_type == DT_DIR) 
	        {
            	//g_print ("DT_DIR %s \n", path);
	            list_directory (path, pattern, window);
	        }
	        if (dp->d_type == DT_REG) {
				//g_print ("DT_REG %s \n", path);
				scan_file (path, pattern, window);
	        }
        }
    }

    closedir(dir);
}

static void
on_entry_key_press_event (GtkWidget                  *entry,
                          XedFullSearchWindow 	     *window)
{
	//g_print("aaa \n");

	// dziala
	//scan_file ("/home/rafal/IdeaProjects/gtksourceview-my-ide/full_search_folder/resources/xed.gresource.xml", "file");

	GtkEntry* e = GTK_ENTRY(entry);
	guint16 len = gtk_entry_get_text_length (e);
	const gchar *preedit = gtk_entry_get_text (e);

	g_print ("preedit: %s \n", preedit);

	if (len >= 3) {
		gtk_list_store_clear (window->liststore);
		list_directory ("/home/rafal/IdeaProjects/gtksourceview-my-ide/full_search_folder", preedit, window);
		g_print ("done\n");
	}
}

static void
xed_full_search_window_init (XedFullSearchWindow *window) {

	gtk_widget_init_template (GTK_WIDGET (window));

	g_signal_connect (window->entry, "changed",
	                  G_CALLBACK (on_entry_key_press_event), window);

	//g_signal_connect (selector->treeview, "row-activated",
	//                  G_CALLBACK (on_row_activated), selector);	


}

XedFullSearchWindow *
xed_full_search_window_new ()
{
	return g_object_new (XED_TYPE_FULL_SEARCH_WINDOW, NULL);
}

