#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <readline/readline.h>
#include <sys/wait.h>
#include <cassert>

char** form_argv(char* line);

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");

    char buf[1024];
    while (fgets(buf, 1024, input) != nullptr) {
        char** argv_new = form_argv(buf);

        if (!fork()) 
            execvp(argv_new[0], argv_new);
        
        wait(nullptr);

        free(argv_new);
    }

    fclose(input);
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

    if (!argc)
        return nullptr;

    char** argv = (char**) malloc((argc + 1)*sizeof(char*));

    cur_tok = strtok(line, " ");
    for (int i = 0; i < argc; i++) {
        argv[i] = cur_tok;
        cur_tok = strtok(nullptr, " ");
    }

    argv[argc] = nullptr;

    return argv;
}

