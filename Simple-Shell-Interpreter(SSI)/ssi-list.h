#define MAX_COMMAND_SIZE 1024


typedef struct bg_pro{
	pid_t pid;
	char command[MAX_COMMAND_SIZE];
	struct bg_pro *next;

} bg_pro;

bg_pro *add_end(bg_pro *list, bg_pro *new);
bg_pro *new_node(pid_t pid_new, char *command_new);
bg_pro *remove_node(bg_pro *list, pid_t pid_del);
