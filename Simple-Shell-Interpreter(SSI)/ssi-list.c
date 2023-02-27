#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "ssi-list.h"

struct bg_pro *new_node(pid_t pid_new, char *command_new){
	
	bg_pro *temp = (bg_pro *)malloc(sizeof(bg_pro));
	
	temp->pid = pid_new;
	strncpy(temp -> command, command_new, MAX_COMMAND_SIZE);	
	temp->next = NULL;
	
	return temp;
}

struct bg_pro *add_end(bg_pro *list, bg_pro *new){
	
	// just a pointer not creating a new struct of bg_pro
	bg_pro *curr;

	if (list == NULL){
		new -> next = NULL;
		return new;
	}
	for (curr = list; curr->next != NULL; curr = curr->next);
	
	// set last item next to new item and then return head
	curr -> next = new;
	new -> next = NULL;
	//printf("%s\n",new ->command);
	return list;

}

struct bg_pro *remove_node(bg_pro *list, pid_t pid_del){
	bg_pro *prev;
	bg_pro *curr;
	
	//printf("%d\n", pid_del);
	if (list == NULL){
		return list;
	}
	for (curr = list; curr->next != NULL;curr = curr -> next){
		// delete this node
		if (pid_del == curr -> pid){
			
			prev -> next = curr -> next;
			free(curr);
			return list;
		}
		prev = curr;
	}
	//return list;
}


