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
	GtkWidget *selection;
	GtkListStore *liststore;
	GtkWidget *entry;
	GtkCheckButton *regex;
	GtkCheckButton *match_case;
	GtkCheckButton *file_mask;
	GtkComboBox *file_type;
	GtkSourceBuffer *source_buffer;
	GtkSourceView *source_view;
	GtkWidget* scrolled_window;

	gint line_num;
	gchar* search_path;
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
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, selection);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, liststore);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, regex);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, match_case);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, file_mask);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, file_type);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, source_buffer);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, source_view);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, entry);
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, scrolled_window);


}

/////////////////////////////////////////////////////////////////

#define BUFFER  500

static void 
add_to_list(GtkListStore *liststore, const char *occurrence, const char *line, gint lnum, gint start, gint end, const gchar* filepath) 
{
  GtkTreeIter iter;

  gtk_list_store_append(liststore, &iter);

  gtk_list_store_set(liststore, &iter, 0, occurrence, 1, line, 2, lnum, 3, start, 4, end, 5, filepath, -1);
}
/*
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
*/
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

            gchar* aaa = g_path_get_basename (filename);
            add_to_list (window->liststore, line, aaa, linenum+1, start_pos, end_pos, filename);

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
	GtkEntry* e = GTK_ENTRY(entry);
	guint16 len = gtk_entry_get_text_length (e);
	const gchar *preedit = gtk_entry_get_text (e);

	if (len >= 3) {
		gtk_list_store_clear (window->liststore);
		list_directory (window->search_path, preedit, window);
	}
}

///////////////////////////////////////////////////////////////////////////////
// wyłączyć do osobnego pliku

static void
result_func (
    GtkSourceFileLoader * loader,
    GAsyncResult *        res,
    XedFullSearchWindow * window)
{
    gboolean success = FALSE;

    success = gtk_source_file_loader_load_finish (loader, res, NULL);

    g_return_if_fail (success);
}

static void 
load_file (GtkSourceBuffer* buffer, gchar* path, XedFullSearchWindow *            window) 
{
    GtkSourceFileLoader * file_loader;
    GtkSourceFile * src_file;
    GFile * file;

    src_file = gtk_source_file_new ();
    file = g_file_new_for_path (path);

    gtk_source_file_set_location (src_file, file);
    g_object_unref (file);

    file_loader = gtk_source_file_loader_new (buffer, src_file);
    gtk_source_file_loader_load_async (file_loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, (GAsyncReadyCallback) result_func, window);

}

static void set_buffer_language (GtkSourceBuffer* source_buffer, const char* lang_name) {
    GtkSourceLanguageManager *manager;
    GtkSourceLanguage *lang = NULL;

    manager = gtk_source_language_manager_get_default ();
    lang = gtk_source_language_manager_get_language (manager, lang_name);
    
    gtk_source_buffer_set_language (source_buffer, lang);
    gtk_source_buffer_set_highlight_syntax (source_buffer, TRUE);
}

///////////////////////////////////////////////////////////////////////////////

static void
row_activated (GtkTreeView       *tree_view,
               GtkTreePath       *path,
               GtkTreeViewColumn *column,
               XedFullSearchWindow           *window) {
    GtkTreeIter iter;
	gchar *value;

  	gtk_tree_model_get_iter (GTK_TREE_MODEL (window->liststore),
                             &iter,
                             path);

	gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 5, &value,  -1);

    g_print("%s \n", value);

    g_free(value);
}

static void
row_changed (GtkTreeSelection *widget, XedFullSearchWindow *window) {
    GtkTreeIter iter;
	gchar *value;
	gint linenum;

	GtkTreeModel* model = GTK_TREE_MODEL(window->liststore);

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {

		gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 2, &linenum,  -1);
		window->line_num = linenum;

		gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 5, &value,  -1);
		load_file (window->source_buffer, value, window);

    	g_free(value);

    	/////

		//gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 3, &start,  -1);
		//gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 4, &end,  -1);

    	/*
	    GtkTextTagTable *tagtable;
	    GtkTextTag *btag, *wtag;
	    GtkTextIter start_iter, end_iter;

	    gint start, end;
	    start = 0;
	    end = 10;

		g_print ("aa %d %d %d \n", start, end, window->line_num);

	    tagtable = gtk_text_buffer_get_tag_table(GTK_TEXT_BUFFER(window->source_buffer));
	    btag = gtk_text_tag_table_lookup(tagtable, "black_bg");
	    wtag = gtk_text_tag_table_lookup(tagtable, "white_fg");
	    gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(window->source_buffer), &start_iter, window->line_num-1, start);
	    gtk_text_buffer_get_iter_at_line_index(GTK_TEXT_BUFFER(window->source_buffer), &end_iter, window->line_num-1, end);
	    gtk_text_buffer_apply_tag(GTK_TEXT_BUFFER(window->source_buffer), btag, &start_iter, &end_iter);
	    gtk_text_buffer_apply_tag(GTK_TEXT_BUFFER(window->source_buffer), wtag, &start_iter, &end_iter);
		*/
	}
}

static void
adjustment_changed (GtkAdjustment *adjustment,
                    XedFullSearchWindow *window)
{
    gdouble upper, pagesize, factor;
    gint line_count;

    line_count = gtk_text_buffer_get_line_count (GTK_TEXT_BUFFER(window->source_buffer));

    upper = gtk_adjustment_get_upper(GTK_ADJUSTMENT(adjustment));
    pagesize = gtk_adjustment_get_page_size (GTK_ADJUSTMENT(adjustment));
    factor = ( ((gdouble)(window->line_num)) * upper) / (gdouble)line_count - pagesize/2;

    gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment), factor);

}

static void
xed_full_search_window_init (XedFullSearchWindow *window) {

	gtk_widget_init_template (GTK_WIDGET (window));

	g_signal_connect (window->entry, "changed",
	                  G_CALLBACK (on_entry_key_press_event), window);

	g_signal_connect (window->selection, "changed", 
					  G_CALLBACK(row_changed), window);

	g_signal_connect (window->treeview, "row-activated", 
					  G_CALLBACK(row_activated), window);

  	GtkAdjustment* adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(window->scrolled_window));
  	g_signal_connect(adjustment, "changed", G_CALLBACK(adjustment_changed), window);

	window->search_path = "/home/rafal/IdeaProjects/gtksourceview-my-ide/full_search_folder";
	
	set_buffer_language (window->source_buffer, "c");

	//gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(window->source_buffer), "black_bg", "background", "red", NULL); 
  	//gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(window->source_buffer), "white_fg", "foreground", "yellow", NULL);

	gtk_entry_grab_focus_without_selecting (GTK_ENTRY(window->entry));

}

GtkWidget *
xed_full_search_window_new (GtkWindow *parent)
{
//	return g_object_new (XED_TYPE_FULL_SEARCH_WINDOW, NULL);

	return GTK_WIDGET (g_object_new (XED_TYPE_FULL_SEARCH_WINDOW,
	                                 "transient-for", parent,
	                                 NULL));

}

