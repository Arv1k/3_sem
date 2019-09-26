#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <cassert>
#include <cerrno>


char** form_argv(char* line);


int main() {
    for(;;) {
        char* input = readline("(.Y.): ");
        if (!input)
            return 0;

        char** argv = form_argv(input);

        if (!fork()) execvp(argv[0], argv);

        int exit_status;
        wait(&exit_status);
        errno = WEXITSTATUS(exit_status);
        if (errno)
            perror(argv[0]);

        free(argv);
        free(input);
    }
}

char** form_argv(char* line) {
    assert(line);

    int argc = 0;
    char* cur_tok = strchr(line, ' ');
    while (cur_tok) {
        argc++;
        cur_tok = strchr(++cur_tok, ' ');
    }
    argc++;

    char** argv = (char**) malloc((argc + 1)*sizeof(char*));

    cur_tok = strtok(line, " ");
    for (int i = 0; i < argc; i++) {
        argv[i] = cur_tok;
        cur_tok = strtok(nullptr, " ");
    }

    argv[argc] = nullptr;

    return argv;
}