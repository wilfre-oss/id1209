
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>

#define NR_OF_THREADS 3
const int BUFFER_SIZE = 500;
int buffer = 0;
pthread_mutex_t mutex;

void *myThread(void *vargp){

    int count = 0;
    while(buffer < BUFFER_SIZE){
        pthread_mutex_lock(&mutex);

        if(buffer >= BUFFER_SIZE){
            pthread_mutex_unlock(&mutex);
            break;
        }

        printf("TID: %lu, PID: %d, Buffer %d\n", pthread_self(), getpid(), buffer);
        buffer++;
        count ++;
        pthread_mutex_unlock(&mutex);
    }

    int *res = malloc(sizeof(int));
    *res = count;
    pthread_exit((void*) res);

}

int main(){

    pthread_t tid[NR_OF_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < NR_OF_THREADS; i++){
        if(pthread_create(tid + i, NULL, &myThread, NULL) != 0){
            perror("Failed to create thread(s)");
            return 1;
        }
    }

    int *res;
    for(int i = 0; i < NR_OF_THREADS; i++){
        pthread_join(tid[i], (void**)&res);
        printf("TID: %lu worked on the buffer %d times\n", tid[i], *res);
        free(res);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}

