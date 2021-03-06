#ifndef FORTH_H
#define FORTH_H

#include "voc.h"

// ====================================================
// NEXT WORD
void nextWRD() {
    WPtr++;
}
// ====================================================
// Переносит значение с вершины стека данных на стек возвратов
// >R
void onR() {
    printf("%s\n",">R run");
    int data = popSD();
    pushSR(data);
    nextWRD();
}
int* crtOnR() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = onR;
    int flag_exec = 0;
    int *cfa = crtWord(2, ">R", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Переносит значение со стека возвратов на стек данных
// R>
void fromR() {
    printf("%s\n","R> run");
    int data = popSR();
    pushSD(data);
    nextWRD();
}
int* crtFromR() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = fromR;
    int flag_exec = 0;
    int *cfa = crtWord(2, "R>", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Копирует значение с вершины стека возвратов на стек данных
// R@
void RCopy() {
    printf("%s\n","R@ run");
    int data = popSR();
    pushSR(data);
    pushSD(data);
    nextWRD();
}
int* crtRcopy() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = RCopy;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(2, "R@", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// разыменовать значение на стеке данных
// @
void _From() {
    printf("%s\n", "@ run");
    int *data = (int *)popSD();
    pushSD((int)*data);
    nextWRD();
}
int* crt_From() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = _From;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(1, "@", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Берется число после слова LIT и кладется на стек
// данных а указателю интерпритации увеличивается чтобы он указвал на след 
// слово после числа
// LIT
void Lit() {
    printf("LIT run\n");
    WPtr++;
    int num = (int) *WPtr;
    pushSD(num);
    WPtr++;
}
int* crtLIT() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Lit;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(3, "LIT", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// устанавливает указ интерпритации на адрес указан сразу за словом 
// после слова Branch берем число прибавляем к текущему адресу
// интерпритации
// BRANCH
void Branch() {
    printf("BRANCH run\n");
    WPtr++;
    int num = (int)*WPtr;
    WPtr = WPtr + num;
    WPtr++;
}
int* crtBRANCH() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Branch;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(6, "Branch", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Если ложь(0) работаем как BRANCH 
// если правда то начинаем исполнение слова сразу за числом
// ?BRANCH
void OBranch() {
    printf("?BRANCH run\n");
    int flag = popSD();
    if (flag == 0) { // ЛОЖЬ
        WPtr++;
        int num = (int)*WPtr;
        WPtr = WPtr + num;
        WPtr++;
    } else {         // ПРАВДА
        WPtr = WPtr + 2;
    }
}
int* crtOBRANCH() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = OBranch;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(7, "?BRANCH", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// HERE
// возвращает вершину словаря
void Here() {
    printf("%s\n","HERE run");
    pushSD((int)PtrVoc);
    nextWRD();
}
int* crtHERE() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Here;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(4, "HERE", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// ALLOT
// резервирует n интов в словаре. Кол-во байт снимает 
// со стека
void Allot() {
    printf("%s\n", "ALLOT run");
    int n = popSD();
    PtrVoc = PtrVoc + n;
    nextWRD();
}
int* crtALLOT() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Allot;
        // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(5, "ALLOT", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// , - запятая (NO DEBUG, NO TEST)
// компилирует значение со стека в словарь после чего вершина словаря
// продвигается на 1
void Comma() {
    printf("%s\n", "COMMA run");
    int data = popSD();
    *PtrVoc = data;
    PtrVoc++;
    nextWRD();
}
int* crtCOMMA() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Comma;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(1, ",", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// COMPILE
// копилирует на вершину словаря значение наход сразу за словом
void Compile() {
    printf("%s\n", "COMPILE run");
    WPtr++;
    int data =(int) *WPtr;
    *PtrVoc = data;
    PtrVoc++;
    WPtr++;
}
int* crtCOMPILE() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Compile;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(7, "COMPILE", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Дублирует значение на стеке данных
void Dup() {
    //printf("DUP run\n");
    int value = popSD();
    pushSD(value);
    pushSD(value);
}
int* crtDUP() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Dup;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(3, "DUP", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// выкинуть значение со стека данных
// A ->
void Drop() {
    popSD();
}
int* crtDROP() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Drop;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(4, "DROP", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// A,B -> A,B,A
void Over() {
    int value = *(SD - 2);
    pushSD(value);
}
int* crtOVER() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Over;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(4, "OVER", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// A,B,C -> B,C,A
void Rot() {
    int c = popSD();
    int b = popSD();
    int a = popSD();
    pushSD(b);
    pushSD(c);
    pushSD(a);
}
int* crtROT() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Rot;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(3, "ROT", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// A,B -> B,A
void Swap() {
    int b = popSD();
    int a = popSD();
    pushSD(b);
    pushSD(a);
}
int* crtSWAP() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Swap;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(4, "SWAP", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// "." - снимаем значение со стека и печатает на терминал
void Dot() {
    int val = popSD();
    printf("%d\n", val);
}
int* crtDOT() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Dot;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(1, ".", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// PLUS + A,B -> C
void Plus() {
  int a = popSD();
  int b = popSD();
  pushSD(a + b);
}
int* crtPLUS() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Plus;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(1, "+", flag_exec, 0, n_pfa, pfa);
    return cfa;
  
}
// ====================================================
// Minus - A,B -> C
void Minus() {
  int a = popSD();
  int b = popSD();
  pushSD(a - b);
}
int* crtMINUS() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Minus;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(1, "-", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Multi * A,B -> C
void Multi() {
  int a = popSD();
  int b = popSD();
  pushSD(a * b);
}
int* crtMULTI() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Multi;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(1, "*", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Division / A,B -> C
void Division() {
  int a = popSD();
  int b = popSD();
  pushSD(a / b);
}
int* crtDIVISION() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Division;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(1, "/", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// Division MOD A,B -> C
void Mod() {
  int a = popSD();
  int b = popSD();
  pushSD(a % b);
}
int* crtMOD() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = Mod;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(3, "MOD", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// ====================================================
// 1+ -> : LIT 1 + ;
//void OnePlus() {
    
//}
//int* crtCONSTANT() {
//    
//}
// ====================================================
// S:
void s_() {
    printf("%s\n","S: run");
    pushSR((int)(WPtr + 1));
    WPtr = Ptr + 1;
}
int* crtS_() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = s_;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(2, "S:", flag_exec, (int*)s_, 0, 0);
    return cfa;
}
// ====================================================
// S;
void _s() {
    printf("%s\n","S; run");
//    Next = (int *)popSR();
    int data = popSR();
    WPtr = (int *)data;
}
int *crt_S() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = _s;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(2, "S;", flag_exec, 0, n_pfa, pfa);
    return cfa;
}

// ====================================================
// Слово с которого происходит раскрутка Forth машины
// START
void exitForth() {
    StopForth = 1; // признак завершения работы Forth
}
int* crtStart(int* w1, int* w2, int* w3) {
    int n_pfa = 2;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = (void(*)())w2;
    *(pfa + 1) = (void(*)())w3;
//    *(pfa + 2) = (void(*)())w3;
        // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(5, "START", flag_exec, (int*)w1, n_pfa, pfa);
    return cfa;
}
// ====================================================

#endif

