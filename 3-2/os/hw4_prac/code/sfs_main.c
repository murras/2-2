// A few extra command added
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfs_func.h"
#define DELIMS " \t\r\n"
#define MAX_ARGC 10

int main() {
    char buf[256];
    int argc;
    char* argv[MAX_ARGC];

    printf("OS SFS shell\n");

    while (!feof(stdin)) {
        printf("os_shell> ");

        fgets(buf, sizeof(buf), stdin);

        argv[0] = strtok(buf, DELIMS);
        if (!argv[0])
            continue;

        argc = 1;
        while ((argv[argc] = strtok(NULL, DELIMS)) != NULL) {
            argc++;
        }

        if (!strcmp(argv[0], "mount")) {
            if (argc != 2) {
                printf("usage: mount disk_img\n");
                continue;
            }

            sfs_mount(argv[1]);
            continue;
        }

        if (!strcmp(argv[0], "umount")) {
            sfs_umount();
            continue;
        }

        if (!strcmp(argv[0], "ls")) {
            if (argc == 1)
                sfs_ls(NULL);
            else if (argc == 2)
                sfs_ls(argv[1]);
            else {
                printf("usage: ls [path]\n");
            }
            continue;
        }

        if (!strcmp(argv[0], "cd")) {
            if (argc == 1)
                sfs_cd(NULL);
            else if (argc != 2) {
                printf("usage: cd [path]\n");
                continue;
            }

            sfs_cd(argv[1]);
            continue;
        }

        if (!strcmp(argv[0], "dump")) {
            sfs_dump();
            continue;
        }

        if (!strcmp(argv[0], "touch")) {
            if (argc != 2) {
                printf("usage: touch path\n");
                continue;
            }

            sfs_touch(argv[1]);
            continue;
        }

        if (!strcmp(argv[0], "mkdir")) {
            if (argc != 2) {
                printf("usage: mkdir directory\n");
                continue;
            }

            sfs_mkdir(argv[1]);
            continue;
        }

        if (!strcmp(argv[0], "rmdir")) {
            if (argc != 2) {
                printf("usage: rmdir directory\n");
                continue;
            }

            sfs_rmdir(argv[1]);
            continue;
        }

        if (!strcmp(argv[0], "rm")) {
            if (argc != 2) {
                printf("usage: rm path\n");
                continue;
            }

            sfs_rm(argv[1]);
            continue;
        }

        if (!strcmp(argv[0], "mv")) {
            if (argc != 3) {
                printf("usage: mv src dst\n");
                continue;
            }

            sfs_mv(argv[1], argv[2]);
            continue;
        }

        if (!strcmp(argv[0], "cpin")) {
            if (argc != 3) {
                printf("usage: copyin local-file file(source)\n");
                continue;
            }

            sfs_cpin(argv[1], argv[2]);
            continue;
        }

        if (!strcmp(argv[0], "cpout")) {
            if (argc != 3) {
                printf("usage: copyout local-file(source) file\n");
                continue;
            }

            sfs_cpout(argv[1], argv[2]);
            continue;
        }

        if (!strcmp(argv[0], "exit")) {
            printf("bye\n");
            return 0;
        }

        if (!strcmp(argv[0], "fsck")) {
            sfs_fsck();
            continue;
        }

        if (!strcmp(argv[0], "bitmap")) {
            sfs_bitmap();
            continue;
        }
        /*
		if( !strcmp(argv[0], "fixdir") )
		{
			sfs_fixdir();
			continue;
		}

		if( !strcmp(argv[0], "fixfiles") )
		{
			sfs_fixfiles();
			continue;
		}

		if( !strcmp(argv[0], "test") )
		{
			if(	argc != 2 )
			{
				printf("usage: test argv\n");
				continue;
			}

			sfs_test(argv[1]);
			continue;
		}
*/
        printf("%s command not found\n", argv[0]);
    }

    return 0;
}
