#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#include "priority_queue.h"

// Use this for return the head of the list
train_struct *main_head;

// Mutex to ensure 2 threads cannot occupy same code area
pthread_mutex_t priority_mutex;
pthread_mutex_t del_node_mutex;

// Return head of linked list
struct train_struct *return_head(){
	usleep(10);
	return main_head;
}

// Create a new train struct and store its values
struct train_struct *new_node(char *command_new, int load, int cross, int unique_id, int priority_n){
	
	train_struct *temp = (train_struct *)malloc(sizeof(train_struct));
	//printf("%d -- ",load);
	strncpy(temp -> command, command_new, MAX_DIRECTION_SIZE);	
    temp -> loading_time = load;
    temp -> crossing_time = cross;
	temp -> u_id = unique_id;
	temp -> priority = priority_n;
	//printf("->%s %d %d--ID %d ----- ", temp->command,temp -> loading_time,temp -> crossing_time,unique_id);
	//printf("->%s %d %d--ID %d\n", command_new,load,cross,unique_id);
	// create init pthread thread and granted here
	//pthread_init(&(temp -> train_thread), NULL);
	//pthread_cond_init(&(temp -> granted),NULL);
	temp->next = NULL;
	return temp;
}

// Add train according to priority
void add_to_priority_queue( train_struct *new){
	
	// Have this to protect the lists integrity from other threads executing at the same time
	pthread_mutex_lock(&priority_mutex);
	//printf("lock - ");
	// just a pointer not creating a new struct of add_to_priority_queue
	train_struct *prev, *curr;
	// Check if any items in list
	if (main_head == NULL){
		new -> next = NULL;
		main_head = new;
		//return main_head;

	// if new item is priority low then add at end
	}else if(new -> priority == 0){
		
		for (curr = main_head; curr->next != NULL; curr = curr->next);
		// set last item next to new item and then return head
		curr -> next = new;
		new -> next = NULL;

	// if priority == 0; go to end of list, if 1; then go until start of low
	}else if (new -> priority == 1){
		curr = main_head;
		// edge case : if the current priority is low and new is high, add high to the start
		if(curr -> priority == 0){
			new -> next = main_head;
			main_head = new;
		}else{
		
		// go to the end of high priority	
		while(curr -> priority == 1 && curr -> next != NULL){
			prev = curr;
			curr = curr -> next;
		}
		// check if first element is lower case
			new -> next = curr;
			prev -> next = new;
		}
	}
	//printf(" unlock\n");
	pthread_mutex_unlock(&priority_mutex);
}

// Delete node in priority linked list by input number
void del_node(int unique_id){

	//pthread_mutex_lock(&del_node_mutex);
	train_struct *prev1, *curr1;
	curr1 = main_head;
	
	// if head node delete
	if(main_head->u_id == unique_id){
		main_head = main_head -> next;
		free(curr1);
	
	// else traverse list and find unique_id and delete
	}else{
		while(curr1->next != NULL && curr1->u_id != unique_id){
			prev1 = curr1;
			curr1 = curr1->next;
		}
		prev1->next = curr1->next;
		free(curr1);
	}
	//pthread_mutex_unlock(&del_node_mutex);
}

// Create and destroy methods for mutex to lock priority queue, so it maintains its integrity
void create_queue_mutex(){
    pthread_mutex_init(&priority_mutex, NULL);
	pthread_mutex_init(&del_node_mutex, NULL);

}
void destroy_queue_mutex(){
	pthread_mutex_destroy(&priority_mutex);
	pthread_mutex_destroy(&del_node_mutex);
}