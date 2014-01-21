/*
    Интерпритация входного потока реал-на средствами С
    для отладки 
*/
#ifndef INTERPRET_H
#define INTERPRET_H

#include <stdio.h>
#include "voc.h"

extern void readWords();
extern void findVoc(int, int);

void interpret() {
    printf("> ");
    if(fgets(TIB, NTIB, stdin) != NULL) {
        readWords();
    } else {
        printf("ERROR\n");
    }
}

void readWords() {
    int lenWord = 0;
    int startWord =0;
    for(int i = 0; i < NTIB; i++) {
        if( *(TIB + i) == '\n') {
            if( lenWord > 0) {
                if(*(TIB + i) != ' ') findVoc(startWord, lenWord);
            }
            break;
        }
        if(*(TIB + i) == ' ') {
            if( lenWord != 0) {
                findVoc(startWord, lenWord);
                printf("\n");
            }
            startWord = i + 1;
            lenWord = 0;
        } else lenWord++;
    }
}

void findVoc(int startWord, int lenWord) {
    int endWord = startWord + lenWord;
    for( startWord; startWord < endWord; startWord++) {
        printf("%c", *(TIB + startWord));
    }
}
#endif
