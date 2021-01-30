#include "scan_file.h"

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

    // TODO -> find fix for this !!!
    if (err != NULL) {
    	g_print ("[WARN] g_regex_new error \n");
    	return;
    }

    while (fgets(line, sizeof(line), file)) {

        g_regex_match (regex, line, 0, &match_info);
        
        // TODO -> find fix for this !!!
        if (match_info == NULL) {
	    	g_print ("[WARN] g_regex_match error \n");
        	return;
        }

        while (g_match_info_matches (match_info))
        {
            gchar *word = g_match_info_fetch (match_info, 0);

            g_match_info_fetch_pos(match_info, 0, &start_pos, &end_pos);

            gchar* aaa = g_path_get_basename (filename);

            if (!g_file_test (filename, G_FILE_TEST_IS_EXECUTABLE)) {

            	//add_to_list (window->liststore, line, aaa, linenum+1, start_pos, end_pos, filename);

                g_print ("%d [%d-%d] \t::%s" , linenum+1, start_pos, end_pos, line);

            }

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
