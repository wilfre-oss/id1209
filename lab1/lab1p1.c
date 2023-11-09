#include <stdio.h>
#include <sys/wait.h>
#include "unistd.h"

int main() {
    char cmd[] = "ls / | wc -l";
    int fd[2];
    if(pipe(fd) == -1){
        printf("Pipe error\n");
        return 1;
    }

    int pid = fork();
    if(pid < 0){
        return 2;
    }
    if(pid == 0){
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execlp("ls", "ls", "/", NULL);
    }
    wait(NULL);
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    execlp("wc", "wc", "-l", NULL);

    return 0;
}
