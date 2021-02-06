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
int find_substr (pcre *reCompiled, pcre_extra *pcreExtra, char *testStrings) {
  const char *psubStrMatchStr;
  int subStrVec[SUBSTR_VEC_LEN];
  int pcreExecRet;
  int j;

    /* Try to find the regex in aLineToMatch, and report results. */
    pcreExecRet = pcre_exec(reCompiled,
                            pcreExtra,
                            testStrings, 
                            strlen(testStrings),    // length of string
                            0,                      // Start looking at this point
                            0,                      // OPTIONS
                            subStrVec,
                            SUBSTR_VEC_LEN);        // Length of subStrVec

    /* pcre_exec OPTIONS (||'ed together) can be:
       PCRE_ANCHORED -- can be turned on at this time.
       PCRE_NOTBOL
       PCRE_NOTEOL
       PCRE_NOTEMPTY */

    // Report what happened in the pcre_exec call..
    //printf("pcre_exec return: %d\n", pcreExecRet);
    if(pcreExecRet < 0) { // Something bad happened..
    	/*
      switch(pcreExecRet) {
      case PCRE_ERROR_NOMATCH      : printf("String did not match the pattern\n");        break;
      case PCRE_ERROR_NULL         : printf("Something was null\n");                      break;
      case PCRE_ERROR_BADOPTION    : printf("A bad option was passed\n");                 break;
      case PCRE_ERROR_BADMAGIC     : printf("Magic number bad (compiled re corrupt?)\n"); break;
      case PCRE_ERROR_UNKNOWN_NODE : printf("Something kooky in the compiled re\n");      break;
      case PCRE_ERROR_NOMEMORY     : printf("Ran out of memory\n");                       break;
      default                      : printf("Unknown error\n");                           break;
      }*/ 
      /* end switch */
      return pcreExecRet;
    } else {
      printf("Result: We have a match!\n");
        
      // At this point, rc contains the number of substring matches found...
      if(pcreExecRet == 0) {
        printf("But too many substrings were found to fit in subStrVec!\n");
        // Set rc to the max number of substring matches possible.
        pcreExecRet = SUBSTR_VEC_LEN / 3;
      } /* end if */

      // PCRE contains a handy function to do the above for you:
      for(j=0; j<pcreExecRet; j++) {
        pcre_get_substring(testStrings, subStrVec, pcreExecRet, j, &(psubStrMatchStr));
        printf("Match(%2d/%2d): (%2d,%2d): '%s'\n", j, pcreExecRet-1, subStrVec[j*2], subStrVec[j*2+1], psubStrMatchStr);
      } /* end for */
      
      // Free up the substring
      pcre_free_substring(psubStrMatchStr);
    }  /* end if/else */
    printf("\n");

    return subStrVec[0];
}

int find_all_substr_in_line (const char* testStrings, const char* aStrRegex) {

  pcre *reCompiled;
  pcre_extra *pcreExtra;

  const char *pcreErrorStr;
  int pcreErrorOffset;
  //char *aStrRegex;
  //char **aLineToMatch;

	//char* testStrings = read_file (filename);
/*
  char testStrings[] = { 
"This should match... hello This could match... hello! More than one hello.. [int main] hello No chance of a match... [int main] fewuifnuinweerrfnweeunwe[int main]riuewnriwefimweoinwef [int main] wrerwer [int main] wrnwuenru ewrin oinewr [int main] " 
	};
*/
  //aStrRegex = "(.*)(hello)+";  
  //aStrRegex = "int main";  
  
  //printf("Regex to use: %s\n", aStrRegex);

  // First, the regex string must be compiled.
  reCompiled = pcre_compile(aStrRegex, 0, &pcreErrorStr, &pcreErrorOffset, NULL);

  /* OPTIONS (second argument) (||'ed together) can be:
       PCRE_ANCHORED       -- Like adding ^ at start of pattern.
       PCRE_CASELESS       -- Like m//i
       PCRE_DOLLAR_ENDONLY -- Make $ match end of string regardless of \n's
                              No Perl equivalent.
       PCRE_DOTALL         -- Makes . match newlins too.  Like m//s
       PCRE_EXTENDED       -- Like m//x
       PCRE_EXTRA          -- 
       PCRE_MULTILINE      -- Like m//m
       PCRE_UNGREEDY       -- Set quantifiers to be ungreedy.  Individual quantifiers
                              may be set to be greedy if they are followed by "?".
       PCRE_UTF8           -- Work with UTF8 strings.
  */

  // pcre_compile returns NULL on error, and sets pcreErrorOffset & pcreErrorStr
  if(reCompiled == NULL) {
    printf("ERROR: Could not compile '%s': %s\n", aStrRegex, pcreErrorStr);
    exit(1);
  } /* end if */

  // Optimize the regex
  pcreExtra = pcre_study(reCompiled, 0, &pcreErrorStr);

  /* pcre_study() returns NULL for both errors and when it can not optimize the regex.  The last argument is how one checks for
     errors (it is NULL if everything works, and points to an error string otherwise. */
  if(pcreErrorStr != NULL) {
    printf("ERROR: Could not study '%s': %s\n", aStrRegex, pcreErrorStr);
    exit(1);
  } /* end if */

  int ret = 1; // for first while loop run
  int sum=0;
  int i;

  for (i=0; ret > 0 || i > SUBSTR_VEC_LEN; i++) {
	  ret = find_substr (reCompiled, pcreExtra, (char*)testStrings + sum + i);
	  sum += ret;

	  if (ret > 0) {
	  	printf("%d %d %d\n", i, ret, sum+i);
	  } else {
	  	break;
	  }
  };

  // Free up the regular expression.
  pcre_free(reCompiled);

  // Free up the EXTRA PCRE value (may be NULL at this point)
  if(pcreExtra != NULL) {
#ifdef PCRE_CONFIG_JIT
    pcre_free_study(pcreExtra);
#else
    pcre_free(pcreExtra);
#endif
  }

  return i;
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
    	int i = find_all_substr_in_line (str, aStrRegex);
        if (i > 0) {
        	printf("------------------- [%d, %d] %s :: %d\n", line, col, str, i);
        }
        line++;
    }
    fclose(fp);
    return 0;
}

int main (int argc, char *argv[]) {
	//read_by_line ("test_scan.c");
	//return 0;

	find_all_substr_in_file ("test_scan.c", "int main");

	// We are all done..
	return 0;

} /* end func main */
