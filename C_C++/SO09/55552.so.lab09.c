// SO IS1 223A LAB09
// Kacper Małecki
// mk55552@zut.edu.pl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

unsigned long int main(int argc, char** argv) {
    if (argc == 1) {
        fprintf(stderr, "Program must have at least one parameter!\n");
        return 201;
    }

    for (int i = 1; i < argc; ++i) {
        char* endPtr;
        int intArg = strtol(argv[i], &endPtr, 10);
        if (strcmp(endPtr, "") != 0 || intArg > 100 || intArg < 0) {
            fprintf(stderr, "Program arguments must be integers and fall within the range [0, 100]\n");
            return 202;
        }
    }

    if (argc == 2) {
        return atoi(argv[1]);
    }
    if (argc == 3) {
        int firstArg = atoi(argv[1]);
        int secondArg = atoi(argv[2]);
        return firstArg < secondArg ? secondArg : firstArg;
    }

    int sizeLeft = (argc - 1) / 2, sizeRight = (argc - 1) - sizeLeft;
    char *argsLeft[sizeLeft];
    char *argsRight[sizeRight];

    for (int i = 0; i < sizeLeft; ++i) {
        argsLeft[i] = argv[i + 1];
    }
    for (int i = 0; i < sizeRight; ++i) {
        argsRight[i] = argv[i + 1 + sizeLeft];
    }

    PROCESS_INFORMATION procInfo[2];
    STARTUPINFO startInfo = {0};
    startInfo.cb = sizeof(STARTUPINFO);

    char cmdLineLeft[1024] = {0};
    char cmdLineRight[1024] = {0};

    sprintf(cmdLineLeft, "%s", argv[0]);
    for (int i = 0; i < sizeLeft; ++i) {
        strcat(cmdLineLeft, " ");
        strcat(cmdLineLeft, argsLeft[i]);
    }

    sprintf(cmdLineRight, "%s", argv[0]);
    for (int i = 0; i < sizeRight; ++i) {
        strcat(cmdLineRight, " ");
        strcat(cmdLineRight, argsRight[i]);
    }

    // Create left process
    if (!CreateProcess(NULL, cmdLineLeft, NULL, NULL,
        FALSE, 0, NULL, NULL, &startInfo, &procInfo[0])) {
        fprintf(stderr, "Failed to create left process: %lu\n", GetLastError());
        return -1;
    }

    // Create right process
    if (!CreateProcess(NULL, cmdLineRight, NULL, NULL,
        FALSE, 0, NULL, NULL, &startInfo, &procInfo[1])) {
        fprintf(stderr, "Failed to create right process: %lu\n", GetLastError());
        return -1;
    }

    HANDLE handles[2] = {procInfo[0].hProcess, procInfo[1].hProcess};
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);


    DWORD exitCodeLeft, exitCodeRight;
    GetExitCodeProcess(procInfo[0].hProcess, &exitCodeLeft);
    GetExitCodeProcess(procInfo[1].hProcess, &exitCodeRight);

    CloseHandle(procInfo[0].hProcess);
    CloseHandle(procInfo[0].hThread);
    CloseHandle(procInfo[1].hProcess);
    CloseHandle(procInfo[1].hThread);

    printf("%lu %lu %lu |", GetCurrentProcessId(), procInfo[0].dwProcessId, exitCodeLeft);
    for (int i = 0; i < sizeLeft; i++) printf(" %s", argsLeft[i]);
    printf("\n");

    printf("%lu %lu %lu |", GetCurrentProcessId(), procInfo[1].dwProcessId, exitCodeRight);
    for (int i = 0; i < sizeRight; i++) printf(" %s", argsRight[i]);
    printf("\n");

    unsigned long biggerCode = (exitCodeLeft > exitCodeRight) ? exitCodeLeft : exitCodeRight;
    printf("%lu       %d\n\n", GetCurrentProcessId(), biggerCode);

    return biggerCode;
}
