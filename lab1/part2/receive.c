#include <fcntl.h>
#include "sys/stat.h"
#include "mqueue.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* This code was inspired by: https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/MQueues.html */

/* This function counts the number of words of the argument
 */
int count_words(char *str){
    int words = 1;

    char *ptr = str;

    // Counts the number of words until the NULL byte in the string.
    while(*(ptr) != '\0'){
        if(*(ptr) == ' '){
            words++;
        }
        ptr++;
    }

    printf("Consumer: Word count: %d\n", words);

    return 0;
}

int main(){
    char *my_mq = "/mymq";

    // Message queue opened
    mqd_t mqd = mq_open(my_mq, O_RDONLY | O_NONBLOCK);
    if(mqd == -1){
        perror("mq_open failed!\n");
        return 1;
    }
    
    struct mq_attr attr;
    
    // Get attributes of the message queue
    if(mq_getattr(mqd, &attr) == -1){
        printf("Consumer: Failed to get attributes\n");
    }
    
    // Define and allocate buffer
    char *buf = calloc(attr.mq_msgsize, 1);
    unsigned int prio = 0;
    
    // Get the message from the message queue
    if((mq_receive(mqd, buf, attr.mq_msgsize, &prio)) == -1){
        printf("Consumer: Failed to get message\n");
    }
    else {
        printf("Consumer: Received message: %s\n", buf);
    }

    // Count the number of words in the message
    if(strlen(buf) != 0){
        count_words(buf);
    }else{
        printf("Consumer: Word count: 0");
    }
    
    // Free buffer and close message queue
    free(buf);
    buf = NULL;
    mq_close(mqd);


    return 0;
}