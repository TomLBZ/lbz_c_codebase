#if !defined(FUNCLIST_MACROS)
#define FUNCLIST_MACROS
    #define FUNC_ID_CHARBUFLEN  14

typedef void (*fptr)(void* input, void* output);

typedef struct funclist_t{
    size_t len;
    size_t* ids;
    fptr* funcs;
} FuncList;

FuncList createFuncList();

void destroyFuncList(FuncList* fl);

void addFuncToList(FuncList* fl, fptr f, size_t id);

void removeFuncByIndex(FuncList* fl, size_t index);

void removeFuncById(FuncList* fl, size_t id);

void executeAtIndex(FuncList* fl, size_t index, void* input, void* output);

void executeById(FuncList* fl, size_t id, void* input, void* output);

void executeAtIndices(FuncList* fl, size_t* indices, void** inputs, void** outputs, size_t len);

void executeByIds(FuncList* fl, size_t* ids, void** inputs, void** outputs, size_t len);

void executeAll(FuncList* fl, void** inputs, void** outputs);

void repeatExecAtIndices(FuncList* fl, size_t* indices, void* input, void** outputs, size_t len);

void repeatExecByIds(FuncList* fl, size_t* ids, void* input, void** outputs, size_t len);

void repeatExecAll(FuncList* fl, void* input, void** outputs);

size_t lowerLettersToFuncId(char* letters, size_t len);

void funcIdToLowerLetters(size_t id, char* out);

void printFuncList(FuncList* fl);

#endif // FUNCLIST_MACROS