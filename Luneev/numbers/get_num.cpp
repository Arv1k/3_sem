long long get_num(int argc, char** argv) {
   if (argc != 2) {
       printf("Wrong arguments format!\n");

       return -1;
   }

   long long number = 0;
   char* is_ok = {};
   number = strtoll(argv[1], &is_ok, 10);

   if (argv[1] == is_ok) {
       printf("Wrong input format!\n");

       return -1;
   }

   if (*is_ok) {
       printf("Wrong string format!\n");

       return -1;
   }

   return number;
}
