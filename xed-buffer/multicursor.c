#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

//
// https://developer.gnome.org/gtk3/stable/TextWidget.html
// https://stackoverflow.com/questions/26052313/gtk-warning-invalid-text-buffer-iterator-when-writing-to-the-same-text-view-mul
//

struct DispatchData {
    GtkIMContext *im_context;
    GtkTextBuffer *buffer;
    GList *list;
    guint mark_max;
    gboolean multicursor; // flag
};

enum KeyButton{
	LEFT, RIGHT, UP, DOWN, ESCAPE
}; 

gboolean escape_callback(struct DispatchData *data) {
    /* same as ESCAPE */
    for (int i=0; i<data->mark_max; i++) {
		GList *t = g_list_nth (data->list, i);
		GtkTextMark* mark = ((GtkTextMark*) t->data);
		gtk_text_mark_set_visible (mark, FALSE);
		gtk_text_buffer_delete_mark (data->buffer, mark);
    }
    g_list_free(data->list);
    data->list = NULL;
    data->mark_max = 0;
    data->multicursor = FALSE;
}

gboolean on_key_press(GtkWidget *window, GdkEventKey* event, struct DispatchData *data)
{
    GtkTextIter iter;
    enum KeyButton kb;
    gchar c = (gchar)event->keyval;

    // Just message for now: multicursor: ctrl + shift + l 
    if ( (event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_L) ) {
		g_print("[INFO] MULTICURSOR CTRL + SHIFT + l\n");
		return FALSE;
    }

    if ( (event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_u) ) {
		g_print("[INFO] MULTICURSOR CTRL + u\n");
		return FALSE;
    }

    if ( (event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_b) ) {
		g_print("[INFO] MULTICURSOR CTRL + b\n");
		return FALSE;
    }


	// GdkModifierType modifiers;
	// modifiers = gtk_accelerator_get_default_mod_mask ();

	//   if ((event->state & modifiers) == GDK_MOD1_MASK)
	//   {
	//     g_print ("Alt was pressed\n");
	//   }
	//   if ((event->state & modifiers) == GDK_CONTROL_MASK)
	//   {
	//     g_print ("Control was pressed\n");
	//   }



	// save in what direction multicursor should be moved in
	if ((event->type == GDK_KEY_PRESS)) {
		switch (event->keyval) {
			case GDK_Left:
				g_print ("Left\n");
				kb = LEFT;
				break;
			case GDK_Right:
				g_print ("Right\n");
				kb = RIGHT;
				break;
			case GDK_KEY_Escape:
				g_print ("Escape\n");
				//kb = ESCAPE;

				escape_callback ( data );

				return TRUE; // !important
				break;
		}
	}

	// move multicursor in given direction
	for (int i=0; i<data->mark_max; i++) {

		GList *t = g_list_nth (data->list, i);
		GtkTextMark* mark = ((GtkTextMark*) t->data);
		g_print("[INFO] Mark name: %s\n", gtk_text_mark_get_name(mark));

		gtk_text_buffer_get_iter_at_mark(data->buffer, &iter, mark);
		gint offset = gtk_text_iter_get_offset (&iter);
		g_print ("[INFO] ok: %d \n", offset);

		gtk_text_iter_set_offset(&iter, offset);
		if (kb == LEFT)
		gtk_text_iter_backward_char(&iter);
		if (kb == RIGHT)
		gtk_text_iter_forward_char(&iter);

		gtk_text_buffer_move_mark (data->buffer, mark, &iter);
	}

    // if the input char is alpha-char we should write it 
    if ( (event->keyval >= 0x041 && event->keyval <= 0x05a) || (event->keyval >= 0x061 && event->keyval <= 0x07a) || (event->keyval == 0x020) || (event->keyval == 0x02e) ) {
		if (data->multicursor) {
		GList *tmp_list = NULL;

		for (int i=0; i<data->mark_max; i++) {
			GList *t = g_list_nth (data->list, i);
			GtkTextMark* cursor = ((GtkTextMark*) t->data);
			gtk_text_buffer_get_iter_at_mark (data->buffer, &iter, cursor);
			gtk_text_buffer_insert (data->buffer, &iter, &c, sizeof(gchar));  // at saved marks

			//gtk_text_iter_forward_char(&iter);

			GtkTextMark *m = gtk_text_mark_new (NULL, 1);
			gtk_text_buffer_add_mark(data->buffer, m, &iter);
			gtk_text_mark_set_visible (m, TRUE);
			tmp_list = g_list_append (tmp_list, m);

			gtk_text_mark_set_visible (cursor, FALSE);
			gtk_text_buffer_delete_mark (data->buffer, cursor);
		}
		g_list_free(data->list);
		data->list = tmp_list;

		} else {
			gtk_text_buffer_insert_at_cursor (data->buffer, &c, sizeof(gchar)); // at current mark
		}
    }

    g_print("[INFO] get_position: 0x%04x : %d \n", event->keyval, event->keyval);
    g_print("[INFO] Char added %c\n", c);

    /* block normal callback for character input  */
    return gtk_im_context_filter_keypress(data->im_context, event);
}

//
// https://people.gnome.org/~shaunm/girdoc/C/Gtk.TextBuffer.get_iter_at_mark.html
// https://www.gnu.org/software/guile-gnome/docs/gtk/html/GtkTextMark.html
// 

/**
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
pewnie nie mogę przechwycić key pressa
ALE
mogę pozwolić na dodanie znaku do text buffera
następnie skopiować dopiero co dodany znak
dodać skopiowany znak pod text markami

to wyżej to bullshit, trzeba tak:
https://stackoverflow.com/questions/40011838/how-to-receive-characters-from-input-method-in-gtk2

alokacja obiektu i przekazywanie w callbacku:
https://stackoverflow.com/questions/26052313/gtk-warning-invalid-text-buffer-iterator-when-writing-to-the-same-text-view-mul

*/
// https://stackoverflow.com/questions/24386412/how-to-detect-mouse-click-over-an-image-in-gtk
// https://developer.gnome.org/gdk3/stable/gdk3-Events.html
gboolean on_button_release(GtkWidget* widget,
                           GdkEventButton *event, 
                           struct DispatchData *data) {
	// 
	// Do kasowania text marków:
	//    https://developer.gnome.org/gtk2/stable/GtkTextBuffer.html
	//      o gtk_text_buffer_add_mark
	//      o gtk_text_buffer_delete_mark
	// 
	if ((event->button == 1)) {
		if (event->state & GDK_CONTROL_MASK) {
			GtkTextIter iter;
			gtk_text_buffer_get_iter_at_mark(data->buffer,
			  &iter, gtk_text_buffer_get_insert(data->buffer));

			char str[10];
			sprintf(str, "%d", data->mark_max);

			GtkTextMark *mark = gtk_text_mark_new (str, 1);
			data->list = g_list_append (data->list, mark);

			gtk_text_buffer_add_mark(data->buffer, mark, &iter);
			gtk_text_mark_set_visible (mark, TRUE);
			data->mark_max++;

			g_print("[INFO] add cursor by LMB \n");

			data->multicursor = TRUE;

			return FALSE; // !important
		} else {

			/* same as ESCAPE */
			escape_callback( data );

		}
	}
	return FALSE;
}

static gboolean commit_callback(GtkWidget *widget,
                                GdkEventKey *event,
                                struct DispatchData *data) {
	g_print("[INFO] commit_callback: 0x%02x : %d \n", event->keyval & 0xff, event->keyval);
	return FALSE;
}

int main(int argc, gchar *argv[]) {

	GtkWidget *window;
	GtkWidget *view;
	GtkWidget *vbox;
  
	GtkTextBuffer *buffer;
	//GtkTextIter start, end;
	GtkTextIter iter;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 350, 300);
	gtk_window_set_title(GTK_WINDOW(window), "Search & highlight");
	GTK_WINDOW(window)->allow_shrink = TRUE;

	GtkIMContext *im_context = gtk_im_multicontext_new();
	GdkWindow *gdk_window = gtk_widget_get_window(GTK_WIDGET(window));
	gtk_im_context_set_client_window(im_context, gdk_window);
	gtk_im_context_focus_in(im_context);

	vbox = gtk_vbox_new(FALSE, 0);
	view = gtk_text_view_new();
	gtk_widget_add_events(view, GDK_BUTTON_PRESS_MASK);
	gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
	for (int i=0; i<10; i++) {
		gtk_text_buffer_insert(buffer, &iter, "Gtk-Message: Failed to load module \"canberra-gtk-module\"\n", -1);
	}

	gtk_text_buffer_create_tag(buffer, "gray_bg", 
		"background", "lightgray", NULL); 
	gtk_container_add(GTK_CONTAINER(window), vbox);

	struct DispatchData *data = g_new0(struct DispatchData, 1);
	data->buffer = buffer;
	data->im_context = im_context;
	data->list = NULL;
	data->mark_max = 0;

	g_signal_connect(G_OBJECT(window), "destroy",
		G_CALLBACK(gtk_main_quit), data);

	g_signal_connect(G_OBJECT(view), "button-release-event",
	  G_CALLBACK(on_button_release), data);

	g_signal_connect (G_OBJECT(window), "key-press-event", 
		G_CALLBACK (on_key_press), data);

	g_signal_connect(im_context, "commit",
		G_CALLBACK(commit_callback), data);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}



