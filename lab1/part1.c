#include <stdio.h>
#include <sys/wait.h>
#include "unistd.h"

/* This code was inspired by: https://www.youtube.com/watch?v=6xbLgZpOBi8 and lecture slides*/

int main() {
    // Define file descriptor
    int fd[2];

    // Open pipe
    if(pipe(fd) == -1){
        printf("Pipe error\n");
        return 1;
    }
    
    // Create a child process
    int pid = fork();
    if(pid < 0){
        printf("Fork error\n");
        return 2;
    }
    
    //Child procces links pipe end to STDOUT and execute "ls /"
    if(pid == 0){
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execlp("ls", "ls", "/", NULL);
    }
    
    //Parent procces waits for child to execute "ls" and terminate
    wait(NULL);

    //Link STDIN to the pipe and execute "wc -l"
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    execlp("wc", "wc", "-l", NULL);

    return 0;
}