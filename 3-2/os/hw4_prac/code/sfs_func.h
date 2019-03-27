#ifndef _SFS_FUNC_H_
#define _SFS_FUNC_H_

void sfs_mount(const char* path);
void sfs_umount();
void sfs_ls(const char* path);
void sfs_cd(const char* path);

void sfs_mkdir(const char* path);
void sfs_rmdir(const char* path);
void sfs_touch(const char* path);
void sfs_rm(const char* path);
void sfs_mv(const char* src_name, const char* dst_name);
void sfs_dump();
void sfs_fsck();
void sfs_bitmap();

void sfs_cpin(const char* local_path, const char* path);
void sfs_cpout(const char* path, const char* local_path);

#endif /*_SFS_FUNC_H_*/
