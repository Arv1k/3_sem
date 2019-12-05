#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>


const int MAX_STR = 512;

enum ERRORs {
    ERROR_WRONG_INPUT,
    ERROR_WRONG_ARGV,
    ERROR_WRONG_STRING,
};


struct msbuf {
    long mtype;
    char mtext[MAX_STR];
};


int runner(int id, long long i, long long number);
int dredd(int id, long long number);


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Wrong arguments format!\n");

        return ERROR_WRONG_ARGV;
    }

    printf("Dredd is here. Begin registration!\n");

    long long number = 0;
    char *is_ok = {};
    number = strtoll(argv[1], &is_ok, 10);

    if (argv[1] == is_ok) {
        printf("Wrong input format!\n");

        return ERROR_WRONG_INPUT;
    }

    if (*is_ok) {
        printf("Wrong string format!\n");

        return ERROR_WRONG_STRING;
    }

    int id = msgget(IPC_PRIVATE, IPC_CREAT | 0606);
    if (id < 0) {
        perror("pipeline");

        return errno;
    }

    for (long long i = 1; i <= number; i++)
        if (!fork()) {
            int err = runner(id, i, number);
            if (err) {
                perror("runner");

                return err;
            }

            exit(0);
        }

    int err = dredd(id, number);
    if (err) {
        perror("dredd");

        return err;
    }

    msgctl(id, IPC_RMID, nullptr);
}

int runner(int id, long long i, long long number) {
    printf("Hello! I'm runner %lld.\n", i);

    msbuf msg = {};
    msg.mtype = i;
    snprintf(msg.mtext, MAX_STR,
             "Hello! I'm %lld participant.", i);

    if (msgsnd(id, (void*) &msg, MAX_STR, 0) < 0) {
        perror("msgsend");

        return errno;
    }

    if (msgrcv(id, (void*) &msg, MAX_STR, i + number, 0) < 0) {
        perror("msgrcv");

        return errno;
    }

    if ((i + number) == number*2)
        printf("%s I'm %lld. Pass it to Dredd\n", msg.mtext, i);
    else
        printf("%s I'm %lld. Pass it to %lld\n", msg.mtext, i, i + 1);

    msg.mtype = i + number + 1;
    snprintf(msg.mtext, MAX_STR,
             "This is esta!");

    if (msgsnd(id, (void*) &msg, MAX_STR, 0) < 0) {
        perror("msgsend");

        return errno;
    }

    return 0;
}

int dredd(int id, long long number) {
    msbuf nmsg = {};
    for (long long i = 1; i <= number; i++) {
        if (msgrcv(id, (void*) &nmsg, MAX_STR, i, 0) < 0) {
            perror("msgrcv");

            return errno;
        }

        printf("Received registration from %ld runner.\n", nmsg.mtype);
    }

    printf("\nEnd of Registration. Participant 1 to start!\n");

    nmsg.mtype = number + 1;
    snprintf(nmsg.mtext, MAX_STR,
                 "This is esta!");

    if (msgsnd(id, (void*) &nmsg, MAX_STR, 0) < 0) {
        perror("msgsend");

        return errno;
    }

    if (msgrcv(id, (void*) &nmsg, MAX_STR, number*2 + 1, 0) < 0) {
        perror("msgrcv");

        return errno;
    }

    printf("%s I'm Dredd.\n\nCompetition end.\n", nmsg.mtext);

    return 0;
}