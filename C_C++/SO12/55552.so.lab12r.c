// SO IS1 223A LAB12
// Kacper Małecki
// mk55552@zut.edu.pl

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MEMORY 101
#define MAX_BYTES 100

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <key> <destination>\n", argv[0]);
        return 1;
    }

    int key = atoi(argv[1]);
    if (key == 0) {
        fprintf(stderr, "Error converting key to int\n");
        return 2;
    }

    int shmid = shmget(key, MAX_MEMORY, 0666);
    if (shmid < 0) {
        int error = errno;
        fprintf(stderr, "Error getting shared memory: %d\n%s\n", error,
                strerror(error));
        return 3;
    }

    void *memseg0 = shmat(shmid, NULL, 0);

    if (memseg0 == (void *)-1) {
        int error = errno;
        fprintf(stderr, "Error attaching shared memory: %d\n%s\n", error,
                strerror(error));
        return 4;
    }

    int fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);

    printf("Shared memory attached, ready to receive\n");
    char *memseg = (char *)memseg0;
    char *ready = &memseg[100];
    *ready = -1; // sender wznawia prace

    while (1) {
        *ready = -1; // sender wznawia prace
        while (*ready == -1) {
        }

        if (*ready >= 0) {
            write(fd, memseg, *ready);
            printf("%d bytes recieved...\n", *ready);
            break;
        }

        write(fd, memseg, MAX_BYTES);
        printf("%d bytes received...\n", MAX_BYTES);
    }

    printf("Copying finished\n");

    if (close(fd) != 0) {
        int error = errno;
        fprintf(stderr, "Error closing file: %d\n%s\n", error, strerror(error));
        return 5;
    }

    *ready = -3;
    if (shmdt(memseg) == -1) {
        int error = errno;
        fprintf(stderr, "Error dettaching shared memory: %d\n%s\n", error,
                strerror(error));
        return 6;
    }

    printf("Memory dettached\n");

    return 0;
}
