// gcc test.c -lm -g -Wall -o test
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define FUNC_ID_CHARBUFLEN  14

typedef void (*fptr)(void* input, void* output);

typedef struct funclist_t{
    size_t len;
    size_t* ids;
    fptr* funcs;
} FuncList;

FuncList createFuncList(){
    FuncList fl = {
        .len = 0,
        .ids = malloc(sizeof(size_t)),
        .funcs = malloc(sizeof(fptr))
    };
    return(fl);
}

void destroyFuncList(FuncList* fl){
    fl->len = 0;
    free(fl->ids);
    free(fl->funcs);
    free(fl);
}

void addFuncToList(FuncList* fl, fptr f, size_t id){
    if(NULL == f || NULL == fl) return;
    fl->ids = realloc(fl->ids, sizeof(size_t) * (fl->len + 1));
    fl->funcs = realloc(fl->funcs, sizeof(fptr) * (fl->len + 1));
    fl->funcs[fl->len] = f;
    fl->ids[fl->len] = id;
    fl->len++;
}

void removeFuncByIndex(FuncList* fl, size_t index){
    if(NULL == fl) return;
    if(index >= fl->len || index < 0) return;
    for (size_t i = index; i < fl->len - 1; i++)
    {
        fl->ids[i] = fl->ids[i+1];
        fl->funcs[i] = fl->funcs[i+1];
    }
    fl->len--;
    fl->ids[fl->len] = 0;
    fl->funcs[fl->len] = NULL;
}

void removeFuncById(FuncList* fl, size_t id){
    if(NULL == fl) return;
    size_t i = 0;
    for (;i < fl->len; i++) if(id == fl->ids[i]) break;
    removeFuncByIndex(fl, i);
}

void executeAtIndex(FuncList* fl, size_t index, void* input, void* output){
    if(NULL == fl || index >= fl->len) return;
    fl->funcs[index](input, output);
}

void executeById(FuncList* fl, size_t id, void* input, void* output){
    if(NULL == fl) return;
    size_t i = 0;
    for (;i < fl->len; i++) if(id == fl->ids[i]) break;
    if(i == fl->len) return;
    fl->funcs[i](input, output);
}

void executeAtIndices(FuncList* fl, size_t* indices, void** inputs, void** outputs, size_t len){
    if(NULL == fl || NULL == indices) return;
    for (size_t i = 0; i < len; i++) fl->funcs[indices[i]](inputs[i], outputs[i]);
}

void executeByIds(FuncList* fl, size_t* ids, void** inputs, void** outputs, size_t len){
    if(NULL == fl || NULL == ids) return;
    size_t done[len];
    for (size_t j = 0; j < len; j++) done[j] = 0; // initialize to 0
    for (size_t i = 0; i < len; i++){ // for each function in list
        for (size_t j = 0; j < fl->len; j++){ // check for each id
            if(fl->ids[i] == ids[j] && !done[j]){ // ith function matches jth id
                fl->funcs[i](inputs[j], outputs[j]);
                done[j] = 1; // mark as done
            }
        }
    }
}

void executeAll(FuncList* fl, void** inputs, void** outputs){
    for (size_t i = 0; i < fl->len; i++) fl->funcs[i](inputs[i], outputs[i]);
}

void repeatExecAtIndices(FuncList* fl, size_t* indices, void* input, void** outputs, size_t len){
    if(NULL == fl || NULL == indices) return;
    for (size_t i = 0; i < len; i++) fl->funcs[indices[i]](input, outputs[i]);
}

void repeatExecByIds(FuncList* fl, size_t* ids, void* input, void** outputs, size_t len){
    if(NULL == fl || NULL == ids) return;
    size_t done[len];
    for (size_t j = 0; j < len; j++) done[j] = 0; // initialize to 0
    for (size_t i = 0; i < len; i++){ // for each function in list
        for (size_t j = 0; j < fl->len; j++){ // check for each id
            if(fl->ids[i] == ids[j] && !done[j]){ // ith function matches jth id
                fl->funcs[i](input, outputs[j]);
                done[j] = 1; // mark as done
            }
        }
    }
}

void repeatExecAll(FuncList* fl, void* input, void** outputs){
    for (size_t i = 0; i < fl->len; i++) fl->funcs[i](input, outputs[i]);
}

size_t lowerLettersToFuncId(char* letters, size_t len){
    if(len <= 0 || len > 13) return 0;
    size_t res = 0;
    for (size_t i = 0; i < len; i++) res += (size_t)pow(26, i) * (letters[i] - 96);
    return res;
}

void funcIdToLowerLetters(size_t id, char* out){
    size_t metric = id;
    size_t divided;
    for (size_t i = 0; i < FUNC_ID_CHARBUFLEN - 1; i++)
    {
        if(metric > 26){
            divided = metric / 26;
            out[i] = (char)(metric - divided * 26 + 96);
            metric = divided;
        }else{
            out[i] = (char)(metric + 96);
            out[i + 1] = '\0';
            break;
        }
    }
}

void printFuncList(FuncList* fl){
    printf("Function List [%lu]{\n", fl->len);
    char name[FUNC_ID_CHARBUFLEN]; char* pname = name;
    for (size_t i = 0; i < fl->len; i++){
        funcIdToLowerLetters(fl->ids[i], pname);
        printf("func[%lu]: id = %lu, name = %s, ptr = {%lu}\n",
            i, fl->ids[i], pname, (size_t)(fl->funcs[i]));
    }
    printf("}\n");
}

typedef struct inStruct_t{
    int a;
    int b;
} InStruct;

typedef struct resStruct_t{
    int res;
} ResStruct;

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
    InStruct input1 = {.a = 6, .b = 3};
    InStruct input2 = {.a = 55, .b = 11};
    InStruct input3 = {.a = 90, .b = 9};
    ResStruct output1 = {.res = 0};
    ResStruct output2 = {.res = 0};
    ResStruct output3 = {.res = 0};
    InStruct inputs[3] = {input1, input2, input3};
    ResStruct outputs[3] = {output1, output2, output3};
    void* pinputs = inputs;
    void* poutputs = outputs;
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
    executeAtIndex(&fl, 0, &input1, &output1);
    printf("done. now executing at index 0, should be \"mult\"...\nres = %d\n", output1.res);
    executeById(&fl, lowerLettersToFuncId("div", 3), &input2, &output2);
    printf("done. now executing by id \"div\"...\nres = %d\n", output2.res);
    printf("done. now executing all...\n");
    executeAll(&fl, &pinputs, &poutputs);
    for(size_t i = 0; i < 3; i++) printf("outputs[%lu] = %d\n", i, outputs[i].res);
    printf("done. now executing by ids \"add\" then \"div\"...\n");
    size_t ids[2];
    ids[0] = 20384; ids[1] = lowerLettersToFuncId("div", 3);
    executeByIds(&fl, ids, &pinputs, &poutputs, 2);
    for(size_t i = 0; i < 3; i++) printf("outputs[%lu] = %d\n", i, outputs[i].res);
    printf("done. now repeat executing by indices 1 and 2, using input 6 and 3, should be \"div\" then \"add\"...\n");
    ids[0] = 0; ids[1] = 1;
    repeatExecAtIndices(&fl, ids, &pinputs, &poutputs, 2);
    for(size_t i = 0; i < 3; i++) printf("outputs[%lu] = %d\n", i, outputs[i].res);
    printf("Congrats, all tests passed!\n");
}