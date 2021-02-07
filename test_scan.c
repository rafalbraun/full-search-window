#include <stdio.h>              /* I/O lib         C89   */
#include <stdlib.h>             /* Standard Lib    C89   */
#include <string.h>             /* Strings         C89   */

#define SUBSTR_VEC_LEN 30
#define MAXCHAR 1000

char buf[MAXCHAR];
char str[MAXCHAR];

// https://stackoverflow.com/questions/29866616/how-to-find-all-occurrences-of-a-substring-in-c
int find_all_substr_in_line (const char* str, const char* aStrRegex, int line, const char* filename) {

	char *ptr = strstr(str, aStrRegex);
	
	while (ptr) {

	    printf ("[%s] %d :: %s ", filename, line, ptr);
	    
    	size_t len = strlen(aStrRegex);
    	strncpy(buf, ptr, len);
		buf[len] = '\0';

		printf("%d :: %s ", line, buf);

	    ptr = strstr(ptr+1, aStrRegex);

	    printf("\n");
	}

  	return 0;
}

int find_all_substr_in_file (const char* filename, const char* aStrRegex) {
    FILE *fp;
    int line=0, col=0;

    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s", filename);
        return 1;
    }

    while (fgets(str, MAXCHAR, fp) != NULL) {
		find_all_substr_in_line (str, aStrRegex, line, filename);
        line++;
    }
    fclose(fp);

    return 0;
}

int find_all_substr_in_all_files (const char** filenamesArr, int filenamesArrSize, const char* aStrRegex) {
	for (int i=0; i<filenamesArrSize; i++) {
		find_all_substr_in_file (filenamesArr[i], aStrRegex);
	}
}

int main (int argc, char *argv[]) {
	//find_all_substr_in_file ("test_scan.c", "int main (int argc");
	
	const char* filenamesArr[] = {"test_scan.c", "main.c", "test_treeview.c", "test_scan_regex.c"};
	find_all_substr_in_all_files (filenamesArr, 4, "int main");

	return 0;
} /* end func main */
