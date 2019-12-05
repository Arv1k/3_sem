#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
#include <cerrno>


enum ERRORs {
    ER_NUM_ARGS = -111,
};

const int num_bog = 33;


void *bogat_thread(void *vargp);


char *song;
sem_t semaphore;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Wrong number of arguments!\n");

        return ER_NUM_ARGS;
    }

    song = strdup(argv[1]);

    if (sem_init(&semaphore, 0, 1) < 0) {
        perror("sem_init");

        return errno;
    }

    errno = 0;
    for (int i = 0; i < num_bog; i++) {
        pthread_t tid;
        int ret = pthread_create(&tid, nullptr, bogat_thread, nullptr);
        if (ret != 0) {
            perror("pthread_create");

            return ret;
        }
    }

    if (sem_destroy(&semaphore) < 0) {
        perror("sem_destroy");

        return errno;
    }

    free(song);
}


int g = 0;
int alphabet[26] = {};

#ifdef MEOW
#define PRINTF(...) printf(...)

#else
#define PRINTF(...) ;

#endif

void *bogat_thread(void *vargp) {
    char my_char = 0;
    unsigned long tid = pthread_self();

    for (;;) {
        PRINTF("My ID: %lu. Waiting for access.\n", tid);

        if (sem_wait(&semaphore) < 0) {
            perror("sem_wait");

            break;
        }

        PRINTF("My ID: %lu. Get access!\n", tid);

        char cur_char = song[g];
        if (cur_char == '\0') {
            if (sem_post(&semaphore) < 0)
                perror("sem_post");

            PRINTF("My ID: %lu. God...\n", tid);

            break;
        }

        if (my_char == 0) {
            if (alphabet[cur_char - 97] == 0) {
                alphabet[cur_char - 97]++;

                my_char = cur_char;
            }
        }

        if (my_char == cur_char) {
            printf("%c ", my_char);

            g++;
        }

        if (sem_post(&semaphore) < 0) {
            perror("sem_post");

            break;
        }

        PRINTF("My ID: %lu. I've done.\n", tid);
    }

    return nullptr;
}