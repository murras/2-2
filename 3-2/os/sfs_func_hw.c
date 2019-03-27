#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* optional */
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/***********/

#include "sfs.h"
#include "sfs_disk.h"
#include "sfs_func.h"
#include "sfs_types.h"

void dump_directory();
void sfs_ls_child();
/* BIT operation Macros */
/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a, b) ((a) |= (1 << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1 << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1 << (b)))
#define BIT_CHECK(a, b) ((a) & (1 << (b)))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

#define DEBUG 0
static struct sfs_super spb;                 // superblock
static struct sfs_dir sd_cwd = {SFS_NOINO};  // current working directory

struct sfs_inode ls_cwdInode, ls_childInode;  // inode for ls operation

u_int32_t bitMapBlockSize;
const u_int32_t DIRSIZE = sizeof(struct sfs_dir);
u_int32_t havetoFree[150];
const u_int32_t fileMaxSize = (SFS_NDIRECT + SFS_DBPERIDB) * SFS_BLOCKSIZE;

// dir entry(1) + file inode(1) + direct(15) + indirect(1) + indirect-IN(128) = 146

u_int32_t getBlock();
void freeBlock(u_int32_t);
u_int32_t getFileSize(FILE*);
void cpin_free(u_int32_t index);

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
            printf("%s: can't open %s input file\n", message, path);
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
        case -11:
            printf("%s: input file size exceeds the max file size\n", message, path);
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

    bitMapBlockSize = SFS_BITBLOCKS(spb.sp_nblocks);
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

void sfs_ls(const char* path) {
    int i, j;
    struct sfs_dir dEntry[SFS_DENTRYPERBLOCK];

    disk_read(&ls_cwdInode, sd_cwd.sfd_ino);
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (ls_cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(dEntry, ls_cwdInode.sfi_direct[i]);

        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            disk_read(&ls_childInode, dEntry[j].sfd_ino);
            // if path is null
            if (path == NULL) {
                // print about cwd
                if (ls_childInode.sfi_type == SFS_TYPE_DIR) {
                    printf("%s/\t", dEntry[j].sfd_name);
                } else if (ls_childInode.sfi_type == SFS_TYPE_FILE) {
                    printf("%s\t", dEntry[j].sfd_name);
                }

            } else {
                // Checking path
                if (strcmp(path, dEntry[j].sfd_name) == 0) {
                    if (ls_childInode.sfi_type == SFS_TYPE_DIR) {
                        ls_cwdInode = ls_childInode;
                        sfs_ls_child();
                        return;
                    } else if (ls_childInode.sfi_type == SFS_TYPE_FILE) {
                        printf("%s\n", dEntry[j].sfd_name);
                        return;
                    }
                }
            }
        }
    }
    if (path == NULL) {
        printf("\n");
    } else {
        error_message("ls", path, -1);
        return;
    }
}

void sfs_ls_child() {
    int i, j;
    struct sfs_dir dEntry[SFS_DENTRYPERBLOCK];

    for (i = 0; i < SFS_NDIRECT; i++) {
        if (ls_cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(dEntry, ls_cwdInode.sfi_direct[i]);

        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            disk_read(&ls_childInode, dEntry[j].sfd_ino);
            // print about cwd
            if (ls_childInode.sfi_type == SFS_TYPE_DIR) {
                printf("%s/\t", dEntry[j].sfd_name);
            } else if (ls_childInode.sfi_type == SFS_TYPE_FILE) {
                printf("%s\t", dEntry[j].sfd_name);
            }
        }
    }
    printf("\n");
}

void sfs_cd(const char* path) {
    int i, j;
    struct sfs_inode cwdInode, childInode;
    struct sfs_dir dEntry[SFS_DENTRYPERBLOCK];

    // if path is null, cd root direcotry
    if (path == NULL) {
        sd_cwd.sfd_ino = SFS_ROOT_LOCATION;
        sd_cwd.sfd_name[0] = '/';
        for (i = 1; i < SFS_NAMELEN; i++)
            sd_cwd.sfd_name[i] = '\0';
    }
    // if path is not null
    else {
        // read cwd Inode
        disk_read(&cwdInode, sd_cwd.sfd_ino);
        for (i = 0; i < SFS_NDIRECT; i++) {
            if (cwdInode.sfi_direct[i] == 0) {
                error_message("cd", path, -1);
                return;
            }

            disk_read(dEntry, cwdInode.sfi_direct[i]);
            // one block = 512(BLOCKSIZE) / sizeof(struct sfs_dir) -> 8
            for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
                // if path == sfd_name -> hit
                if ((dEntry[j].sfd_ino != SFS_NOINO) && (strcmp(path, dEntry[j].sfd_name) == 0)) {
                    disk_read(&childInode, dEntry[j].sfd_ino);
                    // if path is directory -> success
                    if (childInode.sfi_type == SFS_TYPE_DIR) {
                        // change sd_cwd
                        // void * memcpy ( void * destination, const void * source, size_t num );
                        memcpy((void*)&sd_cwd, (void*)&dEntry[j], DIRSIZE);
                        return;
                    }
                    // if file -> fail, error return
                    else if (childInode.sfi_type == SFS_TYPE_FILE) {
                        error_message("cd", path, -2);
                        return;
                    }
                }
            }
        }
    }
}

void sfs_mv(const char* src_name, const char* dst_name) {
    int i, j;
    struct sfs_inode cwdInode;
    struct sfs_dir dEntry[SFS_DENTRYPERBLOCK];
    bool srcExist, dstExist;
    srcExist = dstExist = false;
    int srcdEntry, srcdEntryIndex;

    bool srcCheck = !strcmp(src_name, ".") || !strcmp(src_name, "..");
    bool dstCheck = !strcmp(dst_name, ".") || !strcmp(dst_name, "..");
    if (srcCheck) {
        error_message("mv", src_name, -8);
        return;
    }
    if (dstCheck) {
        error_message("mv", dst_name, -8);
        return;
    }

    disk_read(&cwdInode, sd_cwd.sfd_ino);

    // 1. src, dst check first
    // file name change after error checking.
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(dEntry, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            // src check
            if (!srcExist && strcmp(src_name, dEntry[j].sfd_name) == 0) {
                srcExist = true;
                srcdEntry = i;
                srcdEntryIndex = j;
            }
            // dst check
            if (!dstExist && strcmp(dst_name, dEntry[j].sfd_name) == 0) {
                // already exist
                dstExist = true;
            }
            if (srcExist && dstExist)
                break;
        }
        if (srcExist && dstExist)
            break;
    }
    // 2. Error check
    if (srcExist && !dstExist) {
        // 3. Normal operation
        // Change name and Write
        disk_read(dEntry, cwdInode.sfi_direct[srcdEntry]);
        int dstLength = strlen(dst_name);
        strncpy(dEntry[srcdEntryIndex].sfd_name, dst_name, dstLength);
        dEntry[srcdEntryIndex].sfd_name[dstLength] = '\0';
        disk_write(dEntry, cwdInode.sfi_direct[srcdEntry]);
        return;
    }
    if (srcExist && dstExist) {
        error_message("mv", dst_name, -6);
    } else if (!srcExist) {
        error_message("mv", src_name, -1);
    }
}

void sfs_touch(const char* path) {
    int i, j;
    struct sfs_inode cwdInode;
    struct sfs_dir cwd[SFS_DENTRYPERBLOCK];

    disk_read(&cwdInode, sd_cwd.sfd_ino);
    u_int32_t size = cwdInode.sfi_size / DIRSIZE;
    u_int32_t maxSize = SFS_NDIRECT * SFS_DENTRYPERBLOCK;
    // Size Check
    if (size >= maxSize) {
        error_message("touch", path, -3);
        return;
    }
    // Path Check
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            if ((cwd[j].sfd_ino != SFS_NOINO) && (strcmp(path, cwd[j].sfd_name) == 0)) {
                error_message("touch", path, -6);
                return;
            }
        }
    }
    // bzero(newbie) and newbie setting
    struct sfs_inode newbieInode;
    struct sfs_dir newbieDEntry[SFS_DENTRYPERBLOCK];
    bzero(&newbieInode, SFS_BLOCKSIZE);
    newbieInode.sfi_size = 0;
    newbieInode.sfi_type = SFS_TYPE_FILE;
    // Get info about empty entry
    bool empty, first;
    empty = first = false;
    u_int32_t blockNumber, entryIndex, directIndex;
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            first = true;
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            if (cwd[j].sfd_ino == SFS_NOINO) {
                empty = true;
                break;
            }
        }
        if (empty) {
            break;
        }
        j = 0;
    }
    directIndex = i;
    if (first)
        entryIndex = 0;
    else
        entryIndex = j;
    blockNumber = cwdInode.sfi_direct[directIndex];
    if (empty) {
        disk_read(cwd, blockNumber);
        cwd[entryIndex].sfd_ino = getBlock();
        if (cwd[entryIndex].sfd_ino == false) {
            error_message("touch", path, -4);
            return;
        }
    } else {
        blockNumber = getBlock();
        if (blockNumber == false) {
            error_message("touch", path, -4);
            return;
        }
        cwdInode.sfi_direct[directIndex] = blockNumber;
        bzero(cwd, SFS_BLOCKSIZE);
        cwd[entryIndex].sfd_ino = getBlock();
        if (cwd[entryIndex].sfd_ino == false) {
            error_message("touch", path, -4);
            freeBlock(blockNumber);
            return;
        }
    }
    // Size plus
    cwdInode.sfi_size += DIRSIZE;
    // Name assign
    strncpy(cwd[entryIndex].sfd_name, path, SFS_NAMELEN);
    // Write all
    disk_write(&cwdInode, sd_cwd.sfd_ino);
    disk_write(cwd, blockNumber);
    disk_write(&newbieInode, cwd[entryIndex].sfd_ino);
}

void sfs_mkdir(const char* path) {
    int i, j;
    struct sfs_inode cwdInode;
    struct sfs_dir cwd[SFS_DENTRYPERBLOCK];

    disk_read(&cwdInode, sd_cwd.sfd_ino);
    u_int32_t size = cwdInode.sfi_size / DIRSIZE;
    u_int32_t maxSize = SFS_NDIRECT * SFS_DENTRYPERBLOCK;
    // Size Check
    if (size >= maxSize) {
        error_message("mkdir", path, -3);
        return;
    }
    // Path Check
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            if ((cwd[j].sfd_ino != SFS_NOINO) && (strcmp(path, cwd[j].sfd_name) == 0)) {
                error_message("mkdir", path, -6);
                return;
            }
        }
    }
    // Get info about empty entry
    bool empty, first;
    empty = first = false;
    u_int32_t blockNumber, entryIndex, directIndex;
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            first = true;
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            if (cwd[j].sfd_ino == SFS_NOINO) {
                empty = true;
                break;
            }
        }
        if (empty) {
            break;
        }
        j = 0;
    }
    directIndex = i;
    if (first)
        entryIndex = 0;
    else
        entryIndex = j;
    blockNumber = cwdInode.sfi_direct[directIndex];
    u_int32_t havetoFree1, havetoFree2;
    bool free1, free2;
    free1 = free2 = false;
    havetoFree1 = havetoFree2 = 0;
    if (empty) {
        disk_read(cwd, blockNumber);
    } else {
        blockNumber = getBlock();
        if (blockNumber == false) {
            error_message("mkdir", path, -4);
            return;
        }
        free1 = true;
        havetoFree1 = cwdInode.sfi_direct[directIndex] = blockNumber;
        bzero(cwd, SFS_BLOCKSIZE);
    }
    havetoFree2 = cwd[entryIndex].sfd_ino = getBlock();
    if (cwd[entryIndex].sfd_ino == false) {
        error_message("mkdir", path, -4);
        if (free1) freeBlock(havetoFree1);
        return;
    }
    free2 = true;
    // Size plus
    cwdInode.sfi_size += DIRSIZE;
    // Name assign
    strncpy(cwd[entryIndex].sfd_name, path, SFS_NAMELEN);
    // bzero(newbie) and newbie setting
    struct sfs_inode newbieInode;
    struct sfs_dir newbieDEntry[SFS_DENTRYPERBLOCK];
    bzero(&newbieInode, SFS_BLOCKSIZE);  //
    newbieInode.sfi_size = 2 * DIRSIZE;
    newbieInode.sfi_type = SFS_TYPE_DIR;
    newbieInode.sfi_direct[0] = getBlock();
    if (newbieInode.sfi_direct[0] == 0) {
        error_message("mkdir", path, -4);
        if (free1) freeBlock(havetoFree1);
        if (free2) freeBlock(havetoFree2);
        return;
    }
    bzero(newbieDEntry, SFS_BLOCKSIZE);
    // Newbie direct entry[0] = ".", [1] = ".." and its inode setting
    strncpy(newbieDEntry[0].sfd_name, ".", SFS_NAMELEN);
    strncpy(newbieDEntry[1].sfd_name, "..", SFS_NAMELEN);
    newbieDEntry[0].sfd_ino = cwd[entryIndex].sfd_ino;
    newbieDEntry[1].sfd_ino = sd_cwd.sfd_ino;
    // Write all
    disk_write(&cwdInode, sd_cwd.sfd_ino);
    disk_write(cwd, blockNumber);
    disk_write(&newbieInode, cwd[entryIndex].sfd_ino);
    disk_write(newbieDEntry, newbieInode.sfi_direct[0]);
}

void sfs_rmdir(const char* path) {
    int i, j;
    struct sfs_inode cwdInode;
    struct sfs_dir cwd[SFS_DENTRYPERBLOCK];
    // os_shell> rmdir .
    // rmdir: .: Invalid argument
    // os_shell> rmdir ..
    // rmdir: ..: Directory not empty

    // invalid path check
    if (strcmp(path, ".") == 0) {
        error_message("rmdir", path, -8);
        return;
    }
    disk_read(&cwdInode, sd_cwd.sfd_ino);

    bool done = true;
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            // Path check
            if ((cwd[j].sfd_ino != SFS_NOINO) && (strcmp(path, cwd[j].sfd_name) == 0)) {
                struct sfs_inode pathInode;

                disk_read(&pathInode, cwd[j].sfd_ino);
                // Type check
                if (pathInode.sfi_type != SFS_TYPE_DIR) {
                    error_message("rmdir", path, -2);
                    return;
                }
                // Not empty
                if (pathInode.sfi_size != 2 * DIRSIZE) {
                    error_message("rmdir", path, -7);
                    return;
                }
                // FreeBlock and size minus and reset
                freeBlock(cwd[j].sfd_ino);
                cwd[j].sfd_ino = SFS_NOINO;
                cwdInode.sfi_size -= DIRSIZE;
                freeBlock(pathInode.sfi_direct[0]);

                // Write all
                disk_write(&cwdInode, sd_cwd.sfd_ino);
                disk_write(cwd, cwdInode.sfi_direct[i]);
                done = true;
            }
        }
    }
    if (!done) {
        error_message("rmdir", path, -1);
        return;
    }
}

void sfs_rm(const char* path) {
    int i, j, k;
    struct sfs_inode cwdInode;
    struct sfs_dir cwd[SFS_DENTRYPERBLOCK];
    u_int32_t freeCount = 0;

    disk_read(&cwdInode, sd_cwd.sfd_ino);

    bool done = false;
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            // Path check
            if ((cwd[j].sfd_ino != 0) && (strcmp(path, cwd[j].sfd_name) == 0)) {
                struct sfs_inode pathInode;
                disk_read(&pathInode, cwd[j].sfd_ino);
                // Type Check
                if (pathInode.sfi_type != SFS_TYPE_FILE) {
                    error_message("rm", path, -9);
                    return;
                }
                // FreeBlock
                // 1. sfi_direct
                for (k = 0; k < SFS_NDIRECT; k++) {
                    freeBlock(pathInode.sfi_direct[k]);
                }
                // 2. sfi_indirect (IN)
                if (pathInode.sfi_indirect != 0) {
                    // read 128 Block
                    u_int32_t dofid[SFS_DBPERIDB];
                    disk_read(dofid, pathInode.sfi_indirect);
                    for (k = 0; k < SFS_DBPERIDB; k++) {
                        freeBlock(dofid[k]);
                    }
                }
                // 3. sfi_indirect
                freeBlock(pathInode.sfi_indirect);
                // 4. cwd[j].sfd_ino and size minus and reset
                freeBlock(cwd[j].sfd_ino);
                cwd[j].sfd_ino = SFS_NOINO;
                cwdInode.sfi_size -= DIRSIZE;

                disk_write(&cwdInode, sd_cwd.sfd_ino);
                disk_write(cwd, cwdInode.sfi_direct[i]);
                done = true;
            }
        }
    }
    if (!done) {
        error_message("rm", path, -1);
    }
}

void sfs_cpin(const char* local_path, const char* path) {
    int i, j;
    FILE* fp = fopen(path, "rb");
    // 1. path not exist v
    // 2. local_path exist v
    // 3. directory is full v
    // 4. i-node can't be allocated v
    // 5. datablock can't be allocated v
    // 6. path size is over maximum file size v

    // 1. Path check
    if (fp == NULL) {
        error_message("cpin", path, -5);
        return;
    }
    // 6. Size check
    u_int32_t pathSize = getFileSize(fp);
    if (pathSize > fileMaxSize) {
        // os_shell> cpin input_local input
        // cpin: input file size exceeds the max file size
        error_message("cpin", path, -11);
        return;
    }
    struct sfs_inode cwdInode;
    struct sfs_dir cwd[SFS_DENTRYPERBLOCK];
    disk_read(&cwdInode, sd_cwd.sfd_ino);
    u_int32_t size = cwdInode.sfi_size / DIRSIZE;
    u_int32_t maxSize = SFS_NDIRECT * SFS_DENTRYPERBLOCK;
    // 3. directory is full
    if (size >= maxSize) {
        error_message("cpin", path, -3);
        return;
    }
    // 2. Local_path Check
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            if ((cwd[j].sfd_ino != SFS_NOINO) && (strcmp(local_path, cwd[j].sfd_name) == 0)) {
                error_message("cpin", local_path, -6);
                return;
            }
        }
    }
    // Path Check
    // bzero(newbie) and newbie setting
    struct sfs_inode newbieInode;
    struct sfs_dir newbieDEntry[SFS_DENTRYPERBLOCK];
    bzero(&newbieInode, SFS_BLOCKSIZE);
    newbieInode.sfi_size = 0;
    newbieInode.sfi_type = SFS_TYPE_FILE;
    // Get info about empty entry
    bool empty, first;
    empty = first = false;
    u_int32_t blockNumber, entryIndex, directIndex;
    for (i = 0; i < SFS_NDIRECT; i++) {
        if (cwdInode.sfi_direct[i] == 0) {
            first = true;
            break;
        }
        disk_read(cwd, cwdInode.sfi_direct[i]);
        for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
            if (cwd[j].sfd_ino == SFS_NOINO) {
                empty = true;
                break;
            }
        }
        if (empty) {
            break;
        }
        j = 0;
    }
    directIndex = i;
    if (first)
        entryIndex = 0;
    else
        entryIndex = j;
    blockNumber = cwdInode.sfi_direct[directIndex];
    if (empty) {
        disk_read(cwd, blockNumber);
        cwd[entryIndex].sfd_ino = getBlock();
        if (cwd[entryIndex].sfd_ino == false) {
            error_message("cpin", local_path, -4);
            return;
        }
    } else {
        blockNumber = getBlock();
        if (blockNumber == false) {
            error_message("cpin", path, -4);
            return;
        }
        cwdInode.sfi_direct[directIndex] = blockNumber;
        bzero(cwd, SFS_BLOCKSIZE);
        cwd[entryIndex].sfd_ino = getBlock();
        if (cwd[entryIndex].sfd_ino == false) {
            error_message("cpin", local_path, -4);
            return;
        }
    }
    // Size plus
    cwdInode.sfi_size += DIRSIZE;
    // Name assign
    strncpy(cwd[entryIndex].sfd_name, local_path, SFS_NAMELEN);
    // Write all
    disk_write(&cwdInode, sd_cwd.sfd_ino);
    disk_write(cwd, blockNumber);
    disk_write(&newbieInode, cwd[entryIndex].sfd_ino);

    u_int32_t blockNum = cwd[entryIndex].sfd_ino;
    disk_read(&newbieInode, blockNum);  //
    // file reading start
    char buffer[SFS_BLOCKSIZE];
    bool indirectUsed = false;
    u_int32_t readN;                    //
    u_int32_t usedDir, usedIndir;       // used direct and indirect count
    u_int32_t new;                      // new block number
    u_int32_t id, dofid[SFS_DBPERIDB];  // indirect block number
    u_int32_t remain = pathSize;
    usedDir = usedIndir = 0;
    bzero(dofid, SFS_BLOCKSIZE);
    // size_t fread( void *ptr, size_t size, size_t nitems, FILE *stream);
    // void *ptr : 파일 내용을 읽어 들일 메모리 포인터
    // size_t size : 데이터 하나의 크기
    // size_t nitems : 읽어 등일 데이터의 개수
    // FILE *stream 대상 파일 스트림
    // 반환 size_t 읽어들인 데이터 개수를 반환하며,
    // 오류가 발생하면 -1을 반환
    do {
        bzero(buffer, SFS_BLOCKSIZE);
        readN = fread(buffer, SFS_BLOCKSIZE, 1, fp);  // read one block
        if (readN == -1) {
            printf("cpin: %s reading error\n", path);
            return;
        }
        // check : direct is full ?
        if (usedDir < SFS_NDIRECT) {
            // 1. Write in direct
            new = getBlock();
            if (new == false) {
                disk_write(&newbieInode, blockNum);  //
                error_message("cpin", local_path, -4);
                fclose(fp);
                return;
            }
            newbieInode.sfi_size += MIN(remain, SFS_BLOCKSIZE);
            remain -= SFS_BLOCKSIZE;
            newbieInode.sfi_direct[usedDir] = new;
            usedDir++;
            if (pathSize < 0) {
                break;
            }
        } else {
            // 2. Write in indirect
            if (!indirectUsed) {
                // id allocation
                indirectUsed = true;
                id = getBlock();
                if (id == false) {
                    disk_write(&newbieInode, blockNum);  //
                    error_message("cpin", local_path, -4);
                    fclose(fp);
                    return;
                }
                newbieInode.sfi_indirect = id;
            }
            new = getBlock();
            if (new == false) {
                disk_write(&newbieInode, blockNum);  //
                disk_write(dofid, id);
                error_message("cpin", local_path, -4);
                fclose(fp);
                return;
            }
            newbieInode.sfi_size += MIN(remain, SFS_BLOCKSIZE);
            remain -= SFS_BLOCKSIZE;
            dofid[usedIndir] = new;
            usedIndir++;
        }
        disk_write(buffer, new);
    } while (readN != 0);

    fclose(fp);

    disk_write(&newbieInode, blockNum);
    disk_write(dofid, id);
}

u_int32_t getFileSize(FILE* fp) {
    fseek(fp, 0, SEEK_END);  // fp -> end of file
    u_int32_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);  // fp -> start of file
    return size;
}

void cpin_free(u_int32_t index) {
    int i, j;
    for (i = 0; i < index; i++) {
        freeBlock(havetoFree[i]);
    }
}

void sfs_cpout(const char* local_path, const char* path) {
    int i, j, k, l;
    bool exist = true;
    FILE *ftest, *fp;
    ftest = fopen(path, "r");  // return NULL -> not exist
    struct sfs_inode cwdInode, pathInode;
    struct sfs_dir cwd[SFS_DENTRYPERBLOCK];
    char buffer[SFS_BLOCKSIZE];
    bool done = false;
    u_int32_t localpathSize;
    u_int32_t dofid[SFS_DBPERIDB];
    bzero(dofid, SFS_BLOCKSIZE);
    disk_read(&cwdInode, sd_cwd.sfd_ino);

    if (ftest == NULL) {
        for (i = 0; i < SFS_NDIRECT; i++) {
            if (cwdInode.sfi_direct[i] == 0) {
                break;
            }
            disk_read(cwd, cwdInode.sfi_direct[i]);
            for (j = 0; j < SFS_DENTRYPERBLOCK; j++) {
                // Local path exists
                if ((cwd[j].sfd_ino != SFS_NOINO) && (strcmp(local_path, cwd[j].sfd_name) == 0)) {
                    fp = fopen(path, "wb");
                    disk_read(&pathInode, cwd[j].sfd_ino);
                    localpathSize = pathInode.sfi_size;

                    for (k = 0; k < SFS_NDIRECT; k++) {
                        if (pathInode.sfi_direct[k] == 0) {
                            break;
                        }
                        disk_read(buffer, pathInode.sfi_direct[k]);
                        fwrite(buffer, MIN(localpathSize, SFS_BLOCKSIZE), 1, fp);
                        localpathSize -= SFS_BLOCKSIZE;
                    }
                    if (pathInode.sfi_indirect != 0) {
                        disk_read(dofid, pathInode.sfi_indirect);
                        for (l = 0; l < SFS_DBPERIDB; l++) {
                            if (dofid[l] != 0) {
                                disk_read(buffer, dofid[l]);
                                fwrite(buffer, MIN(localpathSize, SFS_BLOCKSIZE), 1, fp);
                                localpathSize -= SFS_BLOCKSIZE;
                            } else {
                                break;
                            }
                        }
                    }
                    done = true;
                    chmod(path, 00700);
                    fclose(fp);
                    break;
                }
            }
            if (done) {
                break;
            }
        }
    }
    if (ftest != NULL) {
        error_message("cpout", path, -6);
        return;
    }
    if (!done) {
        error_message("cpout", local_path, -1);
        return;
    }
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

u_int32_t getBlock() {
    int i, j, k;
    u_int32_t result;
    // bitmap reading
    unsigned char bitMap[bitMapBlockSize][SFS_BLOCKSIZE];
    for (i = 0; i < bitMapBlockSize; i++) {
        disk_read(bitMap[i], SFS_MAP_LOCATION + i);
    }

    for (i = 0; i < bitMapBlockSize; i++) {
        for (j = 0; j < SFS_BLOCKSIZE; j++) {
            for (k = 0; k < CHAR_BIT; k++) {
                result = i * SFS_BLOCKSIZE + j * CHAR_BIT + k;
                if (result > spb.sp_nblocks) {
                    if (DEBUG) printf("getBlock false return.\n");
                    return false;
                }
                //if (DEBUG) printf("BIT_CHECK : %d, k : %d, & op : %d\n", bitMap[i][j], k, BIT_CHECK(bitMap[i][j], k));
                if (BIT_CHECK(bitMap[i][j], k) == 0) {
                    if (DEBUG) printf("getBlock %d return.\n", result);
                    BIT_SET(bitMap[i][j], k);
                    //if (DEBUG) printf("BIT_CHECK : %d, k : %d, & op : %d\n", bitMap[i][j], k, BIT_CHECK(bitMap[i][j], k));
                    disk_write(bitMap[i], SFS_MAP_LOCATION + i);
                    return result;
                }
            }
        }
    }
    return false;
}

void freeBlock(u_int32_t block) {
    int i, j;
    unsigned char bitMap[bitMapBlockSize][SFS_BLOCKSIZE];
    for (i = 0; i < bitMapBlockSize; i++) {
        disk_read(bitMap[i], SFS_MAP_LOCATION + i);
    }
    if (block == 0) {
        return;
    }
    u_int32_t byteIndex = block / CHAR_BIT;
    u_int32_t bitIndex = block % CHAR_BIT;
    u_int32_t blockIndex = byteIndex / SFS_BLOCKSIZE;
    u_int32_t entryIndex = byteIndex % SFS_BLOCKSIZE;

    BIT_CLEAR(bitMap[blockIndex][entryIndex], bitIndex);

    for (i = 0; i < bitMapBlockSize; i++) {
        disk_write(bitMap[i], SFS_MAP_LOCATION + i);
    }
}
