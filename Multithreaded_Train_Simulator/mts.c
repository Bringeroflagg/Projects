#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h> 
#include "priority_queue.h"


// 
train_struct *ts_head, *ts_curr;
int unique_id = 0;

// Global dispatcher and global mutex for signal to cross train.
pthread_t dispatcher_thread;
pthread_mutex_t train_cross;
// global timer, and train_ready for all threads
pthread_mutex_t start_timer;
pthread_cond_t train_ready_to_load;

// global time variables
long t_s_s = 0;
long t_s_ns = 0;
float t_s = 0.0;
int mins_elapsed = 0;

float mins(float num){
    if (num > 60) {
        mins_elapsed++;
        return(num - 60);
    }
    return num;
}

// Train method will wait for all trains ready to load signal, print ready,On,and OFF.
// Will wait for signal from dispatcher and return to dispatcher after completing crossing.
void* train(void *t){
    
    // Cast a pointer from void to a struct train_struct pointer
    train_struct *train = (struct train_struct*) t;
    struct timespec time1 = {0};
    
    // Broadcast, so everything begins at the same time
    pthread_mutex_lock(&start_timer);
    pthread_cond_wait(&train_ready_to_load, &start_timer);
    pthread_mutex_unlock(&start_timer);    

    int w = pthread_cond_init( &(train->granted), NULL);
    // Once sleep is compeleted then go to 
    usleep( 100000 * (train -> loading_time) );
    //printf("------------(%d:loading time) ------ (%d:crossing time)\n", train->loading_time,train->crossing_time);

    

    clock_gettime(CLOCK_MONOTONIC, &time1);
    long t_t_s = time1.tv_sec;
    long t_t_ns = time1.tv_nsec;
    float t_t = t_t_s+t_t_ns/1000000000;

    // check the direction its going in
    if( strncmp( "e", train ->command,1 ) == 0 || strncmp( "E", train ->command,1 ) == 0){
        if((t_t_ns/100000000) > 1){
            printf("00:0%d:%02.f.%01.ld Train %2d is ready to go %4s\n", mins_elapsed,(t_t_s - t_s),(t_t_ns/100000000), train ->u_id, "EAST", train->loading_time);
        }else{
            printf("00:0%d:%02.f.0 Train %2d is ready to go %4s\n", mins_elapsed,(t_t_s - t_s), train ->u_id, "EAST", train->loading_time);
        }
    }else{
         if( (t_t_ns/100000000) > 1 ){
            printf("00:0%d:%02.f.%01.ld Train %2d is ready to go %4s\n", mins_elapsed,(t_t_s - t_s),(t_t_ns)/100000000, train ->u_id, "WEST", train->loading_time);
         }else{
            printf("00:0%d:%02.f.0 Train %2d is ready to go %4s\n", mins_elapsed,(t_t_s - t_s), train ->u_id, "WEST", train->loading_time);
        }
    }
    add_to_priority_queue(train);
    // create mutex lock for when waiting for dispatcher
    pthread_mutex_lock( &train_cross );
    // cond granted in struct, global train_tracks mutex
    pthread_cond_wait( &(train->granted), &train_cross );
    pthread_mutex_unlock( &train_cross );

    clock_gettime(CLOCK_MONOTONIC, &time1);
    t_t_s = time1.tv_sec;
    t_t_ns = time1.tv_nsec;

    if( strncmp( "e", train ->command,1 ) == 0 || strncmp( "E", train ->command,1 ) == 0){
         if((t_t_ns/100000000) > 1){
            printf("00:0%d:%02.f.%01.ld Train %2d is ON the main track going %4s\n",mins_elapsed,t_t_s - t_s,(t_t_ns/100000000), train ->u_id, "East",train->crossing_time);
         }else{
            printf("00:0%d:%02.f.0 Train %2d is ON the main track going %4s\n",mins_elapsed,t_t_s - t_s, train ->u_id, "East",train->crossing_time);
         }
    }else{
        if((t_t_ns/100000000) > 1){
            printf("00:0%d:%02.f.%01.ld Train %2d is ON the main track going %4s\n",mins_elapsed,t_t_s - t_s,(t_t_ns/100000000), train ->u_id, "West",train->crossing_time);
        }else{
            printf("00:0%d:%02.f.0 Train %2d is ON the main track going %4s\n",mins_elapsed,t_t_s - t_s, train ->u_id, "West",train->crossing_time);
         }
    }
    
    usleep( 100000 * train->crossing_time );

    clock_gettime(CLOCK_MONOTONIC, &time1);
    t_t_s = time1.tv_sec;
    t_t_ns = time1.tv_nsec;

    if( strncmp( "e", train ->command,1 ) == 0 || strncmp( "E", train ->command,1 ) == 0){
        if((t_t_ns/100000000) > 1){
            printf("00:0%d:%02.f.%01.ld Train %2d is OFF the main track after going %4s\n",mins_elapsed, t_t_s - t_s,(t_t_ns/100000000), train ->u_id, "East");
        }else{
            printf("00:0%d:%02.f.0 Train %2d is OFF the main track after going %4s\n",mins_elapsed, t_t_s - t_s, train ->u_id, "East");
        }
    }else{
        if((t_t_ns/100000000) > 1){
            printf("00:0%d:%02.f.%01.ld Train %2d is OFF the main track after going %4s\n",mins_elapsed, t_t_s - t_s,(t_t_ns/100000000), train ->u_id, "West");
        }else{
            printf("00:0%d:%02.f.0 Train %2d is OFF the main track after going %4s\n",mins_elapsed, t_t_s - t_s, train ->u_id, "West");
        }
    }
    // pthread_join(&dispatcher thread, NULL);
    //printf("Yeetus please workus\n");
    pthread_exit(NULL);
}

// Releases all trains, sends signal to each thread when it can cross.
void dispatcher(){
    // while something in return list_head send signal to train, and wait for response
    train_struct *dispatcher_head, *dispatcher_curr;
    
    // create and initialize pthread cond for train_cross
    train_struct *ts_curr;

    int current_id = 0;
    int unique_id_dispatcher = unique_id;
    
    struct timespec time1 = {0};
    
    // once at dispatcher then release all trains and broadcast
    int ret = pthread_mutex_init(&train_cross, NULL);
    
    
    pthread_cond_broadcast(&train_ready_to_load);
    // begin global time
    clock_gettime(CLOCK_MONOTONIC, &time1);
	t_s_s = time1.tv_sec;
	t_s_ns = time1.tv_nsec;
	t_s = t_s_s+t_s_ns/1000000000;

    // While there are elements left keep checking linked list
    while(unique_id_dispatcher > 0){
        dispatcher_head = return_head();

        // Keep signaling trains to cross until nothing remains in linked list.
        if(dispatcher_head != NULL){
            //dispatcher_curr = dispatcher_head ->next;
            pthread_cond_signal( &(dispatcher_head->granted) );
            //printf("waiting\n");
            pthread_join( dispatcher_head->train_thread,NULL );
            //printf("received\n");

            unique_id_dispatcher--;
            del_node(dispatcher_head->u_id);
            //printf("%d\n",dispatcher_head->u_id);
        }
        
    }
    // Destroy train cross mutex
    pthread_mutex_destroy(&train_cross);
    // Return to main
    pthread_exit(NULL);
}

// Main will recieve file name, scan file to create new nodes, create train threads and create dispatcher thread
// Then will wait until dispatcher thread exits out
int main(int argc, char *argv[]){
    
    // Check for number of arguments and open file
    if (argc != 2) return printf("Invalid Number Of Arguments.\n");

    // Check if file was able to open successfully
    FILE *mainFile = fopen(argv[1], "r");
    if(mainFile == NULL) {
        fclose( mainFile );
        exit( 10 );
    }
    
    //direction_buffer = (char *)malloc(3 * sizeof(char));
    char buffer[2];
    int load, cross;
    // [row->][coloumn]
    //int loading, crossing_time;
    char* buffer_direction;

    // intitalize pthread mutex for priority queue
    create_queue_mutex();

    // returns -1 if did not work
    while( fscanf(mainFile ,"%s %d %d", buffer, &load, &cross) != -1 ){
        // create new node and check if high priority or low priority; 1 == high, 0 == low
        if( strcmp("W", buffer) == 0 || strcmp("E", buffer) == 0 ){
        	ts_curr = new_node(buffer, load, cross, unique_id++, 1);
        }else{
        	ts_curr = new_node(buffer, load, cross, unique_id++, 0);
	}
        //printf("%d %d--ID %d ", load,cross,unique_id);
        // create thread here
        pthread_create( &(ts_curr -> train_thread), NULL, train, (void *)(ts_curr) );
        
    }
    fclose( mainFile );

    // print list
    /*
   for( ts_curr = return_head(); ts_curr != NULL; ts_curr = ts_curr->next ){
     printf( "%d)%s %d %d (%d-input_num) %d\n",++ret ,ts_curr ->command, ts_curr->loading_time, ts_curr ->crossing_time, ts_curr ->u_id, ts_curr ->priority );
    }
    */

    // after done creating all threads, create a dispatcher thread and wait for it.
    pthread_create( &dispatcher_thread, NULL, dispatcher, NULL );
    pthread_join(dispatcher_thread,NULL);

    // destroying queue mutex
    destroy_queue_mutex();
    exit( 1 );
}