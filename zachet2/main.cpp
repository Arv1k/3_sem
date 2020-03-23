#include <cstdio>
#include <sys/types.h>
#include <dirent.h>
#include <cerrno>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <pwd.h>


struct user_procs {
    uid_t uid      = -1;
    char* name     = nullptr;
    char* paswwd   = nullptr;
    int   num_proc = 0;
};


const int n_users = 32;


int main(int argc, char** argv) {
    DIR* proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("opendir");

        return errno;
    }

    user_procs hey[n_users] = {};
    int free_uid = 0;
    for (;;) {
        errno = 0;
        dirent* cur_elem = readdir(proc_dir);
        if (errno) {
            perror("readdir");

            break;
        }

        if (!cur_elem)
            break;

        if (isdigit(cur_elem->d_name[0])) {
            /*printf("%s\n", cur_elem->d_name);*/
            char* path = (char*) calloc(5 + strlen(cur_elem->d_name), 1);

            strcat(path, "/proc/");
            strcat(path, cur_elem->d_name);

            /*printf("%s\n", path);*/

            struct stat statbuf = {};
            if (lstat(path, &statbuf) < 0) {
                perror("lstat");

                break;
            }

            /*printf("%d\n", statbuf.st_uid);*/

            int my_user_num = 999;
            for (int i = 0; i < n_users; i++)
                if (hey[i].uid == statbuf.st_uid) {
                    my_user_num = i;

                    break;
                }

            if (my_user_num == 999) {
                my_user_num = free_uid++;
                hey[my_user_num].uid = statbuf.st_uid;

                passwd* cur_user = getpwuid(statbuf.st_uid);
                if (!cur_user) {
                    perror("getpwuid");

                    return errno;
                }

                hey[my_user_num].paswwd = strdup(cur_user->pw_passwd);
                hey[my_user_num].name = strdup(cur_user->pw_name);
            }

            /*passwd* cur_user = getpwuid(statbuf.st_uid);
            printf("%s\n", cur_user->pw_name);*/

            hey[my_user_num].num_proc++;

            /*printf("%s\n\n", hey[my_user_num].name);*/

            free(path);
        }
    }


    for (int i = 0; i < n_users; i++)
        if (hey[i].name) {
            printf("%s: %d\n", hey[i].name, hey[i].num_proc);
            printf("%s\n\n", hey[i].paswwd);

            free(hey[i].paswwd);
            free(hey[i].name);
        }

    if (closedir(proc_dir) < 0) {
        perror("closedir");

        return errno;
    }
}
