#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

enum { PORT = 80 };

int main(void)
{
    int sock_fd =  socket(PF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        fprintf(stderr, "socket: %s\n", strerror(errno));
        exit(1);
    }
    
    /*
    int sopt = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &sopt, sizeof(sopt));
    setsockopt(sock_fd, SOL_SOCKET, SO_RESUSEPORT, &sopt, sizeof(sopt));
    */
    
    struct sockaddr_in ba;
    ba.sin_family = AF_INET;
    ba.sin_addr.s_addr = INADDR_ANY;
    ba.sin_port = htons(PORT);
    int r = bind(sock_fd, (struct sockaddr *)&ba, sizeof(ba));
    if(r < 0){
        fprintf(stderr, "socket: %s\n", strerror(errno));
        exit(1);
    }
    
    r = listen(sock_fd, 5);
    if(r < 0){
        fprintf(stderr, "socket: %s\n", strerror(errno));
        exit(1);
    }
    
    while(1) {
        struct sockaddr_in aa;
        socklen_t alen = sizeof(aa);
        int status;
        while( waitpid(-1, &status, WNOHANG) > 0){}
        int acc_fd = accept(sock_fd, (struct sockaddr *)&aa, &alen);
        
        FILE *fin = fdopen(dup(acc_fd), "r");
        FILE *fout = fdopen(acc_fd, "w");
        
        int x;
        while(fscanf(fin, "%d", &x) == 1){
            printf("read: %d\n", x);
            fflush(stdout);
            fprintf(fout, "%d\n", x+1);
            fflush(fout);
        }
    }
}
