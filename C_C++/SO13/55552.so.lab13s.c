// SO IS1 223A LAB13
// Kacper Małecki
// mk55552@zut.edu.pl

#include <windows.h>
#include <fcntl.h>
#include <stdio.h>
#include <io.h>
#include <string.h>

#define MAX_BYTES 100

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <filename> <mapping_name>\n", argv[0]);
        return 1;
    }

    HANDLE mapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        MAX_BYTES + 1,
        argv[2]);

    if (mapFile == NULL) {
        fprintf(stderr, "Error creating file mapping: %ld\n", GetLastError());
        return 2;
    }

    void *memseg = MapViewOfFile(
        mapFile,
        FILE_MAP_WRITE,
        0,
        0,
        MAX_BYTES + 1);

    if (memseg == NULL) {
        fprintf(stderr, "Error mapping view of file: %ld\n", GetLastError());
        CloseHandle(mapFile);
        return 3;
    }

    char *ready = (char *)memseg + 100;
    *ready = -2;

    printf("Shared memory created, mapping name = %s\n", argv[2]);

    int fd = _open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        UnmapViewOfFile(memseg);
        CloseHandle(mapFile);
        return 4;
    }

    printf("Waiting for receiver...\n");
    while (*ready == -2) {
        Sleep(10);
    }

    int readBytes;
    do {
        readBytes = _read(fd, memseg, MAX_BYTES);
        printf("%d bytes sent...\n", readBytes);

        if (readBytes != MAX_BYTES) {
            *ready = readBytes;
        } else {
            *ready = -2;
        }

        while (*ready == -2) {
            Sleep(10);
        }
    } while (readBytes == MAX_BYTES);

    *ready = readBytes;

    while (*ready != -3) {
        Sleep(10);
    }

    printf("Copying finished, receiver finished\n");

    _close(fd);
    UnmapViewOfFile(memseg);
    CloseHandle(mapFile);

    return 0;
}
