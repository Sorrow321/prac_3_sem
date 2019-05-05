#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int pid = fork();
    if(!pid) {
        char *args[] = { "/bin/ls", "-l", "--color", "/", 0 };
        execve("/bin/ls", args, NULL);
        _exit(1);
    }
    wait(NULL);
    return 0;
}
