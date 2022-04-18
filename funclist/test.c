#include <stdlib.h>
#include <stdio.h>
#include "funclist.h"

typedef struct inStruct_t{
    int a;
    int b;
} InStruct;

typedef struct resStruct_t{
    int res;
} ResStruct;

void add(void* input, void* output);
void subtract(void* input, void* output);
void multilpy(void* input, void* output);
void divide(void* input, void* output);
void test();

int main(){
    test();
    return 0;
}

void add(void* input, void* output){
    ((ResStruct*)output)->res = ((InStruct*)input)->a + ((InStruct*)input)->b;
}

void subtract(void* input, void* output){
    ((ResStruct*)output)->res = ((InStruct*)input)->a - ((InStruct*)input)->b;
}

void multilpy(void* input, void* output){
    ((ResStruct*)output)->res = ((InStruct*)input)->a * ((InStruct*)input)->b;
}

void divide(void* input, void* output){
    ((ResStruct*)output)->res = ((InStruct*)input)->a / ((InStruct*)input)->b;
}

void test(){
    printf("creating func list...\n");
    InStruct input1 = {.a = 6, .b = 6};
    InStruct input2 = {.a = 55, .b = 11};
    InStruct input3 = {.a = 90, .b = 9};
    ResStruct output1 = {.res = 0};
    ResStruct output2 = {.res = 0};
    ResStruct output3 = {.res = 0};
    InStruct* inputs[3] = {&input1, &input2, &input3};
    ResStruct* outputs[3] = {&output1, &output2, &output3};
    FuncList fl = createFuncList();
    printFuncList(&fl);
    printf("adding funcs to func list\n");
    addFuncToList(&fl, add, lowerLettersToFuncId("add", 3));
    addFuncToList(&fl, subtract, lowerLettersToFuncId("sub", 3));
    addFuncToList(&fl, multilpy, lowerLettersToFuncId("mul", 3));
    addFuncToList(&fl, divide, lowerLettersToFuncId("div", 3));
    printFuncList(&fl);
    printf("All added. Removing \"add\" from func list by 2809...\n");
    removeFuncById(&fl, 2809);
    printFuncList(&fl);
    printf("done. now adding it back...\n");
    addFuncToList(&fl, add, 2809);
    printFuncList(&fl);
    printf("done. now removing sub from list at index 0...\n");
    removeFuncByIndex(&fl, 0);
    printFuncList(&fl);

    printf("done. now executing at index 0, should be \"mult\"...\n");
    executeAtIndex(&fl, 0, &input1, &output1);
    printf("MUL RES = %d\n", output1.res);
    printf("done. now executing by id \"div\"...\n");
    executeById(&fl, lowerLettersToFuncId("div", 3), &input2, &output2);
    printf("DIV RES = %d\n", output2.res);
    for(int i = 0; i < 3; i++) printf("inputs[%d] = (%d, %d)\noutputs[%d] = %d\n", i, inputs[i]->a, inputs[i]->b, i, outputs[i]->res);

    printf("done. now executing all...\n");
    executeAll(&fl, (void**)&inputs, (void**)&outputs);
    for(int i = 0; i < 3; i++) printf("inputs[%d] = (%d, %d)\noutputs[%d] = %d\n", i, inputs[i]->a, inputs[i]->b, i, outputs[i]->res);
    printf("done. now repeat executing all using (6,6)...\n");
    repeatExecAll(&fl, &input1, (void**)&outputs);
    for(int i = 0; i < 3; i++) printf("inputs[%d] = (%d, %d)\noutputs[%d] = %d\n", i, inputs[i]->a, inputs[i]->b, i, outputs[i]->res);

    printf("done. now executing by ids \"add\" then \"div\" using (6,6) and (55,11)...\n");
    size_t ids[2];
    ids[0] = 2809; ids[1] = lowerLettersToFuncId("div", 3);
    executeByIds(&fl, ids, (void**)&inputs, (void**)&outputs, 2);
    for(int i = 0; i < 3; i++) printf("inputs[%d] = (%d, %d)\noutputs[%d] = %d\n", i, inputs[i]->a, inputs[i]->b, i, outputs[i]->res);
    printf("done. now repeat executing by ids \"add\" then \"div\" using (90,9)...\n");
    repeatExecByIds(&fl, ids, &input3, (void**)&outputs, 2);
    for(int i = 0; i < 3; i++) printf("inputs[%d] = (%d, %d)\noutputs[%d] = %d\n", i, inputs[i]->a, inputs[i]->b, i, outputs[i]->res);

    printf("done. now executing by indices 0 and 2 using input (6, 6) and (55, 11), should be \"mul\" then \"add\"...\n");
    ids[0] = 0; ids[1] = 2;
    executeAtIndices(&fl, ids, (void**)&inputs, (void**)&outputs, 2);
    for(int i = 0; i < 3; i++) printf("inputs[%d] = (%d, %d)\noutputs[%d] = %d\n", i, inputs[i]->a, inputs[i]->b, i, outputs[i]->res);
    printf("done. now repeat executing by indices 0 and 1 using input (90, 9), should be \"mul\" then \"div\"...\n");
    ids[0] = 0; ids[1] = 1;
    repeatExecAtIndices(&fl, ids, &input3, (void**)&outputs, 2);
    for(int i = 0; i < 3; i++) printf("inputs[%d] = (%d, %d)\noutputs[%d] = %d\n", i, inputs[i]->a, inputs[i]->b, i, outputs[i]->res);
    printf("Congrats, all tests passed!\n");
}