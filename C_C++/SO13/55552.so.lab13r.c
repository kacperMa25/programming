// SO IS1 223A LAB13
// Kacper Małecki
// mk55552@zut.edu.pl

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#define MAX_BYTES 100

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <key> <destination>\n", argv[0]);
        return 1;
    }

    HANDLE MapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        argv[1]);

    if (MapFile == NULL) {
        fprintf(stderr, "Error getting shared memory");
        return 3;
    }

    void *buff0 = MapViewOfFile(
        MapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        MAX_BYTES + 1
        );

    if (buff0 == NULL) {
        fprintf(stderr, "Error attaching shared memory");
        CloseHandle(MapFile);
        return 4;
    }

    int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);

    printf("Shared memory attached, ready to receive\n");
    char *buff = (char*) buff0;
    char *ready = &buff[100];
    *ready = -1; // sender wznawia prace

    while (1) {
        *ready = -1; // sender wznawia prace
        while (*ready == -1) {
        }

        if (*ready >= 0) {
            write(fd, buff, *ready);
            printf("%d bytes recieved...\n", *ready);
            break;
        }

        write(fd, buff, MAX_BYTES);
        printf("%d bytes received...\n", MAX_BYTES);
    }

    printf("Copying finished\n");

    if (close(fd) != 0) {
        int error = errno;
        fprintf(stderr, "Error closing file: %d\n%s\n", error, strerror(error));
        return 5;
    }

    *ready = -3;
    if (!UnmapViewOfFile(buff)) {
        fprintf(stderr, "Error dettaching shared memory");
        CloseHandle(MapFile);
        return 6;
    }
    CloseHandle(MapFile);
    printf("Memory dettached\n");

    return 0;
}