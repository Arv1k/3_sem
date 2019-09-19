#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

enum ERRORs {
    ERROR_WRONG_INPUT,
    ERROR_WRONG_ARGV,
    ERROR_WRONG_STRING,
};

int main(const int argc, char** argv) {
    if (argc <= 2) {
        printf("Wrong arguments format!\n");

        return ERROR_WRONG_ARGV;
    }

    int* numbers = (int*) calloc(argc - 1, sizeof(int));

    for (int i = 1; i < argc; i++) {
        long long number = 0;
        char *is_ok = {};
        number = strtoll(argv[i], &is_ok, 10);

        if (argv[i] == is_ok) {
            printf("Wrong input format!\n");

            return ERROR_WRONG_INPUT;
        }

        if (*is_ok) {
            printf("Wrong string format!\n");

            return ERROR_WRONG_STRING;
        }

        numbers[i - 1] = (int) number;

        if (fork() == 0) {
            usleep(number*10000);
            printf("%lld ", number);

            exit(0);
        }
    }

    for (int i = 1; i < argc; i++)
        wait(nullptr);

    printf("\n");

    int exit_status = 0;
    int sum = 0;

    int start = 0;
    int end   = (argc - 1)/2;
    if (fork() == 0) {
        for (int i = start; i < end; i++)
            sum += numbers[i];

        return sum;
    }

    start = (argc - 1)/2;
    end   = argc + 1;
    if (fork() == 0) {
        for (int i = start; i < end; i++)
            sum += numbers[i];

        return sum;
    }

    wait(&exit_status);
    sum += WEXITSTATUS(exit_status);

    wait(&exit_status);
    sum += WEXITSTATUS(exit_status);

    printf("\nSum = %d\n", sum);

    free(numbers);
}