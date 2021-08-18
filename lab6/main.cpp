#include <stdlib.h>     /* NULL */
#include  <stdio.h>    /* printf */
#include  <sys/types.h>    /* pid_t */
#include <unistd.h>    /* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h>     /* wait */
#include <pthread.h>

#define PRODUCER_NO 5    //Number of producers
#define NUM_PRODUCED 2000 //Number of items to be produced
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;
void *generator_function(void *arg);
void *print_function(void *arg);
long sum; /* Sum of generated values*/
long finished_producers; /* number of the producer that finished producing */

//C: Mutex declaration and initialization
//F: Condition variable declaration and initialization

int main(void) {
    /* initialize random seed: */
    pthread_t generatorThread[5];
    pthread_t printThread[1];
    int i;
    srand(time(NULL));
    sum = 0;
    //A: Creates five generator thread
    for(i=0;i<5;i++){
        pthread_create(&generatorThread[i], NULL, &generator_function, NULL);
        }
    //D: Creates print thread
    pthread_create(&printThread[0], NULL, &print_function, NULL);
    //B: Makes sure that all generator threads has finished before proceeding
    for(i=0; i != 4; i++){
            pthread_join(generatorThread[i], NULL);
        }
    pthread_join(printThread[0], NULL);
   // print_function();
    //E: Makes sure that print thread has finished before proceeding
    return (0);

}

void *generator_function(void *arg) {

    long counter = 0;
    long sum_this_generator = 0;

    while (counter < 2000L) {
pthread_mutex_lock(&mutex1);
        long tmpNumber = sum;
        long rnd_number = rand() % 10;
        printf("current sum of the generated number up to now is %ld going to add %ld to it.\n", tmpNumber, rnd_number);
        sum = tmpNumber + rnd_number;
        counter++;
        sum_this_generator += rnd_number;
        pthread_mutex_unlock(&mutex1);
        usleep(1000);
    }
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    printf("The sum of produced items for this number generator at the end is: %ld \n", sum_this_generator);
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    finished_producers++;
    //H: If all generator has finished fire signal for condition variable
    while(1) {
        if(finished_producers == 5){
    pthread_cond_signal(&condition_cond);
    }
    break;
}
    return (0);
}

void *print_function(void *arg) {
    //G: Wait until all generator has finished
    while(finished_producers !=5){
        pthread_cond_wait(&condition_cond, &mutex1);
        }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("The value of counter at the end is: %ld \n", sum);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}