#include <cstdio>
#include <cmath>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <cerrno>
#include <unistd.h>


enum {
    ERROR_ARGS         = -9090,
    ERROR_INVALID_ARG  = -9091,
};

const long long yad_num = -909091;
const size_t    num_sem = 3;


long long str_to_num(const char* str);

int passenger(long long proc_num, sembuf* semop, int semid);

int ship(long long num_seat, long long num_trip, sembuf* semop, int semid);


int main(int argc, char** argv) {
    if (argc != 4) {
        perror("Wrong arguments number!\n");

        return ERROR_ARGS;
    }

    long long num_seat = str_to_num(argv[1]);
    if (num_seat == yad_num) {
        perror("Invalid argument #1!\n");

        return ERROR_INVALID_ARG;
    }

    long long num_people = str_to_num(argv[2]);
    if (num_people == yad_num) {
        perror("Invalid argument #2!\n");

        return ERROR_INVALID_ARG;
    }

    long long num_trip = str_to_num(argv[3]);
    if (num_trip == yad_num) {
        perror("Invalid argument #3!\n");

        return ERROR_INVALID_ARG;
    }

    int semid = semget(IPC_PRIVATE, num_sem, IPC_CREAT | 0606);
    if (semid < 0) {
        perror("semget");

        return errno;
    }

    sembuf semops[num_sem] = {};
    /*
     * 1 - semaphore for trap  (raised  - 1 or lowered - 0)
     * 2 - semaphore for queue (busy    - 1 or free    - 0)
     * 3 - semaphore for ship  (gone    - 1 or arrived - 0)
     */

    for (long long i = 1; i <= num_people; i++) {
        pid_t ch_op = fork();
        if (ch_op < 0) {
            perror("fork");

            return errno;
        }

        if (!ch_op) {
            if (passenger(i, semops, semid) < 0)
                return errno;

            exit(0);
        }
    }

    if (ship(num_seat, num_trip, semops, semid) < 0)
        return errno;

    if (semctl(semid, num_sem, IPC_RMID) < 0) {
        perror("semctl");

        return errno;
    }
}

long long str_to_num(const char* str) {
    long long number = 0;
    char *is_ok = {};
    number = strtoll(str, &is_ok, 10);

    if (str == is_ok) {
        printf("Wrong input format!\n");

        return yad_num;
    }

    if (*is_ok) {
        printf("Wrong string format!\n");

        return yad_num;
    }

    return number;
}

int passenger(long long proc_num, sembuf* semops, int semid) {

}

int ship(long long num_seat, long long num_trip, sembuf* semops, int semid) {


    for (long long i = 0; i < num_trip; i++) {
        printf("Ship arrived!");

        if (semop(semid, semops, num_sem) < 0) {
            perror("semop");

            return -1;
        }

        
    }

    return 0;
}