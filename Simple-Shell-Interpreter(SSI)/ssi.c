#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "ssi-list.h"

#define BUFFER_SIZE 1024


void print_cwd(){
	char *CWD;
	CWD = (char*) malloc(128*sizeof(char));
	printf("%s", getcwd(CWD,128));
	free(CWD);
	}

void execute_command(char *arg_list[]){
	//example -- > arg_list[] = {"ls","-l",NULL};
	/*
	for (int i = 0; arg_list[i] != NULL; i++){
		printf("%s %s ",arg_list[i], arg_list[i+1] );
	}
	*/
	int status_code = execvp(arg_list[0],arg_list);
	exit(1);
}
void sleep_command(int x){
	sleep(x);
	//printf("%d slept this long\n", x);
}

int main(int argc, char *argv[]){
	char *user_id, hostname[BUFFER_SIZE];
	user_id = getlogin();
	gethostname(hostname,BUFFER_SIZE);
	bg_pro *bg_head,*bg_curr; 

	// i for array, pid for process id, and bg_processes for the number of bg processes running
	int i, pid, bg_processes = 0;
	char input_str[BUFFER_SIZE];
	char* args[BUFFER_SIZE];
	int do_loop;
	
	while(1){
		printf("%s@%s: ", user_id, hostname);
		print_cwd();
		printf(" >");

		fgets(input_str,BUFFER_SIZE,stdin);
		// ex - ls -l, args[0] holds ls now, so go to if else to find the match
		args[0] = strtok(input_str, " \n");	

		if ((strcmp("exit", args[0]) == 0)){
			exit(1);
		}else if(strcmp("bg",args[0]) == 0){
			
			// pid = fork(), if parent head = add_end(head,new_node(head))
			pid = fork();
			// in child
			if(pid == 0){
				// left shift by 3 and done
				// strtok and do command
				int temp_int = 0;
				while(args[temp_int] != NULL){
					args[temp_int + 1] = strtok(NULL," \n");
					temp_int++;
				}
				
				char *new_arr[1024];
				temp_int = 1;

				while(args[temp_int] != NULL){
					new_arr[temp_int - 1] = args[temp_int];
					temp_int++;			
				}
				execvp(new_arr[0], new_arr);
				
			// in parent
			}else{
				args[1] = strtok(input_str, " \n");
				bg_processes += 1;
				bg_curr = new_node(pid,args[1]);
				bg_head = add_end(bg_head, bg_curr);
				printf("%d : %s\nTotal Background processes: %d\n", bg_curr -> pid, bg_curr -> command, bg_processes);
			}
		}
		if(!strcmp("cd",args[0])){
			args[1] = strtok(NULL, " \n");

			if (! strcmp(args[1],"~") || args[1] == NULL){
				chdir(getenv("HOME"));
			}else if ( !strcmp(args[1], "..")){
				chdir("..");
			}else{
				chdir(args[1]);
			}			
		
		// bglist checks if any process returned from waitpid, if so remove from list.	
		}else if (strcmp(args[0], "bglist") == 0){

			do_loop = bg_processes;
			// do_loop as many times as the number of linked list created; check ter
			// returns a child process meaning that child has exited, then check if head
			// if head then remove child, otherwise loop.
			while (do_loop > 0){
				if(bg_processes > 0){
				pid_t ter = waitpid(0,NULL,WNOHANG);
				// any child terminates
				if (ter > 0){
					if (bg_head -> pid == ter){
						printf("Background process %d exited\n", bg_head -> pid);
						remove_node(bg_head, ter);
						bg_processes -=1;
						bg_curr = NULL;
					}else{
						bg_curr = bg_head;
						while (bg_curr != NULL){
							if (bg_curr -> pid == ter){
								printf("%d:%s has terminated.\n",bg_head -> pid, bg_head -> command);
								bg_head = remove_node(bg_head, ter);
								bg_processes -=1;
								bg_curr = NULL;
							}else{		
								bg_curr = bg_curr -> next;
								}	
							}
						}					
				}
			do_loop -=1;
				}

			}
			if (bg_processes > 0){
				int x1 = 1;
				// print pid: cwd and queue number
				bg_curr = bg_head;
				while(bg_curr != NULL){
					printf("%d: ", bg_curr->pid);
					print_cwd();
					printf(" %d\n",x1);
					bg_curr = bg_curr -> next;
					x1++;
				}
				printf("Total background jobs: %d\n\n", bg_processes);
			
				// print pid and command
				x1 = 1;
				bg_curr = bg_head;
				while(bg_curr != NULL){
					printf("%d: %s\n",bg_curr -> pid, bg_curr -> command);
					bg_curr = bg_curr -> next;
				}
				printf("Total background jobs: %d\n", bg_processes);
			}else{
				printf("Total background jobs: %d\n", bg_processes);
			}

		}else{	
			//put all commands in args
			while(args[i] != NULL){
				args[i+1] = strtok(NULL, " \n");
				i++;
			}
			pid = fork();
			if (pid == 0){
				// in child process
				execute_command(args);
			}else{
				// In parent process
				wait(NULL);
		}
		i = 0;
		}
	}
}
