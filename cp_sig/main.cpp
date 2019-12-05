#include <cstdio>
#include <unistd.h>
#include <csignal>


enum ERRORs {
    ERROR_NUMBER_ARGC = -111,
};


int child_dest(char* file_name);

int parent_src(char* file_name);

int flag = 0;


int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Wrong number of arguments!\n");

        return ERROR_NUMBER_ARGC;
    }

    int ch_op = fork();

    if (!ch_op) {
        int ret_val = child_dest(argv[2]);

        return ret_val;
    }

    parent_src(argv[1]);
}

int child_dest(char* file_name) {

}

int parent_src(char* file_name) {
    printf("hello\n");

    sigset_t mask = {};
    sigset_t nmask = {};
    nmask.__val[4] = 1;

    sigprocmask(SIG_BLOCK, &mask, nullptr);
    while (!flag) {
        sigsuspend(&nmask);


    }

    for (int i = 10000; i > 0; i--) {
        printf("\r ~");
        usleep(50000);
        fflush(stdout);

        printf("\r \\");
        usleep(50000);
        fflush(stdout);

        printf("\r /");
        usleep(50000);
        fflush(stdout);

        printf("\r ~");
        usleep(50000);
        fflush(stdout);
    }
}