#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>


enum {
    MAX_STR_SIZE = 1024,
    STD_OUT      = 0,
    STD_IN       = 1,
};


int main(int argc, char** argv) {
    size_t num_files = 0;

    num_files = (argc > 1) ? (argc - 1) : 1;

    for (size_t i = 1; i <= num_files; i++) {
        size_t in_fd = 0;
        size_t size  = 0;

        if (argc == 1) {
            in_fd = STD_IN;
            size  = MAX_STR_SIZE;
        }
        else {
            in_fd = open(argv[i], O_RDONLY);

            struct stat file_stat = {};
            fstat(in_fd, &file_stat);

            size = file_stat.st_size;
        }

        char *buf = (char *) calloc(size, 1);

        size_t ch_read = read(in_fd, buf, MAX_STR_SIZE);
        while (ch_read) {
            write(STD_OUT, buf, ch_read);
            ch_read = read(in_fd, buf, MAX_STR_SIZE);
        }
        printf("\n");


        close(in_fd);
        free(buf);
    }
}