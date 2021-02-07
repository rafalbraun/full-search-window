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
	GtkAdjustment* vadjustment;

	gint line_num, start, end, flag;
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
	gtk_widget_class_bind_template_child (widget_class, XedFullSearchWindow, vadjustment);


}

/////////////////////////////////////////////////////////////////


static void 
add_to_list(GtkListStore *liststore, const char *occurrence, const char *line, gint lnum, gint start, gint end, const gchar* filepath) 
{
  GtkTreeIter iter;

  gtk_list_store_append(liststore, &iter);

  gtk_list_store_set(liststore, &iter, 0, occurrence, 1, line, 2, lnum, 3, start, 4, end, 5, filepath, -1);
}

#define MAXCHAR 1000

char buf[MAXCHAR];
char str[MAXCHAR];

// https://stackoverflow.com/questions/29866616/how-to-find-all-occurrences-of-a-substring-in-c
int find_all_substr_in_line (const char* str, const char* aStrRegex, int line, const char* filename, XedFullSearchWindow* window) {

	char *ptr = strstr(str, aStrRegex);
	
	while (ptr) {

	    //printf ("[%s] %d :: %s ", filename, line, ptr);
	    
    	size_t len = strlen(aStrRegex);
    	strncpy(buf, ptr, len);
		buf[len] = '\0';

		//printf("%d :: %s ", line, buf);

		int start = ptr - str;
		int end = start + strlen(aStrRegex);

		GtkListStore* liststore = GTK_LIST_STORE (window->liststore);
		add_to_list (liststore, g_strdup(str), g_strdup(filename), line+1, start, end, filename);

	    ptr = strstr(ptr+1, aStrRegex);

	    //printf("\n");
	}

  	return 0;
}

int find_all_substr_in_file (const char* filename, const char* aStrRegex, XedFullSearchWindow* window) {
    FILE *fp;
    int line=0, col=0;

    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s", filename);
        return 1;
    }

    while (fgets(str, MAXCHAR, fp) != NULL) {
		find_all_substr_in_line (str, aStrRegex, line, filename, window);
        line++;
    }
    fclose(fp);

    return 0;
}

int find_all_substr_in_all_files (const char** filenamesArr, int filenamesArrSize, const char* aStrRegex, XedFullSearchWindow* window) {
	for (int i=0; i<filenamesArrSize; i++) {
		find_all_substr_in_file (filenamesArr[i], aStrRegex, window);
	}
}

/////////////////////////////////////////////////////////////////

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
/*
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
				int len = strlen(path);
				if (path[len-2]=='.' && (path[len-1]=='h' || path[len-1]=='c'))   {
					scan_file (path, pattern, window);
				}
	        }
        }
    }

    closedir(dir);
}
*/
const char* filenamesArr[] = {
"full_search.c",
"full_search.h",
"full_search.ui",
"glib_regex.c",
"main.c",
"Makefile",
"pcredemo.c",
"README.md",
"scan_file.c",
"scan_file.h",
"test_scan.c",
"test_scan_regex.c",
"test_search.c",
"test_treeview.c",
"xed-full-search-window.c",
"xed-full-search-window.h",
"xed-highlight-mode-dialog.c",
"xed-highlight-mode-dialog.h",
"xed-highlight-mode-selector.c",
"xed-highlight-mode-selector.h",
"xed-resources.c",
"xed-tree-view.c",
"xed-tree-view.h"
};

gboolean
on_entry_key_press_event (GtkWidget *entry,
               //GdkEventKey  *event,
               //char     *preedit,
               XedFullSearchWindow 	     *window)
{
	GtkEntry* e = GTK_ENTRY(entry);
	guint16 len = gtk_entry_get_text_length (e);
	const gchar *preedit = gtk_entry_get_text (e);

	// MEGA MEGA WAŻNE
	
	//gtk_tree_selection_unselect_all (GTK_TREE_SELECTION(window->selection));
	//g_print ("ffffff %d \n", gtk_tree_selection_count_selected_rows (GTK_TREE_SELECTION(window->selection)));

	g_print ("[INFO] %s \n", preedit);

	/*
	TODO : naprawić

	if (event->keyval == GDK_KEY_BackSpace) {
		GtkTextIter start, end;
		gtk_text_buffer_get_bounds (GTK_TEXT_BUFFER(window->source_buffer), &start, &end);
		gtk_text_buffer_delete (GTK_TEXT_BUFFER(window->source_buffer), &start, &end);
	}*/

	if (len >= 3) {
		// TODO
		// tutaj usuwamy i dodajemy wiec pewnie to tutaj dac trzeba zapamietac -> unselect_all -> przywrocic
		window->flag = 1;

		gtk_list_store_clear (window->liststore);
		find_all_substr_in_all_files (filenamesArr, 24, preedit, window);

		window->flag = 0;
	}

	return FALSE;
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

    if (success) {
		GtkTextIter iter1, iter2;
		gtk_text_buffer_get_iter_at_line_index (GTK_TEXT_BUFFER(window->source_buffer), &iter1, window->line_num-1, window->start);
		gtk_text_buffer_get_iter_at_line_index (GTK_TEXT_BUFFER(window->source_buffer), &iter2, window->line_num-1, window->end);
		gtk_text_buffer_apply_tag_by_name (GTK_TEXT_BUFFER(window->source_buffer), "blue_fg", &iter1, &iter2);
	}

    g_return_if_fail (success);
}

static void 
load_file (GtkSourceBuffer* buffer, gchar* path, XedFullSearchWindow *window) 
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
row_changed (GtkTreeSelection *widget/*, GdkEvent  *event*/,XedFullSearchWindow *window) {
    GtkTreeIter iter;
	gchar *value;
	gint linenum, start, end;

	GtkTreeModel* model = GTK_TREE_MODEL(window->liststore);

	//g_print ("aaa %d %d \n", widget == NULL, gtk_tree_selection_count_selected_rows (GTK_TREE_SELECTION(widget)));

	// TODO
	// 1. zapamietujemy wybrany
	// 2. robimy clear na liście
	// 3. czyscimy bufor
	// 4. wybieramy zapamietany wczesniej
	// 5. ustawiamy nowa zawartosc bufora
	// 
	// ale po sprawdzeniu czy event to był klik usera (a nie add albo delete)
	// 

	//gtk_tree_selection_unselect_all (GTK_TREE_SELECTION(window->selection));
	//gint aaa = gtk_tree_selection_count_selected_rows (GTK_TREE_SELECTION(window->selection));
	//g_print ("aaaaaaaaa %d \n", aaa);

	if (window->flag == 0) {
	    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(window->selection), &model, &iter)) {

			gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 2, &linenum,  -1);
			window->line_num = linenum;

			gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 3, &start,  -1);
			window->start = start;

			gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 4, &end,  -1);
			window->end = end;

			gtk_tree_model_get(GTK_TREE_MODEL (window->liststore), &iter, 5, &value,  -1);
			load_file (window->source_buffer, value, window);

	    	g_free(value);

		}
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

/**
w momencie gdy mamy wybrany row, a następnie dodajemy nowe w wyniku wyszukania, to selection ma nadal wybrany jakiś row i względem niego próbuje zupdateować bufor

okazuje się, że GtkTreeSelection robi unselect_all dopiero jak pododaje wszystkie rows które ma do dodania -> czy możemy cokolwiek z tym zrobić?

okej, jednak problem jest wtedy gdy rekordy są usuwane (robimy clear) , czyli przy każdym delete robi się select i jakiś wiersz jest zawsze wybrany ???

*/
gboolean
user_function (GtkTreeView *treeview,
               gpointer     user_data){
	g_print ("aaa \n");
}

static void
xed_full_search_window_init (XedFullSearchWindow *window) {

	gtk_widget_init_template (GTK_WIDGET (window));

	window->search_path = "/home/rafal/IdeaProjects/xed";

	g_signal_connect (window->entry, "changed",
	                  G_CALLBACK (on_entry_key_press_event), window);

	//g_signal_connect (window->selection, "changed", 
	//				  G_CALLBACK(row_changed), window);

	g_signal_connect (window->treeview, "row-activated", 
					  G_CALLBACK(row_activated), window);

	g_signal_connect (window->treeview, "cursor-changed", 
					  G_CALLBACK(row_changed), window);

  	g_signal_connect (window->vadjustment, "changed", 
  					  G_CALLBACK(adjustment_changed), window);
	
	set_buffer_language (window->source_buffer, "c");

  	gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(window->source_buffer), "blue_fg", 
      		"background", "red", NULL); 

	gtk_entry_grab_focus_without_selecting (GTK_ENTRY(window->entry));

	window->flag = 0;
}

GtkWidget *
xed_full_search_window_new (GtkWindow *parent)
{
//	return g_object_new (XED_TYPE_FULL_SEARCH_WINDOW, NULL);

	return GTK_WIDGET (g_object_new (XED_TYPE_FULL_SEARCH_WINDOW,
	                                 "transient-for", parent,
	                                 NULL));

}

