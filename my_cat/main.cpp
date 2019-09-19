#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>



enum {
    MAX_STR_SIZE = 1024*1024,
    STD_OUT      = 0,
    STD_IN       = 1,
};


int save_write(char* buf, int ch_read);


int main(int argc, char** argv) {
    int num_files = (argc > 1) ? (argc - 1) : 1;

    for (int i = 1; i <= num_files; i++) {
        int in_fd = 0;

        if (argc == 1)
            in_fd = STD_IN;
        else {
            in_fd = open(argv[i], O_RDONLY);
            if (in_fd < 0) {
                perror("Can't open the file! ");

                continue;
            }
        }

        for (;;) {
            char buf[MAX_STR_SIZE];
            int ch_read = read(in_fd, buf, MAX_STR_SIZE);
            if(ch_read < 0) {
                perror("Can't read the data! ");

                break;
            }

            if (ch_read == 0) break;

            int ch_write = save_write(buf, ch_read);
            if(ch_write < 0) {
                perror("Can't write the data! ");

                break;
            }
        }

        close(in_fd);
    }
}

int save_write(char* buf, int ch_read) {
    int ch_write     = 0;
    int ch_to_write  = ch_read;
    int cur_ch_write = 0;

    for (;;) {
        cur_ch_write = write(STD_OUT, &buf[ch_write], ch_to_write);

        if (ch_write == ch_read) return ch_write;
        if (cur_ch_write < 0)    return -1;

        ch_write    += cur_ch_write;
        ch_to_write -= ch_write;
    }
}