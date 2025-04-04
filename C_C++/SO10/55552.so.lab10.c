//SO IS1 223A LAB10
//Kacper Małecki
//mk55552@zut.edu.pl

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_WORDS 370104

int hist[26] = {0};

void getWords(char*** words) {
    *words = malloc(MAX_WORDS * sizeof(char*));
    FILE* file = fopen("/home/kacper/CLionProjects/SO10/slownik.txt", "r");
    assert(file != NULL);
    char line[255];
    for (int i = 0; i < MAX_WORDS; i++) {
        if (fgets(line, sizeof(line), file)) {
            (*words)[i] = strdup(line);
        } else break;
    }
    fclose(file);
}

void freeWords(char* words[]) {
    for (int i = 0; i < MAX_WORDS; i++) {
        free(words[i]);
    }
    free(words);
}

int checkArgs(char* argc[]) {
    char* endPtr;
    int intArg = strtol(argc[1], &endPtr, 10);
    if (strcmp(endPtr, "") != 0 || intArg < 0 || intArg > 16) {
        return -1;
    }
    return intArg;
}

pthread_mutex_t hist_mutex = PTHREAD_MUTEX_INITIALIZER;

void *countChar(void *data) {
    int localHist[26] = {0};
    pthread_t tid = pthread_self();
    const char** words = (const char**) data;
    printf("Thread #%ld\n", tid);
    for (int i = 0; words[i] != NULL; i++) {
        for (int j = 0; words[i][j] != '\n'; j++) {
            char c = tolower(words[i][j]);
            if (c >= 'a' && c <= 'z') {
                localHist[c - 'a']++;
            }
        }
    }
    pthread_mutex_lock(&hist_mutex);
    for (int i = 0; i < 26; i++) {
        hist[i] += localHist[i];
    }
    pthread_mutex_unlock(&hist_mutex);
    return NULL;
}

void countCharWoThreads(char* words[]) {
    for (int i = 0; words[i] != NULL; i++) {
        for (int j = 0; words[i][j] != '\n'; j++) {
            char c = tolower(words[i][j]);
            if (c >= 'a' && c <= 'z') {
                hist[c - 'a']++;
            }
        }
    }
}


void allocForThreads(char**** threadArgs, int numThreads, int wordPerThread, int extraWords) {
    *threadArgs = malloc(numThreads * sizeof(char**));
    for (int i = 0; i < numThreads - 1; i++) {
        (*threadArgs)[i] = malloc(wordPerThread * sizeof(char*));
    }
    (*threadArgs)[numThreads - 1] = malloc((wordPerThread + extraWords) * sizeof(char*));
}

void freeForThreads(char ****threadArgs, int numThreads) {
    for (int i = 0; i < numThreads; i++) {
        free((*threadArgs)[i]);
    }
    free(*threadArgs);
}

int main(int argv, char** argc) {
    if (argv != 2) {
        fprintf(stderr, "Usage: SO10 \"n\"\n"
                        "where: n has to fall in range (0 - 16)");
        return 1;
    }
    char** words = 0;
    getWords(&words);

    int numThreads = checkArgs(argc);
    if (numThreads == -1) {
        fprintf(stderr, "Usage: SO10 \"n\"\n"
                        "where: n has to fall in range (0 - 16)");
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    if (numThreads == 0) {
        countCharWoThreads(words);
    }
    else {
        pthread_t threads[numThreads];
        int extraWords = MAX_WORDS % numThreads;
        int wordPerThread = MAX_WORDS / numThreads;

        char*** threadArgs;
        allocForThreads(&threadArgs, numThreads, wordPerThread, extraWords);

        for (int i = 0; i < MAX_WORDS;) {
            for (int j = 0; j < numThreads - 1; j++) {
                for (int k = 0; k < wordPerThread; k++, i++) {
                    threadArgs[j][k] = words[i];
                }
            }
            for (int k = 0; k < wordPerThread + extraWords; k++, i++) {
                threadArgs[numThreads - 1][k] = words[i];
            }
        }

        for (int i = 0; i < numThreads; i++) {
            pthread_create(&threads[i], NULL, countChar, threadArgs[i]);
        }
        for (int i = 0; i < numThreads; i++) {
            pthread_join(threads[i], NULL);
        }

        freeForThreads(&threadArgs, numThreads);
    }
    freeWords(words);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %lf\n", elapsed);
    for (int i = 0, c = 'a'; i < 26; i++, c++) {
        if (i % 5 == 0) printf("\n");
        printf("%c: %d  ", c, hist[i]);
    }
    return 0;
}
