#include "headers/lbzstr.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

char* strtok_n(char* str, char* delim){
    static char *saveline = NULL;
    char *p;
    int n;
    if(str != NULL) saveline = str;
    //if reached the end of the line
    if(saveline == NULL || *saveline == '\0') return(NULL);
    //number of characters that aren't delim
    n = strcspn(saveline, delim);
    p = saveline; //save start of this token
    saveline += n; //go past the delim
    if(*saveline != '\0') *saveline++ = '\0'; // end str
    return(p);
}

char* strReplace(char *str, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    long len_rep;  // length of rep (the string to remove)
    long len_with; // length of with (the string to replace rep with)
    long len_front; // distance between rep and end of last rep
    size_t count;    // number of replacements
    // sanity checks and initialization
    if (!str || !rep) return NULL; // null reference returns null
    len_rep = strlen(rep);
    if (len_rep == 0) return NULL; // empty rep causes infinite loop during count
    if (!with) with = "";   // null with means deletion instead of replacement
    len_with = strlen(with);
    // count the number of replacements needed
    ins = str;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) ins = tmp + len_rep;
    // allocate memory for result string, point tmp to result
    tmp = result = malloc(strlen(str) + (len_with - len_rep) * count + 1);
    if (!result) return NULL; // no memory is allocated, returns null
    // replacement
    while (count--) {
        ins = strstr(str, rep); // points to next rep
        len_front = ins - str; // length of first substring
        tmp = strncpy(tmp, str, len_front) + len_front; // copies first substring and moves tmp to rep
        tmp = strcpy(tmp, with) + len_with; //copies with and moves tmp after with
        str += len_front + len_rep; // move str to next substring
    }
    strcpy(tmp, str); //copies end of str
    return result;
}

char** strToLines(char* str, char* delim, size_t* nlines){
    char** result = 0;
    char* tmp;
    char* ins;// next insertion point
    long len_delim;
    size_t count = 0;
    // sanity checks and initialization
    if (!str || !delim) return NULL; // null reference returns null
    len_delim = strlen(delim);
    // count the number of replacements needed
    ins = str;
    for (count = 0; (tmp = strstr(ins, delim)); ++count) ins = tmp + len_delim;
    // allocate memory for result
    result = malloc(sizeof(char*) * ++count);
    if (!result) return NULL; // no memory is allocated, returns null
    // split loop
    char* token = strtok_n(str, delim);
    for (size_t i = 0; i < count; i++)
    {
        result[i] = !token ? "" : strdup(token);
        token = strtok_n(NULL, delim);
    }
    *nlines = count;
    return result;
}

char** strWrapLine(char* str, size_t width, size_t* nlines){
    // sanity checks and initialization
    if (!str || !width) return NULL; // null reference returns null
    char** result = 0;
    size_t txtlen = strlen(str);
    size_t count = txtlen / width;
    if (count * width != txtlen) count++;
    if(!count) count++;
    char* ins; // insertion point
    // allocate memory
    result = malloc(count * sizeof(char*));
    if (!result) return NULL; // no memory is allocated, returns null
    // wrap loop
    ins = str; // points to start of str
    for (int i = 0; i < count; i++)
    {
        result[i] = malloc((width + 1) * sizeof(char));
        if (!result[i]) return NULL; // no memory is allocated, returns null
        result[i] = strncpy(result[i], ins, width);
        result[i][width] = '\0'; // end of string
        ins += width;
    }
    *nlines = count;
    return result;
}

char** strAppendLines(char** lines1, char** lines2, size_t n1, size_t n2){
    if(!lines1 || !lines2) return NULL;
    if(!n1 && !n2) return NULL;
    if(!n1 && n2) return lines2;
    if(!n2 && n1) return lines1;
    char** result = 0;
    result = malloc((n1+n2) * sizeof(char*));
    for (size_t i = 0; i < n1 + n2; i++) {
        char* tmp = i < n1 ? lines1[i] : lines2[i-n1];
        result[i] = malloc(strlen(tmp) * sizeof(char));
        result[i] = i<n1 ? strcpy(result[i], lines1[i]) : strcpy(result[i], lines2[i-n1]);
    }
    return result;
}

char** strAppendAllLines(char*** linesArr, size_t* sizeArr, size_t n, size_t* nlines){
    if(!linesArr || !sizeArr || !n) return NULL;
    char** result = 0;
    size_t len = 0;
    size_t index = 0;
    for(size_t i = 0; i < n; i++) len += sizeArr[i];
    result = malloc(len * sizeof(char*));
    for (size_t i = 0; i < n; i++)
    {
        if(!linesArr[i] || !sizeArr[i]) {
            result[index] = malloc(sizeof(char));
            result[index] = '\0';
            index++;
        } else{
            for (size_t j = 0; j < sizeArr[i]; j++)
            {
                result[index] = malloc(strlen(linesArr[i][j]) * sizeof(char));
                result[index] = strcpy(result[index], linesArr[i][j]);
                index++;
            }
        }
    }
    *nlines = len;
    return result;
}

char*** strLinesToPages(char** lines, size_t len, size_t width, size_t height, size_t* npages){
    if(!lines) return NULL;
    char*** result = 0;
    size_t index = 0;
    size_t pages = len / height;
    if (pages * height != len) pages++;
    if(!pages) pages++;
    // allocate memory
    result = malloc(pages * sizeof(char**));
    if(!result) return NULL;
    for (size_t i = 0; i < pages; i++)
    {
        result[index] = malloc(sizeof(char*) * height);
        for(size_t j = 0; j < height; j++) result[index][j] = malloc(sizeof(char)*(width+1));
        index++;
    }
    size_t p = 0;
    size_t l = 0;
    for (size_t i = 0; i < len; i++)
    {
        result[p][l] = strncpy(result[p][l], lines[i], width);
        if (result[p][l][strlen(lines[i])] != '\0') result[p][l][strlen(lines[i])] = '\0';
        l++;
        if(l==height){
            p++;
            l=0;
        }
    }
    *npages = pages;
    return result;
}

char*** strToPages(char* str, size_t width, size_t height, size_t* npages){
    if(!str || !width || !height) return NULL;
    size_t nlines = 0;
    size_t index = 0;
    size_t* sizes;
    size_t totallen = 0;
    char*** rawresult = 0;
    char** lines = strToLines(str, "\n", &nlines);
    for (size_t i = 0; i < nlines; i++)
    {
        if(!index) {
            rawresult = (char***)malloc(sizeof(char**));
            sizes = (size_t*)malloc(sizeof(size_t));
        } else {
            rawresult = (char***)realloc(rawresult, sizeof(char**) * (index+1));
            sizes = (size_t*)realloc(sizes, sizeof(size_t) * (index + 1));
        }
        size_t ilines;
        rawresult[index] = strWrapLine(lines[i], width, &ilines);
        sizes[index] = ilines;
        index++;
    }
    char** combined = strAppendAllLines(rawresult, sizes, nlines, &totallen);
    size_t pagecount;
    char*** pages = strLinesToPages(combined, totallen, width, height, &pagecount);
    // lines has only one layer to free
    free(lines);
    // rawrested is nested allocated
    for(size_t i = 0; i < nlines; i++) freeNestedLines(rawresult[i], sizes[i]);
    free(sizes);
    free(rawresult);
    freeNestedLines(combined, totallen);
    *npages = pagecount;
    return pages;
}

void freeStr(char* str){
    free(str);
}

void freeNestedLines(char** lines, size_t len){
    if(!lines) return;
    for (size_t i = 0; i < len; i++) if(lines[i]) free(lines[i]);
    free(lines);
}

void freePages(char*** pages, size_t npages, size_t height){
    if(!pages) return;
    for (size_t i = 0; i < npages; i++) if(pages[i]) freeNestedLines(pages[i], height);
    free(pages);
}

void println(char* str){
    if (!str) return;
    printf("%s\n", str);
}

void printlns(char** lines, size_t len){
    if(!lines) return;
    for (size_t i = 0; i < len; i++) println(lines[i]);
    printsep(0);
}

void printpgs(char*** pages, size_t npages, size_t height){
    if(!pages) return;
    for (size_t i = 0; i < npages; i++) printlns(pages[i], height);
    printsep(-1);
}

void printsep(int len){
    if(len < 0) len = DEFAULT_LEN;
    char str[len + 1];
    for (size_t i = 0; i < len; i++)
        str[i] = '=';
    str[len]='\0';
    println(str);
}

void printlnsep(char* str, int len){
    println(str);
    printsep(len);
}

int sprintln(char* output, char* str){
    if (!str) return 0;
    return sprintf(output, "%s\n", str);
}

int sprintlns(char* output, char** lines, size_t len){
    if(!lines) return 0;
    int off = 0;
    for (size_t i = 0; i < len; i++) {
        int offset = sprintln((char*)(output + off), lines[i]);
        off += offset;
    }
    off += sprintsep((char*)(output + off), 0);
    return off;
}

int sprintpgs(char* output, char*** pages, size_t npages, size_t height){
    if(!pages) return 0;
    int off = 0;
    for (size_t i = 0; i < npages; i++) {
        int offset = sprintlns((char*)(output + off), pages[i], height);
        off += offset;
    }
    off += sprintsep((char*)(output + off), -1);
    return off;
}

int sprintsep(char* output, int len){
    if(len < 0) len = DEFAULT_LEN;
    char str[len + 1];
    for (size_t i = 0; i < len; i++)
        str[i] = '=';
    str[len]='\0';
    return sprintln(output, str);
}

int sprintlnsep(char* output, char* str, int len){
    if (!str) return 0;
    if(len < 0) len = DEFAULT_LEN;
    char sstr[len + 1];
    for (size_t i = 0; i < len; i++)
        sstr[i] = '=';
    sstr[len]='\0';
    return sprintf(output, "%s\n%s\n", str, sstr);
}