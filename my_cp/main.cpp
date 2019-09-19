#include <unistd.h>
#include <cstdio>


enum ERRORs {
    ER_WRONG_INPUT = -1,
};

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Wrong input format!");

        return ER_WRONG_INPUT;
    }

    bool flags[4] = {};
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            switch (*argv[i]) {
                
            }
        }
    }
}