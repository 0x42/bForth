#ifndef VOC_H
#define VOC_H
#include <stdlib.h>
//#define DEBUG_PRINT
//===============================================
// Входной поток
int NTIB = 100; // #TIB
char *TIB;      // 100 байт
//===============================================
// 0 - режим интерпритации
// 1 - режим компиляции
int STATE = 0; 
int* WPtr = 0;
int* Ptr = 0;
int* Ptr2 = 0;
void (*Call)() = 0;
int StopForth = 0; // остановка адрес интерп

//===============================================
// Vocabular 
int *PtrVoc = 0;
int Nfa = 0;
int* crtVoc() {
    PtrVoc = (int *)malloc(131072*sizeof(int)); // 512 КБайт
    return PtrVoc;
}
//================================================
//     Создание слова
//================================================
int* crtWord(int _n, char *_name, int flag, int* _cfa, int _n_pfa, void(**_pfa)() ) {
    static int first_word = 1;
//printf("PtrVoc = %d\n", PtrVoc);
    *PtrVoc = 80 + _n;
    int nfa =(int)PtrVoc;
    PtrVoc++;
//printf("name -> ");
    for(int i = 0; i < _n ; i++ ) {
        *PtrVoc = _name[i];
//printf("%c", _name[i]);
        PtrVoc++;
    }
//printf("\n");
// записываем флаг 1 - IMMEDIATE(немедленное исполнение) 
//                 0 - обычное слово
    *PtrVoc = flag;
    PtrVoc++; 
    if(first_word == 1) {
        first_word = 0;
        *PtrVoc = 0;
    } else {
        *PtrVoc = Nfa;
    }

    PtrVoc++;
    Nfa = nfa;
    if(_cfa == 0) {
        *PtrVoc = (int)(PtrVoc + 1);
       // printf("%d\n",);
    } else {
        *PtrVoc = (int)_cfa;
    }
    int* cfa = PtrVoc;
    PtrVoc++;
#ifdef DEBUG_PRINT
    printf("%s\n", _name);
    printf("cfa = %d\n", cfa);
    printf("*cfa = %d\n", *cfa);
#endif
    for(int i = 0; i < _n_pfa; i++) {
        *PtrVoc = (int)(*(_pfa + i));
#ifdef DEBUG_PRINT
        printf("pfa = %d\n", PtrVoc);
        printf("*pfa = %d\n", *PtrVoc);
#endif
        PtrVoc++;
    }
//    printf("======================================\n");
    return cfa;
}
//================================================
// Стек возвратов
// int имеет такой же размер как и указатель на 
// функцию, или структуру
// поэтому на стеке можно хранить числа адреса
//================================================
int *SP = 0; // указатель на стек возватов
// Инициализируем СТЕК !!!
void srInit() {
    SP = (int *)malloc(131072*sizeof(int));
}

void pushSR(int data) {
    *SP = data;
    SP++;
}

int popSR() {
    SP--;
    int data = *SP;
    return data;
}
//================================================
// Стек данных
//================================================
int *SD = 0; // указатель на стек данных
// Инициализируем СТЕК !!!
void sdInit() {
    SD = (int *)malloc(262144*sizeof(int)); // 1 MByte
}

void pushSD(int data) {
    *SD = data;
    SD++;
}

int popSD() {
    SD--;
    int data = *SD;
    return data;
}
//================================================
#endif
