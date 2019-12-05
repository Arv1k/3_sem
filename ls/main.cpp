#include <dirent.h>
#include <cerrno>
#include <getopt.h>
#include <malloc.h>
#include <cstring>


enum ERRORs {
    ERROR_WRONG_NUM_ARGS = -111,
    ERROR_UNKNOWN_OPT    = -112,
    ERROR_ERROR          = -113,
};

struct u_opt {
    char opt_long;
    char opt_all;
    char opt_inode;
    char opt_numeric;
    char opt_recursive;
    char opt_column;
};


u_opt used_opt = {};
const char opts[] = "lainR1"; // -d?
const option long_opts[] =
                   {{"long",      0, nullptr, 'l'},
                    {"all",       0, nullptr, 'a'},
                    {"inode",     0, nullptr, 'i'},
                    {"numeric",   0, nullptr, 'n'},
                    {"recursive", 0, nullptr, 'R'},
                    {"column",    0, nullptr, '1'},
                    {nullptr,     0, nullptr, 0  }};


int list_source(char* path);

int print_elem_info(dirent* cur_elem, char* path);

int print_long(dirent* cur_elem, char* path);

int print_short(dirent* cur_elem, char* path);


int main(int argc, char** argv) {
    for (;;) {
        int cur_opt = getopt_long(argc, argv, opts, long_opts, nullptr);
        if (cur_opt < 0)
            break;

        if (cur_opt == '?')
            return ERROR_UNKNOWN_OPT;

        switch (cur_opt) {
            case 'l':
                used_opt.opt_long = 1;
                break;

            case 'a':
                used_opt.opt_all = 1;
                break;

            case 'i':
                used_opt.opt_inode = 1;
                break;

            case 'n':
                used_opt.opt_numeric = 1;
                break;

            case 'R':
                used_opt.opt_recursive = 1;
                break;

            case '1':
                used_opt.opt_column = 1;
                break;

            default:
                printf("Something wrong with options!\n");

                return ERROR_ERROR;
        }
    }

    char* path;
    if (optind == argc) {
        path = strdup(".");

        if (list_source(path) < 0)
            return errno;

        free(path);
        return 0;
    }

    for (int i = optind; i < argc; i++) {
        path = strdup(argv[i]);

        if (list_source(path) < 0)
            return errno;

        free(path);
    }
}

int list_source(char* path) {
    DIR* cur_dir = opendir(path);
    if (!cur_dir) {
        perror("opendir");

        return -1;
    }

    errno = 0;
    for (;;) {
        dirent* cur_elem = readdir(cur_dir);
        if (errno) {
            perror("readdir");

            return -1;
        }

        if (!cur_elem)
            break;

        if (!used_opt.opt_all)
            if (cur_elem->d_name[0] == '.')
                continue;

        if (print_elem_info(cur_elem, path) < 0)
            return -1;
    }

    errno = 0;
    char* path1 = strdup(path);
    if (used_opt.opt_recursive) {
        for (;;) {
            dirent *cur_elem = readdir(cur_dir);
            if (errno) {
                perror("readdir");

                return -1;
            }

            if (!cur_elem)
                break;

            if (!used_opt.opt_all)
                if (cur_elem->d_name[0] == '.')
                    continue;

            if (cur_elem->d_type == DT_DIR)
                if (list_source(path) < 0)
                    return -1;
        }
    }

    if (!used_opt.opt_column)
        printf("\n");


    free(path1);
    return 0;
}

int print_elem_info(dirent* cur_elem, char* path) {
    int ret_val = 0;

    if (used_opt.opt_long || used_opt.opt_numeric)
        ret_val = print_long(cur_elem, path);

    else
        ret_val = print_short(cur_elem, path);

    return ret_val;
}

int print_short(dirent* cur_elem, char* path) {
    if (used_opt.opt_inode)
        printf("%lu ", cur_elem->d_ino);

    printf("%s  ", cur_elem->d_name);

    if (used_opt.opt_column)
        printf("\n");

    return 0;
}

int print_long(dirent* cur_elem, char* path) {


    return 0;
}