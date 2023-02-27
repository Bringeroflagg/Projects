#define MAX_DIRECTION_SIZE 2

// Template for train struct
typedef struct train_struct{

    pthread_t train_thread;
	// signal the granted
	pthread_cond_t granted; 

	char command[MAX_DIRECTION_SIZE];
    int loading_time;
    int crossing_time;
    // 1 == high, 0 == low
    int u_id;
    int priority;
    int time_elasped;
	struct train_struct *next;

} train_struct;

void create_queue_mutex();
void destroy_queue_mutex();
train_struct *return_head();
train_struct *new_node(char *command_new, int load, int cross, int unique_id, int priority_u);
void add_to_priority_queue( train_struct *new);
void del_node(int unique_id);