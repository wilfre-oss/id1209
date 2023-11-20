#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/wait.h"

/* This code was inspired by: https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/MQueues.html */

int main(){

    // Define buffer size and allocate buffer
    int BUFFER_SIZE = 8192;
    char *buf = calloc(BUFFER_SIZE, 1);
    int file, rd;

    //Create a child proccess that executes the consumer
    int pid = fork();
    if(pid == 0){
        if(execl("receive", "receive", NULL) == -1){
            perror("execl error");
        }
    }

    // Open a file and read its content into a buffer
    file = open("message.txt", O_RDONLY, 0600);
    rd = read(file, buf, BUFFER_SIZE);
    
    printf("Producer: Opening Message Queue\n");
    char *my_mq = "/mymq";
    char *message = buf;
    
    // Open the message queue
    mqd_t mqd = mq_open(my_mq, O_CREAT | O_WRONLY, 0600, NULL);
    if(mqd == -1){
        perror("mq_open failed!\n");
        return(1);
    }
    
    // Send the message to the message queue
    mq_send(mqd, message, strlen(message), 10);

    // Close the message queue
    mq_close(mqd);
    printf("Producer: Message sent\n");

    // Waits for the child procces to count the words of the message before closing
    wait(NULL);
    return 0;
}