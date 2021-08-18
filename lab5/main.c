#include <stdio.h> /* printf */
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h>  /* wait */
#include <pthread.h>
#include <sys/stat.h>



void *counterFunction();
long globalNumber;
//Create a mutex
pthread_mutex_t mutex_stdout = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
    globalNumber = 0;
    int i;
    pthread_t counterThread[5];
    int iret[5];


    //Create the 5 threads
    for (i = 0; i < 5; i++) {
        iret[i] = pthread_create(&counterThread[i], NULL, counterFunction, NULL);
    }
    
    for (i = 0; i < 5; i++) {
        pthread_join(counterThread[i], NULL);
    }
    //Wait for all treads to finish
    //YOU NEED TO INSERT THIS CODE!
   

    printf("End globalNumber:: %d \n", globalNumber);
    return (0);
}

void *counterFunction() {

    long loopCounter = 0;
    //C
    /* Initialize pid and thread_id
     * to the correct values.
     */
    pid_t pid = getpid();
    int thread_id = pthread_self();

    printf("pid: %d, thread id: %d\n", pid, thread_id);
    while (loopCounter < 2000L) {

      pthread_mutex_lock(&mutex_stdout);
        long tmpNumber = globalNumber;
        tmpNumber++;
        globalNumber = tmpNumber;
      pthread_mutex_unlock(&mutex_stdout);
        loopCounter++;
        usleep(1000); //Creates a delay 
    }
    return (0);
}
