#include <stdio.h>
#include "voc.h"
#include "forth.h"
#include "testwrd.h"
#include "interpret.h"
//#define DEBUG_PRINT
#define INFO printf("WPtr = %d\n", WPtr); 
#define INFO2 printf("Ptr  = %d\n", Ptr); 
#define INFO3 printf("Ptr2 = %d\n", Ptr2);
#define INFO4 printf("Call = %d\n", Call);
void main() {
    TIB = (char*)malloc(NTIB);
    int *voc = crtVoc(); // Выделяем память для словаря
    srInit();            // Созд стек возвратов
    sdInit();            // Созд стек данных
    int *cfaOnR = crtOnR();         // >R
    int *cfaFromR = crtFromR();     // R>
    int *cfaRCopy = crtRcopy();     // R@
    int *cfa_From = crt_From();     // @
    int *cfaLIT   = crtLIT();       // LIT
    int *cfaBRANCH = crtBRANCH();   // BRANCH
    int *cfaHERE = crtHERE();   // HERE
    int *cfaALLOT = crtALLOT(); // ALLOT
    int *cfaCOMPILE = crtCOMPILE(); // COMPILE
    int *cfaS_   = crtS_();
    int *cfa_S   = crt_S();

    int *cfaTEST = crtTEST();
    int *cfaTEST1 = crtTEST1();
    int *cfaTEST2 = crtTEST2();
    //int *cfaGoTest = crtGOTEST(cfaS_, cfaHERE, (int*) cfa_S);
    int *cfaGoTest = crtGOTEST(cfaS_, cfaCOMPILE, (int*)42, (int*) cfa_S);
    int *cfaStart = crtStart(cfaS_, cfaGoTest, cfaTEST2);
    // ----- Initialization -----------------
    int *prog = (int*)malloc(2*sizeof(int));
    *prog = (int)cfaStart;
    *(prog + 1) = (int)cfaStart;
    WPtr = prog;

    int *num = (int*)malloc(sizeof(int));
    // *num = 101010;
    // printf("num =%d\n", num);
    // printf("*num =%d\n", *num);
    // pushSD((int)num);
    // --------------------------------------
    printf("====================\n");
    printf("===== FORTH ========\n");
    printf("====================\n");
// ---------------------------------------------------------
// -------- АДРЕСНЫЙ ИНТЕРПРИТАТОР -------------------------
// ---------------------------------------------------------
    int step = 0;
    pushSD(2);
    printf("PtrVoc = %d\n", PtrVoc);
    while(StopForth != 1) {
        Ptr = (int*)*WPtr;
        Ptr2 = (int *)*Ptr;
        Call = (void(*)())*Ptr2;
#ifdef DEBUG_PRINT
    INFO INFO2 INFO3 INFO4
#endif
        Call();
#ifdef DEBUG_PRINT
        step++;
        if(step == 5) break;
#endif
    }
// для тестирования
    int data = popSD();
    printf("data = %d\n", data);
    printf("*PtrVoc = %d\n", *(PtrVoc-1));
//    interpret();
    printf("\n=============================\n");
    printf("Nfa = %d\n", Nfa);
    int *ptr = (int *)Nfa;
    printf("*Nfa = %d\n", *ptr);
    
    char bbbb[2];
    while(ptr != 0) {
        int nNameWrd = *ptr - 80;
        printf("nNameWrd = %d\n next?.", nNameWrd);
        fgets(bbbb, 2, stdin);
        for( int i = 1; i < nNameWrd + 1; i++) {
            printf("%c", *(++ptr));
        }
        printf("|\n");
        ptr++;
        ptr = (int*)(*ptr);
//        printf("ptr = %d\n", ptr);
//        fgets(bbbb, 2, stdin);
    }
}












