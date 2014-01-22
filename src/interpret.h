/*
    Интерпритация входного потока реал-на средствами С
    для отладки 
*/
#ifndef INTERPRET_H
#define INTERPRET_H

#include <stdio.h>
#include "voc.h"

struct Ans {
    char* word_name;
    int word_len;
    int word_state;
    int *cfa;
};

extern void readWords();
extern _Bool findVoc(int, int, struct Ans*);
extern void execWrd(char*, int*);
extern void compileWrd(char*, int*);
extern _Bool checkWrd(int, int, struct Ans*);

void interpret() {
    while(1) {
        printf("> ");
        if(fgets(TIB, NTIB, stdin) != NULL) {
            readWords();
        } else {
            printf("ERROR\n");
        }
    }
}
// Читает слово в входном потоке-> ищет в словаре еслли находит исполняет если не находит
// проверяет число ли это если да то кладет на стек и переходит к чтению след слова
// иначе кидает ошибку
void readWords() {
    int lenWord = 0;
    int startWord = 0;
    struct Ans *result = (struct Ans*)malloc(sizeof(result));
    for(int i = 0; i < NTIB; i++) {
        if( *(TIB + i) == '\n') {
            if( lenWord > 0) {
                if(*(TIB + i) != ' ') 
                    if( checkWrd(startWord, lenWord, result) == 0) break;
                //findVoc(startWord, lenWord, result);
            }
            break;
        }
        if(*(TIB + i) == ' ') {
            if( lenWord != 0) {
                if(checkWrd(startWord, lenWord, result) == 0) break;
                //findVoc(startWord, lenWord, result);
                printf("\n");
            }
            startWord = i + 1;
            lenWord = 0;
        } else lenWord++;
    }
}

_Bool checkWrd(int startWord, int lenWord, struct Ans *result) {
    _Bool find = findVoc(startWord, lenWord, result);
    if(find == 0) {
        printf("%s\n", "Поверка число ли это");
        printf("слово не найдено\n");
        return 0;
    } else {
        printf("OK\n");
        return 1;
    }
}
// 0 - не найдено в словаре
// 1 - найдено
_Bool findVoc(int startWord, int lenWord, struct Ans *result) {
    _Bool ans = 0;
    int endWord = startWord + lenWord;
    char *word = (char *)malloc(lenWord);
    int i = 0;
// read Word
    for( startWord; startWord < endWord; startWord++) {
        *(word + i) = *(TIB + startWord);
        i++;
    }
//    printf("search[%s]\n", word);
// find Word in Dictionary
//    printf("Nfa = %d\n", Nfa);
    int *ptr = (int *)Nfa;
    int *ptrBuf = 0;
//    printf("*Nfa = %d\n", *ptr);
    _Bool flgFind = 0;
    int nNameWrd = 0;
    while(ptr != 0) {
        nNameWrd = *ptr - 80;
        // Если длины слов совпадают то проверяем посимвольно 
        // иначе к след слову
        if(lenWord == nNameWrd) {
            int ii;
            ptrBuf = ptr;
            for( i = 1; i < nNameWrd + 1; i++) {
                ptrBuf++;
                //printf("%c", *(ptrBuf));
                ii = i - 1;
                if(*(word + ii) != (char)*(ptrBuf)) break;
                else {
                    // Если был проверн последние символы и они совпали 
                    // то слова одинаковые
                    if( i == nNameWrd) {
                        flgFind = 1;
                    }
                }
            }
            if (flgFind == 1) break;
        }
        ptr += nNameWrd + 1; // перескакиваем поле с именем
        ptr++; // перескакиваем поле с флагом
        ptr = (int*)(*ptr); // берем поле c NFA след слова
    }
    if (flgFind == 1) {
//        printf("[%s]find in voc\n", word);
        int *cfa  = (ptr + nNameWrd + 3);
        // printf("cfa = %d\n", cfa);
        // printf("*cfa = %d\n", *cfa);
        int *pfa = (int *) *cfa;
        // printf("*pfa = %d\n", *pfa);
        // printf("test() = %d\n", test);
        if( STATE == 0) execWrd(word, cfa);
        else if (STATE == 1) compileWrd(word, cfa);
        ans = 1;
    } else {
//        printf("[%s] doesn't find in voc\n", word);
        ans = 0;
    }
    return ans;
}
// исполняем слово простое
void execWrd(char *name, int *cfa) {
    //printf("%s -> execute\n", name);
    //printf("execWrd %s :cfa = %d/n", name, cfa);
    //WPtr = cfa;
   // Ptr = (int*)*WPtr;
    Ptr = cfa;
    //printf("Ptr = %d\n", Ptr);
    Ptr2 = (int *)*Ptr;
    //printf("Ptr2 = %d\n", Ptr2);
    Call = (void(*)())*Ptr2;
    //printf("Call = %d\n", Call);
    Call();
}
// компилируем слово
void compileWrd(char *name, int*cfa) {
        printf("compileWrd %s :cfa = %d/n", name, cfa);
}
#endif
