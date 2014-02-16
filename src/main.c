#include <stdio.h>
#include <string.h>
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
    int *cfaDUP = crtDUP(); 	// DUP
    int *cfaDrop = crtDROP(); 	// DROP
    int *cfaOver = crtOVER();	// OVER
    int *cfaRot  = crtROT(); 	// ROT
    int *cfaSwap = crtSWAP();	// SWAP
    int *cfaDot  = crtDOT(); 	// DOT
    int *cfaPlus = crtPLUS();   // +
    int *cfaMinus = crtMINUS(); // -
    int *cfaS_   = crtS_();
    int *cfa_S   = crt_S();
    int *cfaTEST = crtTEST();
    int *cfaTEST1 = crtTEST1();
    int *cfaTEST2 = crtTEST2();
    //int *cfaGoTest = crtGOTEST(cfaS_, cfaHERE, (int*) cfa_S);
    int *cfaGoTest = crtGOTEST(cfaS_, cfaCOMPILE, (int*)42, (int*) cfa_S);
    //--------------------------------------------------------------
    int *cfaOnePlus = crtONEPLUS(cfaS_, cfaLIT, (int*)1, cfaPlus, cfa_S);
    int *cfaStart = crtStart(cfaS_, cfaGoTest, cfaTEST2);
    // ----- Initialization -----------------
    int *prog = (int*)malloc(2*sizeof(int));
    *prog = (int)cfaStart;
    *(prog + 1) = (int)cfaStart;
    WPtr = prog;
    // --------------------------------------
    printf("====================\n");
    printf("===== FORTH ========\n");
    printf("====================\n");
// ---------------------------------------------------------
// -------- АДРЕСНЫЙ ИНТЕРПРИТАТОР -------------------------
// ---------------------------------------------------------
    int step = 0;
    //pushSD(2);
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
    printf("\n========== FINISH ===========\n");
// для тестирования
//    int data = popSD();
//    printf("data = %d\n", data);
//    printf("*PtrVoc = %d\n", *(PtrVoc-1));
/*
    char *err;
    long nnn = strtol(num, &err, 10);
    if( strlen(err) > 0) {
        printf("не число\n");
    } else {
        printf("число = %d\n", nnn);
    }
*/
    interpret();
    printf("\n=============================\n");
}












