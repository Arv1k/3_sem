#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <cerrno>
#include <sys/stat.h>


enum ERRORs {
    ER_WRONG_INPUT   = -1,
};


int save_write(int fd, char* buf, int ch_read);
void cp_src_to_dest (int fd_src, int fd_dest);


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Wrong input format!\n");

        return ER_WRONG_INPUT;
    }

    int fd_src  = open(argv[1], O_RDONLY);
    if (fd_src < 0) {
        perror(argv[1]);

        return errno;
    }

    struct stat src_stat = {};
    int check_fstat = fstat(fd_src, &src_stat);
    if (check_fstat < 0) {
        perror(argv[1]);

        return errno;
    }

    int fd_dest = open(argv[2],
                       O_WRONLY | O_CREAT | O_TRUNC,
                       src_stat.st_mode);


    if (fd_dest < 0) {
        perror(argv[2]);

        return errno;
    }

    cp_src_to_dest(fd_src, fd_dest);

    close(fd_src);
    close(fd_dest);
}

const int MAX_STR_SIZE = 1024;
void cp_src_to_dest (int fd_src, int fd_dest) {
    for (;;) {
        char buf[MAX_STR_SIZE];
        int ch_read = read(fd_src, buf, MAX_STR_SIZE);
        if(ch_read < 0) {
            perror("Can't read the data! ");

            break;
        }

        if (ch_read == 0) break;

        int ch_write = save_write(fd_dest, buf, ch_read);
        if(ch_write < 0) {
            perror("Can't write the data! ");

            break;
        }
    }

    close(fd_src);
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