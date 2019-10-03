#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <cerrno>
#include <time.h>


const int ERROR_WRONG_INPUT = -1;
const int MAX_STR_SIZE = 1024;


int save_write(int fd, char* buf, int ch_read);


int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Wrong input!\n");

        return ERROR_WRONG_INPUT;
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("Pipe ERROR");

        return errno;
    }

    timespec time1 = {};
    timespec time2 = {};

    clock_gettime(CLOCK_MONOTONIC, &time1);

    if (!fork()) {
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        close(pipefd[0]);

        execvp(argv[1], &argv[1]);
    }

    close(pipefd[1]);

    int n_lines = 0;
    int n_words = 0;
    int n_bytes = 0;
    for (;;) {
        char buf[MAX_STR_SIZE];
        int ch_read = read(pipefd[0], buf, MAX_STR_SIZE);
        if(ch_read < 0) {
            perror("Can't read the data! ");

            break;
        }

        int i = 0;
        while(buf[i]) {
            if (buf[i] == '\n')
                n_lines++;

            if (buf[i] == ' ')
                n_words++;

            i++;
        }

        n_bytes += ch_read;

        if (ch_read == 0) break;

        int ch_write = save_write(1, buf, ch_read);
        if(ch_write < 0) {
            perror("Can't write the data! ");

            break;
        }
    }

    wait(nullptr);
    clock_gettime(CLOCK_MONOTONIC, &time2);

    printf("\ntime:%.5lf\n", (time2.tv_sec  - time1.tv_sec) + (time2.tv_nsec - time1.tv_nsec) * (1E-9));

    printf("lines:%d\n"
                  "words:%d\n"
                  "bytes:%d\n", n_lines, n_words, n_bytes);

    close(pipefd[0]);
}

int save_write(int fd, char* buf, int ch_read) {
    int ch_write     = 0;
    int ch_to_write  = ch_read;
    int cur_ch_write = 0;

    for (;;) {
        cur_ch_write = write(fd, &buf[ch_write], ch_to_write);

        if (ch_write == ch_read)
            return ch_write;
        if (cur_ch_write < 0)
            return -1;

        ch_write    += cur_ch_write;
        ch_to_write -= ch_write;
    }
}