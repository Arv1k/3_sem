#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <readline/readline.h>
#include <sys/wait.h>
#include <cassert>
#include <cerrno>


char** form_argv(char* line, int* n_pipe);

int main() {
    for(;;) {
        char a[4096];

        char* input = readline("\033[33m(&_&): \033[0m");

        if (!input)
            return 0;

        int n_pipe= 0;
        char** argv = form_argv(input, &n_pipe);

        if (n_pipe) {
            int pipefd[2];
            if (pipe(pipefd) < 0) {
                perror("pipe");

                return errno;
            }

            char** cur_pc = argv;
            char** argv2  = argv;
            for (int i = 0; i < n_pipe; i++) {
                while(**cur_pc != '|')
                    cur_pc++;
                *(cur_pc++) = nullptr;

                                    
                if (!fork()) {
                    dup2(pipefd[1], 1);
                    close(pipefd[1]);
                    close(pipefd[0]);

                    if (execvp(argv2[0], argv2) < 0)
                        perror(argv2[0]);
                    exit(0);
                }
                close(pipefd[1]);



                argv2 = cur_pc;
            }
        }

        if (!fork()) {
            if (execvp(argv[0], argv) < 0)
                perror(argv[0]);

            exit(-1);
        }

        int exit_status;
        wait(&exit_status);

        if (WIFSIGNALED(exit_status))
            printf("(Signal %d)\n",  WTERMSIG(exit_status));

        free(argv);
        free(input);
    }
}

char** form_argv(char* line, int* n_pipe) {
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

    if (!argc)
        return nullptr;

    char** argv = (char**) malloc((argc + 1)*sizeof(char*));

    cur_tok = strtok(line, " ");
    for (int i = 0; i < argc; i++) {
        if (cur_tok)
            if (*cur_tok == '|')
                *n_pipe += 1;

        argv[i] = cur_tok;
        cur_tok = strtok(nullptr, " ");
    }

    argv[argc] = nullptr;

    return argv;
}