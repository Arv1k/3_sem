#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

int main() {
    if (fork() == 0) {
        execlp("echo", "echo", "Hello,", "world!", nullptr);
        exit(0);
    }

    wait(nullptr);
}
