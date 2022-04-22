/******************************************
 * Name : pingpong.c                      
 * Purpose :                              
 * Author : Avizii                    
 * Date : 2022.04.20         
 ******************************************/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2]; // child read, parent write
    int p2[2]; // parent read, child write

    int pid;

    char buf[2];

    // create a pipe for parent write, child read
    pipe(p1);

    // create a pipe for parent read, child write
    pipe(p2);

    // create a child
    pid = fork();

    // child
    if (pid == 0) {
        close(p1[1]); // close parent write
        close(p2[0]); // close parent read

        read(p1[0], buf, sizeof buf); // read child read
        close(p1[0]); // close child read

        fprintf(2, "%d: receive ping\n", getpid());

        write(p2[1], "s", 2);

        close(p2[1]); // close child write

    } else {
        close(p1[0]); // close child read
        close(p2[1]); // close child write

        write(p1[1], "s", 2); // write parent write
        close(p1[1]); // close parent write

        read(p2[0], buf, sizeof buf); // read parent read
        close(p2[0]); // close parent read

        fprintf(2, "%d: receive pong\n", getpid());

    }
    exit(0);
}