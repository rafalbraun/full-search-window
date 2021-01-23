#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#ifndef XED_FULL_SEARCH_WINDOW_H
#define XED_FULL_SEARCH_WINDOW_H

#include <glib-object.h>
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define XED_TYPE_FULL_SEARCH_WINDOW (xed_full_search_window_get_type ())

G_DECLARE_FINAL_TYPE (XedFullSearchWindow, xed_full_search_window, XED, FULL_SEARCH_WINDOW, GtkDialog)

XedFullSearchWindow *xed_full_search_window_new             (void);







G_END_DECLS

#endif /* XED_FULL_SEARCH_WINDOW_H */