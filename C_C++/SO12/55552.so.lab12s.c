// SO IS1 223A LAB12
// Kacper Małecki
// mk55552@zut.edu.pl

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BYTES 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return 1;
    }

    key_t id = ftok(argv[0], 1);

    if (id < 0) {
        int error = errno;
        fprintf(stderr, "Error creating key: %d\n%s\n", error, strerror(error));
        return 2;
    }

    int shmid = shmget(id, 101, 0666 | IPC_CREAT | IPC_EXCL);

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
    char *memseg = (char *)memseg0;
    char *ready = &memseg[100];
    *ready = -2;

    printf("Shared memory attached, key = %d\n", id);

    int fd = open(argv[1], O_RDONLY);

    printf("Waitning for reciever...\n");
    while (*ready == -2) {
    }
    *ready = -1; // sender wznawia prace

    int readBytes;
    do {
        readBytes = read(fd, memseg, MAX_BYTES);
        printf("%d bytes sent...\n", readBytes);
        if (readBytes != MAX_BYTES) {
            *ready = readBytes; // sender konczy prace
        } else {
            *ready = -2;
        }
        while (*ready == -2) {
        }

    } while (readBytes == MAX_BYTES);

    *ready = readBytes;

    while (*ready != -3) {
    }

    printf("Copying finished, reciever finished\n");

    if (close(fd) != 0) {
        int error = errno;
        fprintf(stderr, "Error closing file: %d\n%s\n", error, strerror(error));
        return 5;
    }

    if (shmdt(memseg) == -1) {
        int error = errno;
        fprintf(stderr, "Error dettaching shared memory: %d\n%s\n", error,
                strerror(error));
        return 6;
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        int error = errno;
        fprintf(stderr, "Error removing shared memory: %d\n%s\n", error,
                strerror(error));
        return 7;
    }
    return 0;
}
