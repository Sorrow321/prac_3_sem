#include <unistd.h>
#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    OWNER_BYTES = 6,
    GROUP_BYTES = 3,
    ALL_FLAGS = 0
};

static int compare_bytes(int b_Have, int b_Want)
{
    if((b_Have & (b_Have ^ b_Want)) == (b_Have ^ b_Want)) {
        return 1;
    }
    return 0;
}

int has_group(gid_t gid, unsigned *groups, int num)
{
    for(int i = 0; i < num; i++){
        if(gid == groups[i]){
            return 1;
        }
    }
    return 0;
}

int myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if(task -> uid == 0) {
        return 1;
    }
    
    if(stb -> st_uid == task -> uid) {
        if(compare_bytes(stb -> st_mode >> OWNER_BYTES, access)) {
            return 1;
        }
        return 0;
    }
    
    if(has_group(stb -> st_gid, task -> gids, task -> gid_count)){
        if(compare_bytes(stb -> st_mode >> GROUP_BYTES, access)) {
            return 1;
        }
        return 0;
    }
    
    if(compare_bytes(stb -> st_mode >> ALL_FLAGS, access)) {
        return 1;
    }
    return 0;
}
