#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "hash.h"

// Global constants
#define SAMPLES_TO_COLLECT   10000000
#define RAND_NUM_UPPER_BOUND   100000
#define NUM_SEED_STREAMS            4

team_t team = {
    "LOL",                  /* Team name */
    "Zexuan Wang",                    /* First member full name */
    "998851773",                 /* First member student number */
    "zexuan.wang@mail.utoronto.ca",                 /* First member email address */
    "Dropped the Course",                           /* Second member full name */
    "N/A",                           /* Second member student number */
    "N/A"                            /* Second member email address */
};

// Global variables
class sample;
hash<sample,unsigned> h;    // Key is 'unsigned' and element is 'class sample'
unsigned num_threads;
unsigned samples_to_skip;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

class sample {
    unsigned my_key;
    public:
    sample *next;
    unsigned count;

    sample(unsigned the_key){my_key = the_key; count = 0;};
    unsigned key(){return my_key;}
    void print(FILE *f){printf("%d %d\n",my_key,count);}
};

// simple struct that holds the parameters passed into count_frequencies() function.
struct parameters {
    int start;
    int end;
};

void* count_frequencies(void *args){
    int i,j,k;
    int rnum;
    unsigned key;
    sample *s;
    parameters *p = (parameters*) args;

    // process streams starting with different initial numbers
    for (i=p->start; i<p->end; i++){
        rnum = i;
        // collect a number of samples
        for (j=0; j<SAMPLES_TO_COLLECT; j++){
            // skip a number of samples
            for (k=0; k<samples_to_skip; k++){
                // rand_r takes in a random seed value, in this case, memory address of stream value.
                rnum = rand_r((unsigned int*)&rnum);
            }
            // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
            key = rnum % RAND_NUM_UPPER_BOUND;

            // ++++++ critial section starts +++++++
            pthread_mutex_lock(&mutex);
            // if this sample has not been counted before
            if (!(s = h.lookup(key))){
                // insert a new element for it into the hash table
                s = new sample(key);
                h.insert(s);
            }
            // increment the count for the sample
            s->count++;
            pthread_mutex_unlock(&mutex);
            // ++++++ critial section ends +++++++
        }
    }

}


// Main Function
int main (int argc, char* argv[]){

    // Print out team information
    printf( "Team Name: %s\n", team.team );
    printf( "\n" );
    printf( "Student 1 Name: %s\n", team.name1 );
    printf( "Student 1 Student Number: %s\n", team.number1 );
    printf( "Student 1 Email: %s\n", team.email1 );
    printf( "\n" );
    printf( "Student 2 Name: %s\n", team.name2 );
    printf( "Student 2 Student Number: %s\n", team.number2 );
    printf( "Student 2 Email: %s\n", team.email2 );
    printf( "\n" );

    // Parse program arguments
    if (argc != 3){
        printf("Usage: %s <num_threads> <samples_to_skip>\n", argv[0]);
        exit(1);
    }
    sscanf(argv[1], " %d", &num_threads); // not used in this single-threaded version
    sscanf(argv[2], " %d", &samples_to_skip);

    // initialize a 16K-entry (2**14) hash of empty lists
    h.setup(14);

    // Create the threads based on the value of num_threads
    int i, start = 0, step_size = 4/num_threads;
    pthread_t threads[num_threads];
    parameters *p = new parameters[num_threads];    // array of parameters where each thread only uses one element.
    for (i=0; i< num_threads; i++){
        p[i].start = start;
        p[i].end = start+step_size;
        pthread_create(&threads[i], NULL, count_frequencies, (void*) &p[i]);
        start+=step_size;
    }
    for (i=0; i<num_threads; i++){
        pthread_join(threads[i],NULL);
    }

    // print a list of the frequency of all samples
    h.print();
}
