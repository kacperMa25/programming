//SO IS1 223A LAB08
//Kacper Małecki
//mk55552@zut.edu.pl

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "Program must have at least one parameter!\n");
    return 201;
  }

  for (int i = 1; i < argc; ++i) {
    char *endPtr;
    int intArg = strtol(argv[i], &endPtr, 10);
    if (strcmp(endPtr, "") != 0 || intArg > 100 || intArg < 0) {
      fprintf(stderr, "Program arguments must be integers and fall within the "
                      "range [0, 100]\n");
      return 202;
    }
  }

  if (argc == 2) {
    return atoi(argv[1]);
  }
  if (argc == 3) {
    return atoi(argv[1]) < atoi(argv[2]) ? atoi(argv[2]) : atoi(argv[1]);
  }

  int sizeLeft = (argc - 1) / 2, sizeRight = (argc - 1) - sizeLeft;
  char **argsLeft = (char **)malloc(sizeLeft * sizeof(char *));
  char **argsRight = (char **)malloc(sizeRight * sizeof(char *));

  for (int i = 0; i < sizeLeft; ++i) {
    argsLeft[i] = argv[i + 1];
  }
  for (int i = 0; i < sizeRight; ++i) {
    argsRight[i] = argv[i + 1 + sizeLeft];
  }

  pid_t pidLeft = fork();
  if (pidLeft < 0) {
    fprintf(stderr, "Fork() failed: %s\n", strerror(errno));
    return -1;
  }
  if (pidLeft == 0) {
    char **newArgs = (char **)malloc((sizeLeft + 2) * sizeof(char *));
    newArgs[0] = argv[0];
    for (int i = 0; i < sizeLeft; ++i) {
      newArgs[i + 1] = argsLeft[i];
    }
    newArgs[sizeLeft + 1] = NULL;
    execv(argv[0], newArgs);
    fprintf(stderr, "exec failed: %s\n", strerror(errno));
    free(newArgs);
    return -1;
  }

  pid_t pidRight = fork();
  if (pidRight < 0) {
    fprintf(stderr, "Fork() failed: %s\n", strerror(errno));
    return -1;
  }
  if (pidRight == 0) {
    char **newArgs = (char **)malloc((sizeRight + 2) * sizeof(char *));
    newArgs[0] = argv[0];
    for (int i = 0; i < sizeRight; ++i) {
      newArgs[i + 1] = argsRight[i];
    }
    newArgs[sizeRight + 1] = NULL;
    execv(argv[0], newArgs);
    fprintf(stderr, "exec failed: %s\n", strerror(errno));
    free(newArgs);
    return -1;
  }

  int statusLeft, statusRight;
  waitpid(pidLeft, &statusLeft, 0);
  waitpid(pidRight, &statusRight, 0);

  int codeLeft = WEXITSTATUS(statusLeft);
  int codeRight = WEXITSTATUS(statusRight);

  printf("%d %d %d |", getpid(), pidLeft, codeLeft);
  for (int i = 0; i < sizeLeft; i++)
    printf(" %s", argsLeft[i]);
  printf("\n");

  printf("%d %d %d |", getpid(), pidRight, codeRight);
  for (int i = 0; i < sizeRight; i++)
    printf(" %s", argsRight[i]);
  printf("\n");

  int biggerCode = (codeLeft > codeRight) ? codeLeft : codeRight;
  printf("%d       %d\n\n", getpid(), biggerCode);

  free(argsLeft);
  free(argsRight);
  return biggerCode;
}
