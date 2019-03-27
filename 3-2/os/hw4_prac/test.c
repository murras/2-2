#include <stdio.h>
#define SFS_NOINO 0 /* inode # for free dir entry */
typedef unsigned int u_int32_t;
typedef unsigned short u_int16_t;
typedef unsigned char u_int8_t;
#define SFS_NAMELEN 60 /* max length of filename */

struct sfs_dir {
    u_int32_t sfd_ino;          /* Inode number */
    char sfd_name[SFS_NAMELEN]; /* Filename */
};
int main() {
    FILE *fp = fopen("test.exe", "r");
    if(fp==NULL){
        printf("here\n");
    }
    else{
        printf("nothere\n");
    }
}