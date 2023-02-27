/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"

// used to create exception linked list and sentence linked list
node_t *new_node(char *text) {
    assert( text != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));
    temp->text = strdup(text);
    //strncpy(temp->text, text, LIST_MAXLEN_TEXT);
    temp->next = NULL;

    return temp;
}

// makes new struct for word which holds the word and line number
node_w *new_w(char *word_w, int line_w){
    // have word, line number
    assert(word_w != NULL);

    node_w *temp = (node_w *)emalloc(sizeof(node_w));
    //strncpy(temp->word, word_w, LIST_MAXLEN_TEXT);
    temp -> word = strdup(word_w);
    temp -> line_number = line_w;
    temp -> next = NULL;    
    return temp;
}

node_t *add_front(node_t *list, node_t *new) {
    new->next = list;
    return new;
}


node_t *add_end(node_t *list, node_t *new) {
    node_t *curr;

    if (list == NULL) {
        new->next = NULL;
        return new;
    }
    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}
// use this for node containing word and line number
node_w *add_inorder(node_w *list,node_w *new){
    // check if node exists if not then, then continue,otherwise return list
    node_w *prev = NULL;
    node_w *curr = NULL;
    
    if(list == NULL) return new;

    for(curr = list; curr != NULL; curr = curr -> next){
        if (strcmp(new -> word,curr -> word) >= 0){
            prev = curr;
        }else{
            break;
             }
        }
    new -> next = curr;
    if (prev == NULL){
        return (new);
    }else{
        prev->next = new;
        return list;
            }
}

node_t *peek_front(node_t *list) {
    return list;
}


node_t *remove_front(node_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}


void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}
