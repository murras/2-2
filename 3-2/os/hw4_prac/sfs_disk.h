#ifndef _SFS_DISK_H_
#define _SFS_DISK_H_

void disk_open(const char *path);

u_int32_t disk_blocksize(void);

void disk_write(const void *data, u_int32_t block);
void disk_read(void *data, u_int32_t block);

void disk_close(void);

#endif /*_SFS_DISK_H_*/
