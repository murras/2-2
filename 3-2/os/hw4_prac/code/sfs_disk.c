#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sfs_disk.h"
#include "sfs_types.h"

#define BLOCKSIZE 512

#ifndef EINTR
#define EINTR 0
#endif

static int fd = -1;

void disk_open(const char *path) {
    assert(fd < 0);
    fd = open(path, O_RDWR);

    if (fd < 0) {
        err(1, "%s", path);
    }
}

u_int32_t disk_blocksize(void) {
    assert(fd >= 0);
    return BLOCKSIZE;
}

void disk_write(const void *data, u_int32_t block) {
    const char *cdata = data;
    u_int32_t tot = 0;
    int len;

    assert(fd >= 0);

    if (lseek(fd, block * BLOCKSIZE, SEEK_SET) < 0) {
        err(1, "lseek");
    }

    while (tot < BLOCKSIZE) {
        len = write(fd, cdata + tot, BLOCKSIZE - tot);
        if (len < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            err(1, "write");
        }
        if (len == 0) {
            err(1, "write returned 0?");
        }
        tot += len;
    }
}

void disk_read(void *data, u_int32_t block) {
    char *cdata = data;
    u_int32_t tot = 0;
    int len;

    assert(fd >= 0);

    if (lseek(fd, block * BLOCKSIZE, SEEK_SET) < 0) {
        err(1, "lseek");
    }

    while (tot < BLOCKSIZE) {
        len = read(fd, cdata + tot, BLOCKSIZE - tot);
        if (len < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            err(1, "read");
        }
        if (len == 0) {
            err(1, "unexpected EOF in mid-sector");
        }
        tot += len;
    }
}

void disk_close(void) {
    assert(fd >= 0);
    if (close(fd)) {
        err(1, "close");
    }
    fd = -1;
}
