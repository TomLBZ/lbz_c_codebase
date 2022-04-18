#include "strings.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void testStr();

int main(){
    //testStr();
    testNetwork();
    return 0;
}

void testStr(){
    char str[] = "Hello \nWorld!";
    char str2[] = "888 999 777 666";
    char* str2_cleaned = strReplace(str2, " ", "");
    size_t npages = 0;
    size_t width = 20;
    size_t height = 2;
    char*** pages = strToPages(str, width, height, &npages);
    char buffer[SPRTF_BUFF_SIZE];
    char* p = buffer;
    int o1,o2;
    o1 = sprintpgs(p, pages, npages, height);
    p += o1;
    o2 = sprintlnsep(p, str2_cleaned, -1);
    p += o2;
    freePages(pages, npages, height);
    sprintf(p, "Memory Freed!\n");
    printf("%s",buffer);
}