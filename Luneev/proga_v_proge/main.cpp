#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("There isn't any program to execute!\n");

        return -1;
    }

    printf("Execute: %s\n\n", argv[1]);

    execv(argv[1], &argv[1]);
}
