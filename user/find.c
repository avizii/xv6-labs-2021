/******************************************
 * Name : find.c                      
 * Purpose :                              
 * Author : Avizii                    
 * Date : 2022.04.23         
 ******************************************/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

const char *P_D = ".";
const char *P_P_D = "..";

void find(char *path, char *keyword) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            // path substring to get relative file name
            p = path + (strlen(path) - strlen(keyword));

            // compare by strcmp
            if (!strcmp(p, keyword)) {
                printf("%s\n", path);
            }

            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("find: path too long\n");
                break;
            }

            strcpy(buf, path);

            p = buf + strlen(buf);
            *p++ = '/';

            while (read(fd, &de, sizeof de) == sizeof de) {
                // tips: don't recurse into . and ..
                if (de.inum == 0 || !strcmp(de.name, P_D) || !strcmp(de.name, P_P_D)) continue;

                int len = strlen(de.name);
                memmove(p, de.name, len);
                p[len] = 0;

                find(buf, keyword);
            }
            break;
    }
    // warning: remember to close the file descriptor
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "Invalid input……\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}