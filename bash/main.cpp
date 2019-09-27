#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <readline/readline.h>
#include <sys/wait.h>
#include <cassert>


char** form_argv(char* line);


int main() {
    for(;;) {
        char* input = readline("(&_&): ");
        if (!input)
            return 0;

        char** argv = form_argv(input);

        if (!fork()) {
            if (execvp(argv[0], argv) < 0)
                perror(argv[0]);

            exit(0);
        }

        wait(nullptr);

        free(argv);
        free(input);
    }
}

char** form_argv(char* line) {
    assert(line);

    int argc = 1;
    char* cur_tok = line;
    for (;;) {
        while (*cur_tok == ' ')
            cur_tok++;

        cur_tok = strchr(cur_tok, ' ');

        if (!cur_tok) break;

        argc++;
        cur_tok++;
    }

    char** argv = (char**) malloc((argc + 1)*sizeof(char*));

    cur_tok = strtok(line, " ");
    for (int i = 0; i < argc; i++) {
        argv[i] = cur_tok;
        cur_tok = strtok(nullptr, " ");
    }

    argv[argc] = nullptr;

    return argv;
}

