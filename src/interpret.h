/*
    Интерпритация входного потока реал-на средствами С
    для отладки 
    прочитать слово в буфер
    найти его в словаре -> скомпилировать/интерпретировать
    проверить число ли это -> положить на стек
    Слово не может быть больше NTIB - 1
*/
#ifndef INTERPRET_H
#define INTERPRET_H

#include <stdio.h>
#include "voc.h"

// слово найденное в входном потоке
struct WordTIB {
    char* name;
    int len;
    int state;
    int *cfa;
};

void resetWordTIB(struct WordTIB *word) {
    memset(word->name, 0, NTIB); // обнуляем сроку
    word->len = 0;
    word->state = 0;
    word->cfa = 0;
};
// @return возвращает позицию с которой 
// начать след чтение слова
extern int readWords(int pos, struct WordTIB *word, int *flag);
extern _Bool findVoc(struct WordTIB *word);
extern void execWord(struct WordTIB *word);
extern void compileWord(struct WordTIB *word);
extern _Bool checkWord(struct WordTIB *word);
extern void setError();
extern void printStackData();

void interpret() {
    struct WordTIB *word = (struct WordTIB*)malloc(1*sizeof(struct WordTIB));
    word->name = (char*)malloc(NTIB);
    resetWordTIB(word);
    int endTIB = 1;
    // 1 последние слово в потоке / 0 - читать дальше
    int lastWord = 0;
    // 1 найдено слово 0 не найдено
    int flagFind = 0;
    while(1) {
        printf("> ");
        if(fgets(TIB, NTIB, stdin) != 0) {
            int pos = 0;
            while(pos != NTIB) {
                pos = readWords(pos, word, &lastWord);
                printf("read[%s] pos = %d \n", word->name, pos);
                if(word->len > 0) {
                    if( checkWord(word) == 1) {
                        if( STATE == 0 && word->cfa > 0) execWord(word);
                        else if (STATE == 1) compileWord(word);
                    } else {
                        setError();
                        resetWordTIB(word);
                        break;
                    }
                }
                resetWordTIB(word);
                if(lastWord == 1) {
                    lastWord = 0; 
                    break; 
                }
            }
            printStackData();
        } else {
            lastWord = 0;
            setError();
        }
    }
}
// Читает слово в входном потоке -> найденное слово сохраняем в струтуру word
// @return - возв позицию с оторой надо читать след слово
int readWords(int pos, struct WordTIB* word, int *flag) {
    int lenWord = 0;
    int startWord = 0;
    char symbol;
    for(int i = pos; i < NTIB; i++) {
        symbol = *(TIB + i);
        if( symbol != ' ' && symbol != '\n') {
            *(word->name + word->len) = symbol;
            word->len++;
        }
        if( symbol == '\n') {
            *flag = 1;
            return i;
        }
        if( symbol == ' ') {
            if( word->len != 0) {
                return i;
            }
        }
    }
}
// вывод стека на терминал
void printStackData() {
    printf("STACK DATA: ");
    for(int i = 0; i < ptrSD; i++) {
	printf("%d ", *(SDinit + i));
    }
    printf("\n");
}
// 0 - false
// 1 - true
_Bool checkWord(struct WordTIB *word) {
    _Bool find = findVoc(word);
//    return find; 
        //printf("checkWrd find = %d\n", find);
    if(find == 0) {
        //printf("%s\n len = %d\n", "Проверка число ли это", lenWord);
        char *err;
        int num;
        // проверяем число это или нет если число то кладем на стек
        num = (int)strtol(word->name, &err, 10);
	if(strlen(err) > 0) {
            return 0;
        } else {
            pushSD(num);
            return 1;
        }
    } else {
        return 1;
    }
}
// 0 - не найдено в словаре
// 1 - найдено
_Bool findVoc(struct WordTIB *word) {
    _Bool ans = 0;
// find Word in Dictionary
    int *ptr = (int *)Nfa;
    int *ptrBuf = 0;
    _Bool flgFind = 0;
    int nNameWrd = 0;
    while(ptr != 0) {
        nNameWrd = *ptr - 80;
        // Если длины слов совпадают то проверяем посимвольно 
        // иначе к след слову
        if(word->len == nNameWrd) {
//            printf("findVoc->n = %d\n", nNameWrd);
            int ii;
            ptrBuf = ptr;
            for(int i = 1; i < nNameWrd + 1; i++) {
                ptrBuf++;
                ii = i - 1;
//                printf("stdin %c\n", *(word->name + ii));
//                printf("tib %c\n", (char)*(ptrBuf));
                if(*(word->name + ii) != (char)*(ptrBuf)) break;
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
//        printf("[%s]find in voc\n", word->name);
//        int *cfa  = (ptr + nNameWrd + 3);
//        int *pfa = (int *) *cfa;
        word->cfa  = (ptr + nNameWrd + 3);
        ans = 1;
    } else {
//        printf("[%s] doesn't find in voc\n", word->name);
        word->cfa = 0;
        ans = 0;
    }
    return ans;
}
// исполняем слово простое
void execWord(struct WordTIB *word) {
    // WPtr = cfa;
    // Ptr = (int*)*WPtr;
    Ptr = word->cfa;
    Ptr2 = (int *)*Ptr;
    Call = (void(*)())*Ptr2;
    Call();
}
// компилируем слово
void compileWord(struct WordTIB *word) {
  printf("compileWrd %s :cfa = %d/n", word->name, word->cfa);
}
// Обработка ошибки сброс стеков вывод ошибки на терминал
void setError() {
    printf("ERROR\n");
    resetSD();
}

#endif