/******************************************
 * Name : sleep.c                      
 * Purpose :                              
 * Author : Avizii                    
 * Date : 2022.04.20         
 ******************************************/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: sleep numbers...\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        int num = atoi(argv[i]);
        sleep(num);
    }
    exit(0);
}