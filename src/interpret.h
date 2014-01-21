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
void interpret() {
    printf("> ");
    if(fgets(TIB, NTIB, stdin) != NULL) {
        readWords();
    } else {
        printf("ERROR\n");
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
                if(*(TIB + i) != ' ') findVoc(startWord, lenWord, result);
            }
            break;
        }
        if(*(TIB + i) == ' ') {
            if( lenWord != 0) {
                findVoc(startWord, lenWord, result);
                printf("\n");
            }
            startWord = i + 1;
            lenWord = 0;
        } else lenWord++;
    }
}

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
        printf("[%s]find in voc\n", word);
        int *cfa  =(int *) *(ptr + nNameWrd + 2);
        printf("ptr = %d\n", cfa);
        printf("flag = %d\n", *(ptr + nNameWrd + 1 ));
        execWrd(word, cfa);
        ans = 1;
    } else {
        printf("[%s] doesn't find in voc\n", word);
        ans = 0;
    }
    return ans;
}
// исполняем слово простое
void execWrd(char *name, int *cfa) {
    printf("execWrd %s :cfa = %d/n", name, cfa);
/*    WPtr = cfa;
    Ptr = (int*)*WPtr;
    Ptr2 = (int *)*Ptr;
    Call = (void(*)())*Ptr2;
    Call();
    */
}
#endif
