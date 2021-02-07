/* -*- Mode:C; Coding:us-ascii-unix; fill-column:132 -*- */
/**********************************************************************************************************************************/
/**
   @file      pcre_example.c
   @author    Mitch Richling <https://www.mitchr.me/>
   @Copyright Copyright 1994,1998 by Mitch Richling.  All rights reserved.
   @brief     UNIX regex tools@EOL
   @Keywords  UNIX regular expressions regex perl pcre
   @Std       ISOC POSIX.2 (IEEE Std 103.2) BSD4.3 
   @Tested    
              - Solaris 2.8
              - MacOS X.2
              - Linux (RH 7.3)

   This is an example program intended to illustrate very basic use of the PCRE regular expression library.  PCRE is NOT part of any
   UNIX or language standard; however, it is commonly found on UNIX systems today, and it has a compatibility mode that supports the
   standard interfaces.

   The vast irregularities found in various UNIX favors with regard to regular expression support can make using regular expressions
   frustrating.  It can be less difficult, and safer, to simply carry around a regular expression library with you!  PCRE is by far
   the most popular, "alternate", regular expression library available today.  It makes a fine choice for the budding UNIX
   programmer unwilling to explore the vulgarities of some operating system vendor's regular expression library.  If you are a C++
   programmer, another good alternative may be found as part of the BOOST library.

   Note: This program is very similar to the regex_example.c example found in this directory.
***********************************************************************************************************************************/

#include <pcre.h>               /* PCRE lib        NONE  */
#include <stdio.h>              /* I/O lib         C89   */
#include <stdlib.h>             /* Standard Lib    C89   */
#include <string.h>             /* Strings         C89   */

#define SUBSTR_VEC_LEN 30
#define MAXCHAR 1000

// https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
char* read_file (const char* filename) {
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	char *string = malloc(fsize + 1);
	fread(string, 1, fsize, f);
	fclose(f);

	string[fsize] = 0;

	return string;
}

int read_by_line (const char* filename) {
    FILE *fp;
    char str[MAXCHAR];
    int line=0, col=0;

    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s", filename);
        return 1;
    }
    while (fgets(str, MAXCHAR, fp) != NULL) {
        printf("[%d, %d] %s", line, col, str);
        line++;
    }
    fclose(fp);
    return 0;
}

/**********************************************************************************************************************************/
// int find_substr (pcre *reCompiled, pcre_extra *pcreExtra, char *testStrings) {

// }

char buf[MAXCHAR];

int find_all_substr_in_line (const char* str, const char* aStrRegex, int line) {

  //int ret = 1; // for first while loop run
  //int sum=0;
  //int i;
  /*
  for (i=0; ret > 0 || i > SUBSTR_VEC_LEN; i++) {
	  ret = find_substr ((char*)testStrings + sum + i);
	  sum += ret;

	  if (ret >= 0) {
	  	printf("%d %d (%d, %d)\n\n", i, ret, sum+i, line);
	  } else {
	  	break;
	  }
  };
  */

	// https://stackoverflow.com/questions/29866616/how-to-find-all-occurrences-of-a-substring-in-c
	char *ptr = strstr(str, aStrRegex);
	while (ptr) {
	    /* ... do something with ptr ... */
	    printf ("%d :: %s ", line, ptr);
	    
	    //if (ptr != NULL) {
	    	size_t len = strlen(aStrRegex);
	    	strncpy(buf, ptr, len);
			buf[len] = '\0';

			printf("%d :: %s ", line, buf);
		//}

	    ptr = strstr(ptr+1, aStrRegex);

	    printf("\n");
	}

  return 0;
}

int find_all_substr_in_file (const char* filename, const char* aStrRegex) {
    FILE *fp;
    char str[MAXCHAR];
    int line=0, col=0;

    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s", filename);
        return 1;
    }

    while (fgets(str, MAXCHAR, fp) != NULL) {
    	// TODO
    	// zrobić kolejną pętlę while - fun find_all_substr_in_line powinna zwracać sukcesywnie nowe wystąpienia znaku w danej linii, a jeśli się zakończy to NULL
    	//find_all_substr_in_line (str, aStrRegex, line);
        //if (i > 0) {
        //	printf("------------------- [%d, %d] %s :: %d\n", line, col, str, i);
        //}

		find_all_substr_in_line (str, aStrRegex, line);

        line++;
    }

    // clean the mess
    fclose(fp);

    return 0;
}

int main (int argc, char *argv[]) {
	//read_by_line ("test_scan.c");
	//return 0;

	find_all_substr_in_file ("test_scan.c", "int main (int argc");

	// We are all done..
	return 0;

} /* end func main */
