#include <string.h>

#if !defined(LBZSTR_MACROS)
#define LBZSTR_MACROS
    
#define DEFAULT_LEN 10

#if !defined(SPRTF_BUFF_SIZE)
    #define SPRTF_BUFF_SIZE 1024
#endif // SPRTF_BUFF_SIZE

/*strtok with null delkimiter support*/
char* strtok_n(char* str, char* delim);
/*replace string occurances in str with another string, result must be freed*/
char* strReplace(char *str, char *rep, char *with);
/*convert string literal to lines, get number of lines, result must be freed*/
char** strToLines(char* str, char* delim, size_t* nlines);
/*convert one line to multiple lines using max length, get number of lines, 
 * result must be freed using freeNestedLines*/
char** strWrapLine(char* str, size_t width, size_t* nlines);
/*append lines together. result must be freed using freeNestedLines*/
char** strAppendLines(char** lines1, char** lines2, size_t n1, size_t n2);
/*append all lines, get line count. result must be freed using freeNestedLines*/
char** strAppendAllLines(char*** linesArr, size_t* sizeArr, size_t n, size_t* nlines);
/*convert lines to pages of fixed width and height*/
char*** strLinesToPages(char** lines, size_t len, size_t width, size_t height, size_t* npages);
/*convert string literal to pages of fixed width and height*/
char*** strToPages(char* str, size_t width, size_t height, size_t* npages);
/*frees string resources*/
void freeStr(char* str);
/*frees lines resources*/
void freeNestedLines(char** lines, size_t len);
/*frees pages resources*/
void freePages(char*** pages, size_t npages, size_t height);
/*print one line*/
void println(char* str);
/*print lines*/
void printlns(char** lines, size_t len);
/*print pages*/
void printpgs(char*** pages, size_t npages, size_t height);
/*print separation line*/
void printsep(int len);
/*print one line with separation*/
void printlnsep(char* str, int len);
/*print one line on a string*/
int sprintln(char* output, char* str);
/*print lines on a string*/
int sprintlns(char* output, char** lines, size_t len);
/*print pages on a string*/
int sprintpgs(char* output, char*** pages, size_t npages, size_t height);
/*print separation line on a string*/
int sprintsep(char* output, int len);
/*print one line with separation on a string*/
int sprintlnsep(char* output, char* str, int len);

#endif // LBZSTR_MACROS