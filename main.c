#include "main.h"

//clear && gcc prim.c -o prim.o -c && gcc runner.c -o runner.o -c && gcc escalonador.c -o escalonador.o -c && gcc main.c -o main.o -c && gcc -o run prim.o runner.o escalonador.o main.o

int main(int argc, char **argv){
    time_t t;
    srand((unsigned) time(&t));

    int process_count;
    if(argc == 1){
        process_count = 10;
    }
    else{
        process_count = atoi(argv[1]);
    }
    // char cwd[PATH_MAX];
    // getcwd(cwd, sizeof(cwd));
    // printf("%s\n", cwd);
    // strcat(cwd, "/programas/rapido");


    // clock_t begin, end;

    // begin = clock();
    // execl(cwd, cwd, (char *)NULL);
    // end = clock();


    // printf("%ld\n", end-begin);
    int process_types[process_count];
    for(int i = 0; i < process_count; i++){
        process_types[i] = rand()%3;
        printf("%d ", process_types[i]);
    }
    printf("\n");

    //chamar escalonador;
    escalonador(process_count, process_types);


    return 0;
}