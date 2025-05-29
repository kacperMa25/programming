//SO IS1 223A LAB10
//Kacper Małecki
//mk55552@zut.edu.pl

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define MAX_WORDS 370104

int hist[26] = {0};
HANDLE mutex;

typedef struct {
    char** words;
    int count;
} ThreadData;

void getWords(char*** words) {
    *words = malloc(MAX_WORDS * sizeof(char*));
    FILE* file = fopen("C:\\Users\\asasy\\CLionProjects\\ThreadsWin\\slownik.txt", "r");
    assert(file != NULL);
    char line[255];
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < MAX_WORDS) {
        line[strcspn(line, "\n")] = '\0';
        (*words)[i++] = strdup(line);
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

DWORD WINAPI countChar(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    int localHist[26] = {0};

    for (int i = 0; i < data->count; i++) {
        char* word = data->words[i];
        for (int j = 0; word[j] != '\0'; j++) {
            char c = tolower(word[j]);
            if (c >= 'a' && c <= 'z') {
                localHist[c - 'a']++;
            }
        }
    }

    WaitForSingleObject(mutex, INFINITE);
    for (int i = 0; i < 26; i++) {
        hist[i] += localHist[i];
    }
    ReleaseMutex(mutex);
    return 0;
}

void countCharWoThreads(char* words[]) {
    for (int i = 0; words[i] != NULL; i++) {
        for (int j = 0; words[i][j] != '\0'; j++) {
            char c = tolower(words[i][j]);
            if (c >= 'a' && c <= 'z') {
                hist[c - 'a']++;
            }
        }
    }
}

void allocForThreads(ThreadData* threadArgs, char** words, int numThreads, int wordPerThread, int extraWords) {
    for (int i = 0, idx = 0; i < numThreads; i++) {
        int count = (i == numThreads - 1) ? (wordPerThread + extraWords) : wordPerThread;
        threadArgs[i].words = &words[idx];
        threadArgs[i].count = count;
        idx += count;
    }
}

int main(int argv, char** argc) {
    if (argv != 2) {
        fprintf(stderr, "Usage: SO10 \"n\"\nwhere: n has to fall in range (0 - 16)\n");
        return 1;
    }

    char** words = NULL;
    getWords(&words);

    int numThreads = checkArgs(argc);
    if (numThreads == -1) {
        fprintf(stderr, "Usage: SO10 \"n\"\nwhere: n has to fall in range (0 - 16)\n");
        freeWords(words);
        return 1;
    }

    clock_t start_t, end_t;
    start_t = clock();

    mutex = CreateMutex(NULL, FALSE, NULL);
    if (numThreads == 0) {
        countCharWoThreads(words);
    } else {
        HANDLE threads[numThreads];
        ThreadData threadArgs[numThreads];

        int extraWords = MAX_WORDS % numThreads;
        int wordPerThread = MAX_WORDS / numThreads;

        allocForThreads(threadArgs, words, numThreads, wordPerThread, extraWords);

        for (int i = 0; i < numThreads; i++) {
            threads[i] = CreateThread(NULL, 0, countChar, &threadArgs[i], 0, NULL);
        }

        WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);

        for (int i = 0; i < numThreads; i++) {
            CloseHandle(threads[i]);
        }
    }

    CloseHandle(mutex);
    freeWords(words);

    end_t = clock();
    double elapsed = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Elapsed time: %lf seconds\n", elapsed);

    for (int i = 0, c = 'a'; i < 26; i++, c++) {
        if (i % 5 == 0) printf("\n");
        printf("%c: %d  ", c, hist[i]);
    }
    printf("\n");

    return 0;
}
