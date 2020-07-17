#include <cstdio>
#include <cstdlib>
#include <cctype>

enum ERRORs {
    ERROR_WRONG_INPUT,
    ERROR_WRONG_ARGV,
    ERROR_WRONG_STRING,
};

int main(int argc, char** argv) {
   if (argc != 2) {
       printf("Wrong arguments format!\n");

       return ERROR_WRONG_ARGV;
   }

   long long number = 0;
   char* is_ok = {};
   number = strtoll(argv[1], &is_ok, 10);

   if (argv[1] == is_ok) {
       printf("Wrong input format!\n");

       return ERROR_WRONG_INPUT;
   }

   if (*is_ok) {
       printf("Wrong string format!\n");

       return ERROR_WRONG_STRING;
   }

   if (number < 0) {
       for (long long i = 0; i >= number; i--) {
           printf("%lld\n", i);
       }
   }

   for (long long i = 0; i <= number; i++) {
       printf("%lld\n", i);
   }
}