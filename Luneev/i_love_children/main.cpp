#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include "../numbers/get_num.cpp"
#include <cmath>

int main(int argc, char** argv) {
    long long number = get_num(argc, argv);
    if (number <= 0) {
        printf("Wrong number!\n");

        return 1;
    }

    printf("Parent PID: %d\n\n", getpid());

    for(long long i = 0; i < number; i++)
        if (fork() == 0) {
            printf("PID: %d, serial number: %lld\n", getpid(), i + 1);
            exit(0);
        }
}