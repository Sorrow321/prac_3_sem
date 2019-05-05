#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void find_data(int file, off_t offset)
{
    lseek(file, offset, SEEK_SET);
    struct Node node;
    ssize_t r = read(file, &node, sizeof(node));
    if(r != sizeof(node)) {
        return;
    }
    if(node.right_idx) {
        find_data(file, sizeof(node) * node.right_idx);
    }
    printf("%"PRId32" ", node.key);
    
    if(node.left_idx) {
        find_data(file, sizeof(node) * node.left_idx);
    }
}

int main(int argc, char *argv[])
{
    int file = open(argv[1], O_RDONLY, 0600);
    if(file == -1) {
        return 1;
    }
    find_data(file, 0);
    printf("\n");
    close(file);
    return 0;
}
