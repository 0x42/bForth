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
// @return возвращает позицию с которой 
// начать след чтение слова
extern int readWords(int pos, struct WordTIB *word);
extern _Bool findVoc(int, int, struct WordTIB *word);
extern void execWrd(char*, int*);
extern void compileWrd(char*, int*);
extern _Bool checkWrd(int, int, struct WordTIB *word);
extern void setError();
extern void printStackData();

void interpret() {
    struct WordTIB *word = (struct WordTIB*)malloc(1*sizeof(struct WordTIB));
    word->name = (char*)malloc(NTIB);
    memset(word->name, 0, NTIB); // обнуляем сроку
    word->len = 0;
    word->state = 0;
    word->cfa = 0;
    int endTIB = 1;
    while(1) {
        printf("> ");
        if(fgets(TIB, NTIB, stdin) != NULL) {
            int pos = 0;
            while(pos != NTIB) {
                printf("%d\n", pos);
                pos = readWords(pos, word);
                printf("%s\n", word->name);
                memset(word->name, 0, NTIB); // обнуляем сроку
                word->len = 0;
                word->state = 0;
                word->cfa = 0;
            }
            //printf("word = %s\n", word);
        } else {
            printf("ERROR\n");
        }
    }
}
// Читает слово в входном потоке-> ищет в словаре еслли находит исполняет если не находит
// проверяет число ли это если да то кладет на стек и переходит к чтению след слова
// иначе кидает ошибку
int readWords(int pos, struct WordTIB* word) {
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
            if( word->len > 0) {
                return i;
            }
            
        }
        if( symbol == ' ') {
            if( word->len != 0) {
             //   if(checkWrd(startWord, lenWord, result) == 0) {
             //       setError();
             //       break;
             //   }
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
_Bool checkWrd(int startWord, int lenWord, struct WordTIB *result) {
    _Bool find = findVoc(startWord, lenWord, result);
        //printf("checkWrd find = %d\n", find);
    if(find == 0) {
        //printf("%s\n len = %d\n", "Проверка число ли это", lenWord);
        char num_str[lenWord + 1];
        char *err;
        int num;
        for(int i = 0; i < lenWord; i++) {
            num_str[i] = *(TIB + startWord + i);
        }
        num_str[lenWord] = '\0';
        // проверяем число это или нет если число то кладем на стек
        num = (int)strtol(num_str, &err, 10);
        //printf("num = %d\n", num);
	//printf("err = %s\n", err);
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
_Bool findVoc(int startWord, int lenWord, struct WordTIB *result) {
    _Bool ans = 0;
    int endWord = startWord + lenWord;
    char *word = (char *)malloc(lenWord);
    int i = 0;
// read Word
    for( startWord; startWord < endWord; startWord++) {
        *(word + i) = *(TIB + startWord);
        i++;
    }
// find Word in Dictionary
    int *ptr = (int *)Nfa;
    int *ptrBuf = 0;
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
    //WPtr = cfa;
   // Ptr = (int*)*WPtr;
    Ptr = cfa;
    Ptr2 = (int *)*Ptr;
    Call = (void(*)())*Ptr2;
    Call();
}
// компилируем слово
void compileWrd(char *name, int*cfa) {
  printf("compileWrd %s :cfa = %d/n", name, cfa);
}
// Обработка ошибки сброс стеков вывод ошибки на терминал
void setError() {
    printf("ERROR\n");
    resetSD();
}
#endif
