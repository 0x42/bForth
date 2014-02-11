#ifndef TESTWRD_H
#define TESTWRD_H
#include "voc.h"
#include "forth.h"

/*
    Тестовые слова для отладки алресного интерпритатора
*/

void test() {
    printf("%s\n","TEST");
    nextWRD();
}
int* crtTEST() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = test;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(4, "TEST", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
void test1() {
    printf("%s\n","TEST1");
    nextWRD();
}
int* crtTEST1() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = test1;
    // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(5, "TEST1", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
void test2() {
    printf("%s\n","TEST2");
    exitForth();
    nextWRD();
}
int* crtTEST2() {
    int n_pfa = 1;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = test2;
        // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(5, "TEST2", flag_exec, 0, n_pfa, pfa);
    return cfa;
}
// Тестовое слово для проверки адресн интерпр
// GOTEST
int* crtGOTEST(int *w1, int *w2, int *w3, int *w4) {
    int n_pfa = 3;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = (void(*)())w2;
    *(pfa + 1) = (void(*)())w3;
    *(pfa + 2) = (void(*)())w4;
        // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(6, "GOTEST", flag_exec, (int *)w1, n_pfa, pfa);
    return cfa;
}
// ====================================================
int* crtONEPLUS(int *w1, int *w2, int *w3, int *w4, int *w5) {
    int n_pfa = 4;
    void (**pfa)() = (void(**)())malloc(n_pfa*sizeof(void(*)()));
    *pfa = (void(*)())w2;
    *(pfa + 1) = (void(*)())w3;
    *(pfa + 2) = (void(*)())w4;
    *(pfa + 3) = (void(*)())w5;
        // 1 - IMMEDIATE 0 -SIMPLE
    int flag_exec = 0;
    int *cfa = crtWord(2, "1+", flag_exec, (int *)w1, n_pfa, pfa);
    return cfa;
}
#endif
