/******************************************
 * Name : primes2.c
 * Purpose :
 * Author : Avizii
 * Date : 2022.04.21
 * https://swtch.com/~rsc/thread/
 * https://blog.eastonman.com/blog/2020/11/xv6-primes/
 ******************************************/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void screen_primes(int);

// tips
//   pipe's first file descriptor is to read, second file descriptor is to write
//   why design to this?

int main(int argc, char *argv[]) {
    int p[2]; // p[0] child read , p[1] parent write

    // create a pipe
    pipe(p);

    // fork a child process
    if (fork() == 0) {
        close(p[1]); // close parent write
        screen_primes(p[0]); // ha
    } else {
        close(p[0]); // close child read

        for (int i = 2; i < 36; ++i) {
            write(p[1], &i, sizeof(int)); // write into parent write fd
        }

        close(p[1]); // close parent write
    }

    wait(0); // wait for child process exit
    exit(0);
}

void screen_primes(int read_fd) {
    int prime;

    if (read(read_fd, &prime, sizeof(int)) == 0) {
        close(read_fd);
        exit(0);
    }

    fprintf(2, "primes %d\n", prime);

    int sp[2]; // sp[0] child read, sp[1] parent write

    pipe(sp); // create a pipe

    if (fork() == 0) {
        close(sp[1]);
        screen_primes(sp[0]);
    } else {
        close(sp[0]); // close child write

        int n;
        int eof;
        do {
            eof = read(read_fd, &n, sizeof(int));
            if (eof != 0 && n % prime != 0) {
                write(sp[1], &n, sizeof(int));
            }
        } while (eof);

        close(sp[1]); // close parent write
    }
    wait(0); // wait for child process exit
    exit(0);

}