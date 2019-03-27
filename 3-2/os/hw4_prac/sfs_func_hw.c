//
// Simple FIle System
// Student Name : 
// Student Number :
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* optional */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/***********/

#include "sfs.h"
#include "sfs_disk.h"
#include "sfs_func.h"
#include "sfs_types.h"

void dump_directory();

/* BIT operation Macros */
/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a, b) ((a) |= (1 << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1 << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1 << (b)))
#define BIT_CHECK(a, b) ((a) & (1 << (b)))

static struct sfs_super spb;                 // superblock
static struct sfs_dir sd_cwd = {SFS_NOINO};  // current working directory

int i, j;  // iterator

void error_message(const char* message, const char* path, int error_code) {
    switch (error_code) {
        case -1:
            printf("%s: %s: No such file or directory\n", message, path);
            return;
        case -2:
            printf("%s: %s: Not a directory\n", message, path);
            return;
        case -3:
            printf("%s: %s: Directory full\n", message, path);
            return;
        case -4:
            printf("%s: %s: No block available\n", message, path);
            return;
        case -5:
            printf("%s: %s: Not a directory\n", message, path);
            return;
        case -6:
            printf("%s: %s: Already exists\n", message, path);
            return;
        case -7:
            printf("%s: %s: Directory not empty\n", message, path);
            return;
        case -8:
            printf("%s: %s: Invalid argument\n", message, path);
            return;
        case -9:
            printf("%s: %s: Is a directory\n", message, path);
            return;
        case -10:
            printf("%s: %s: Is not a file\n", message, path);
            return;
        default:
            printf("unknown error code\n");
            return;
    }
}

void sfs_mount(const char* path) {
    if (sd_cwd.sfd_ino != SFS_NOINO) {
        //umount
        disk_close();
        printf("%s, unmounted\n", spb.sp_volname);
        bzero(&spb, sizeof(struct sfs_super));
        sd_cwd.sfd_ino = SFS_NOINO;
    }

    printf("Disk image: %s\n", path);

    disk_open(path);
    disk_read(&spb, SFS_SB_LOCATION);

    printf("Superblock magic: %x\n", spb.sp_magic);

    assert(spb.sp_magic == SFS_MAGIC);

    printf("Number of blocks: %d\n", spb.sp_nblocks);
    printf("Volume name: %s\n", spb.sp_volname);
    printf("%s, mounted\n", spb.sp_volname);

    sd_cwd.sfd_ino = 1;  //init at root
    sd_cwd.sfd_name[0] = '/';
    sd_cwd.sfd_name[1] = '\0';
}

void sfs_umount() {
    if (sd_cwd.sfd_ino != SFS_NOINO) {
        //umount
        disk_close();
        printf("%s, unmounted\n", spb.sp_volname);
        bzero(&spb, sizeof(struct sfs_super));
        sd_cwd.sfd_ino = SFS_NOINO;
    }
}

void sfs_touch(const char* path) {
    //skeleton implementation

    struct sfs_inode si;
    disk_read(&si, sd_cwd.sfd_ino);

    //for consistency
    assert(si.sfi_type == SFS_TYPE_DIR);

    //we assume that cwd is the root directory and root directory is empty which has . and .. only
    //unused DISK2.img satisfy these assumption
    //for new directory entry(for new file), we use cwd.sfi_direct[0] and offset 2
    //becasue cwd.sfi_directory[0] is already allocated, by .(offset 0) and ..(offset 1)
    //for new inode, we use block 6
    // block 0: superblock,	block 1:root, 	block 2:bitmap
    // block 3:bitmap,  	block 4:bitmap 	block 5:root.sfi_direct[0] 	block 6:unused
    //
    //if used DISK2.img is used, result is not defined

    //buffer for disk read
    struct sfs_dir sd[SFS_DENTRYPERBLOCK];

    //block access
    disk_read(sd, si.sfi_direct[0]);

    //allocate new block
    int newbie_ino = 6;

    sd[2].sfd_ino = newbie_ino;
    strncpy(sd[2].sfd_name, path, SFS_NAMELEN);

    disk_write(sd, si.sfi_direct[0]);

    si.sfi_size += sizeof(struct sfs_dir);
    disk_write(&si, sd_cwd.sfd_ino);

    struct sfs_inode newbie;

    bzero(&newbie, SFS_BLOCKSIZE);  // initalize sfi_direct[] and sfi_indirect
    newbie.sfi_size = 0;
    newbie.sfi_type = SFS_TYPE_FILE;

    disk_write(&newbie, newbie_ino);
}

void sfs_cd(const char* path) {
    // if path is null, cd root direcotry
    if (path == NULL) {
        sd_cwd.sfd_ino = SFS_ROOT_LOCATION;
        sd_cwd.sfd_name[0] = '/';
        sd_cwd.sfd_name[1] = '\0';
        for (int i = 2; i < SFS_NAMELEN < i++)
            sd_cwd.sfd_name = '\0';
    }
    // if path is not null
    else {
        struct sfs_inode cwdInode;
        struct sfs_dir dirBlock;
        // read cwd Inode
        disk_read(&cwdInode, sd_cwd.sfd_ino);
        // ndirect checking
        for (i = 0; i < SFS_NDIRECT; i++) {
            // if null -> break
            if (cwdInode.sfi_direct[i] == NULL) {
                error_message("cd", path, -1);
                break;
            }
            // if not null -> read the block
            disk_read(&dirBlock, sfi_direct[i]);
            // one block = 512(BLOCKSIZE) / sizeof(struct sfs_dir) -> 8
            for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
                // if path == sfd_name -> valid path
                if (strcmp(path, dirBlock[j].sfd_name) == 0) {
                    struct sfs_inode childInode;
                    disk_read(&childInode, dirBlock[j].sfd_ino);
                    // if path is directory -> success
                    if (childInode.sfi_type == SFS_TYPE_DIR) {
                        // change sd_cwd
                        sd_cwd.sfd_ino = dirBlock.sfd_ino;
                        strcpy(sd_cwd.sfd_name, dirBlock.sfd_name);
                    }
                    // if file -> fail, error return
                    else if (childInode.sfi_type == SFS_TYPE_FILE) {
                        error_message("cd", path, -2)
                    }
                }
            }
        }
    }
}

void sfs_ls(const char* path) {
        int i, j;
    struct sfs_inode currDirInode, subInode;
    struct sfs_dir dirEntry[SFS_DENTRYPERBLOCK];
    static struct sfs_inode subDirInode;
    static int flag = 0;

    //To implement recursive function, use static variable (flag)
    if (flag == 0)
        disk_read(&currDirInode, sd_cwd.sfd_ino);
    else
        currDirInode = subDirInode;

    //should search whole direct pointer in this HW
    //in case with making a lot of directory, direct pointer[0] could be overflow.
    //after that, another dir block will be allocated to be used.
    //also, some directory entry can be removed,
    //but there is no mechanism whether reordering entry or not, just keep the entry.

    for (i = 0; i < SFS_NDIRECT; i++) {
        if (currDirInode.sfi_direct[i] == INVALID_BLOCK) break;
        disk_read(dirEntry, currDirInode.sfi_direct[i]);

        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            if (path == NULL) {
                if (dirEntry[j].sfd_ino != SFS_NOINO) {
                    disk_read(&subInode, dirEntry[j].sfd_ino);
                    if (subInode.sfi_type == SFS_TYPE_FILE)
                        printf("%s\t", dirEntry[j].sfd_name);
                    else
                        printf("%s/\t", dirEntry[j].sfd_name);
                }
            } else {
                if (dirEntry[j].sfd_ino != SFS_NOINO && !strcmp(path, dirEntry[j].sfd_name)) {
                    disk_read(&subInode, dirEntry[j].sfd_ino);
                    if (subInode.sfi_type == SFS_TYPE_FILE)
                        printf("%s\n", dirEntry[j].sfd_name);
                    else {
                        subDirInode = subInode;
                        flag = 1;
                        sfs_ls(NULL);
                        flag = 0;
                    }
                    return;
                }
            }
        }
    }
    if (path == NULL)
        printf("\n");
    else
        error_message("ls", path, -1);

}

void sfs_mkdir(const char* org_path) {
    printf("Not Implemented\n");
}

void sfs_rmdir(const char* org_path) {
    printf("Not Implemented\n");
}

void sfs_mv(const char* src_name, const char* dst_name) {
    printf("Not Implemented\n");
}

void sfs_rm(const char* path) {
    printf("Not Implemented\n");
}

void sfs_cpin(const char* local_path, const char* path) {
    printf("Not Implemented\n");
}

void sfs_cpout(const char* local_path, const char* path) {
    printf("Not Implemented\n");
}

void dump_inode(struct sfs_inode inode) {
    int i;
    struct sfs_dir dir_entry[SFS_DENTRYPERBLOCK];

    printf("size %d type %d direct ", inode.sfi_size, inode.sfi_type);
    for (i = 0; i < SFS_NDIRECT; i++) {
        printf(" %d ", inode.sfi_direct[i]);
    }
    printf(" indirect %d", inode.sfi_indirect);
    printf("\n");

    if (inode.sfi_type == SFS_TYPE_DIR) {
        for (i = 0; i < SFS_NDIRECT; i++) {
            if (inode.sfi_direct[i] == 0) break;
            disk_read(dir_entry, inode.sfi_direct[i]);
            dump_directory(dir_entry);
        }
    }
}

void dump_directory(struct sfs_dir dir_entry[]) {
    int i;
    struct sfs_inode inode;
    for (i = 0; i < SFS_DENTRYPERBLOCK; i++) {
        printf("%d %s\n", dir_entry[i].sfd_ino, dir_entry[i].sfd_name);
        disk_read(&inode, dir_entry[i].sfd_ino);
        if (inode.sfi_type == SFS_TYPE_FILE) {
            printf("\t");
            dump_inode(inode);
        }
    }
}

void sfs_dump() {
    // dump the current directory structure
    struct sfs_inode c_inode;

    disk_read(&c_inode, sd_cwd.sfd_ino);
    printf("cwd inode %d name %s\n", sd_cwd.sfd_ino, sd_cwd.sfd_name);
    dump_inode(c_inode);
    printf("\n");
}
