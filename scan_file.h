#define GLIB_VERSION_2_28               (G_ENCODE_VERSION (2, 28))
#define GLIB_VERSION_MIN_REQUIRED       GLIB_VERSION_2_28

#include <glib-object.h>
#include <gtksourceview/gtksource.h>

#include "xed-full-search-window.h"

#define BUFFER  500

static void 
scan_file(char const* const filename, char const* const pattern, XedFullSearchWindow* window);

