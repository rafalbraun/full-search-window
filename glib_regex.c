#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <glib.h>
#include <glib/gstdio.h>

#include <stdlib.h>
#include <pthread.h>

#define BUFFER  500
#define SIZE    1000

typedef struct _search_data {
    char* dirname;
    char* pattern;
} search_data;

void scan_file(char const* const filename, char const* const pattern) {
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
            g_print ("%s\x1C%d\x1C%d\x1C%d\x1C%s", filename, linenum, start_pos, end_pos, line);

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

int list_directory( char* dirname, char* pattern ) {
    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); 
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    gchar path[SIZE];
    struct dirent *dp;
    DIR *dir = opendir(dirname);

    // Unable to open directory stream
    if (!dir) {
        return 0;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (g_strcmp0(dp->d_name, ".") != 0 && g_strcmp0(dp->d_name, "..") != 0 && (dp->d_name[0]!='.'))
        {
            // Construct new path from our base path
            //g_stpcpy(path, dirname);
            //g_strlcat(path, "/", SIZE);
            //g_strlcat(path, dp->d_name, SIZE);

            snprintf(path, SIZE, "%s/%s", dirname, dp->d_name); // create name of subdirectory

            scan_file(path, pattern);
            list_directory(path, pattern);
        }
    }

    closedir(dir);
}

void *topfun( void *ptr ) {
    char* dirname = ((search_data*)ptr)->dirname;
    char* pattern = ((search_data*)ptr)->pattern;
    list_directory(dirname, pattern);
}

#if !SEARCH_PATH

int main(int argc, char* argv[]) {
    pthread_t thread;
    void* ret; 
    char* dirname = argv[1];
    char* pattern = argv[2];

    search_data* data = g_new0(search_data, 1);;
    data->dirname = dirname;
    data->pattern = pattern;

    if(pthread_create(&thread, NULL, topfun, (void*) data) != 0) {
        printf("pthread create failed\n"); 
        exit(1); 
    }
    if(pthread_join(thread, &ret)<0){
        perror("thread join failed\n");
        exit(1);
    }

    return 0;
}

#endif
