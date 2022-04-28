/******************************************
 * Name : xargs.c                      
 * Purpose :                              
 * Author : Avizii                    
 * Date : 2022.04.24
 * https://zhuanlan.zhihu.com/p/272199762
 ******************************************/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // shell 将管道 | 左边的输入写入管道，并将右边的 0 文件描述符由console输入改为管道输入
    if (argc <= 1) {
        fprintf(2, "Usage: xargs [command ...]\n");
        exit(1);
    }

    int fd = 0; // read from the fd

    char buf[1024];

    int n, m;
    char *p, *q;

    m = 0;
    while ((n = read(fd, buf + m, sizeof(buf) - m - 1)) > 0) {
        m += n;
        buf[m] = '\0';
        p = buf;

        while ((q = strchr(p, '\n')) != 0) {
//            *q = '\n';
            char arg[q + 1 - p];
            memcpy(arg, p, q + 1 - p);
            if (fork() == 0) {
                char *new_argv[argc + 1];
                for (int i = 1; i < argc; i++) {
                    new_argv[i - 1] = argv[i];
                }
                new_argv[argc - 1] = arg;
                new_argv[argc] = 0;

//                printf("new_argv[0]: %s\n", new_argv[0]);
//                printf("new_argv[1]: %s\n", new_argv[1]);
//                printf("new_argv[2]: %s\n", new_argv[2]);
//                printf("new_argv[3]: %s\n", new_argv[3]);
                exec(new_argv[0], new_argv);

                printf("exec failed!\n");

                exit(1);
            } else {
                wait(0);
                p = q + 1;
            }

        }
        if (m > 0) {
            m -= p - buf;
            memmove(buf, p, m);
        }
    }

    exit(0);
}