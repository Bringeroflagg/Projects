/*
 * concord3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Summer 2021
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"


/*
void print_word(node_t *node, void *arg)
{
    char *format = (char *)arg;
    printf(format, node->text);
}
#ifdef DEBUG
*
 * Just showing the use of the linked-list routines.
 *
void _demo() {
printf("DEBUG: in _demo\n");
    char *words_german[] = {"Der", "Kater", "mit", "Hut."};
    int   words_german_len = 4;
    char *words_english[] = {"The", "cat", "in", "the", "hat."};
    int   words_english_len = 5;
    node_t *temp_node = NULL;
    node_t *head = NULL;
    int i;
    * Add the words in German, at the front. *
    for (i = 0; i < words_german_len; i++) {
        temp_node = new_node(words_german[i]);
        head = add_front(head, temp_node);
    }
    * Add the words in English, at the end. *
    for (i = 0; i < words_english_len; i++) {
        temp_node = new_node(words_english[i]);
        head = add_end(head, temp_node);
    }
    * Print the list of words. *
    apply(head, print_word, "--> %s\n");
    * Free up the memory. This is done rather deliberately
     * and manually.  Asserts are liberally used here as they
     * express state that *must* be true if all of the code is
     * correctly working.
     *
    temp_node = head;
    while (temp_node != NULL) {
        assert(temp_node != NULL);
        head = remove_front(head);
        free(temp_node);
        temp_node = head;
    }  
    assert(head == NULL); 
}
#endif
*/

// free sentence and excluding word list
void free_list(node_t *e_list){
    node_t *temp = NULL;
    temp = e_list;

    for( ; e_list != NULL; e_list = temp){
        temp = e_list->next;
        free(e_list->text);
        free(e_list);
        }  
    /* 
    while(temp != NULL){
        temp = e_list->next;
        free(e_list->text);
        free(e_list);
        e_list = temp;
        }
    */
}
/*
// prind words in excluding and sentence list, used for debuging
void print_words(node_t *e_list){
    node_t *cur_word = NULL;
    for(cur_word = e_list; cur_word != NULL; cur_word = cur_word -> next){
        printf("%s\n",cur_word -> text);
        }
}
*/
// prints spaces by taking the max_word length and adding 2
void print_spaces(int count){
    for(int i = 0; i < count + 2; i++) printf(" ");
}
// traverses through entire sentence list and prints when it i is greater than line number
void print_sentence(node_t *s_list,int line){
    node_t *cur = NULL;
    cur = s_list;
    for(int i = 0; i < line; i++){
            cur = cur -> next;
    }
    printf("%s ",cur->text);

}
// prints word char by char in upper case
void to_upper(char *word){
    int i = 0;
    char ch;
    while(word[i]){
        ch = word[i];
        printf("%c",toupper(ch));
        i++;
    }
    }

// main method to print word, sentence, and line number
void print_main(node_w *m_list,node_t *s_list){
    node_w *cur_word = NULL;
    int max_word = 0;

    // get largest word size
    for(cur_word = m_list; cur_word != NULL; cur_word = cur_word -> next){
            if(strlen(cur_word -> word) > max_word){
                max_word = strlen(cur_word -> word);
        }
        }
    // go through entire linked list of words, and line number
    // print word upper, print spaces, then print sentence associated with the line number
    // finally found out if the next line is the same as current like, if it is print (line_number*)
    for(cur_word = m_list; cur_word != NULL; cur_word = cur_word->next){
        to_upper(cur_word->word);
        print_spaces(max_word - strlen(cur_word->word));
        print_sentence(s_list,cur_word -> line_number);
            if(cur_word->next != NULL && !strcmp(cur_word->word,cur_word->next->word) && cur_word->line_number == cur_word->next->line_number){
                printf("(%d*)\n",++(cur_word -> line_number));
                cur_word = cur_word -> next;
            }else{
                printf("(%d)\n",++(cur_word -> line_number));
                }
            }
    }
// free main word list, freeing the word then freeing the entire node
void free_list_w(node_w *list_l){
    node_w *cur_n = NULL;
    cur_n = list_l;
    while(cur_n != NULL){
        cur_n = list_l -> next;
        free(list_l->word);
        free(list_l);
        list_l = cur_n;
    }

    /*for(node_w *cur_n = list_l ; cur_n != NULL; list_l = cur_n){
        cur_n = list_l -> next;
        free(cur_n->word);
        free(cur_n);
    }*/
}
// goes through entire exlude word list and checks if work exists in exclude list
int check_exclude(char *word, node_t *list){
    node_t *cur = NULL;

    if(list == NULL) return 0;

    cur = list;
    while(cur != NULL){
        if(strcmp(word, cur -> text) == 0){
            return 1;
            }
        cur = cur -> next;
    }
    return 0;
    }

// parses command line, exclude file and main words file
int main(int argc, char *argv[])
{
    char *str = "Include a valid file path name.\n";
    int main_file = 0;
    int e_file = 0;

    /*
    char * expception_file = NULL;
    char *input_file = NULL;
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i+i < argc) {
            exception_file = argv[++i];
        } else {
            input_file = argv[i];
        }
    }
    */
    // add exception words into list
    // read command line
    if(argc == 1){
        fprintf(stderr,"%s",str);
        exit(1);
    }else if(argc == 2){
        main_file = 1;    
    }else{
        // -e english.txt in0x.txt
        if(strcmp(argv[1],"-e") == 0){
            e_file = 2;
            main_file = 3;
        // in0x.txt -e english.txt
        }else{
            e_file = 3;
            main_file = 1;
            }
        }
    // line is for current line and token is for current word in line
    char *line = NULL;
    char *token = NULL;
    // 3 linked lists; 1 for exception words, 1 for sentences and the final list to store word and linenumber
    node_t *exception_list = NULL;
    node_t *sentence_list = NULL;
    node_w *word_list = NULL;
    int len;
    token = (char *)malloc(sizeof(char) * LIST_MAXLEN_TEXT);
    line = (char *)malloc(sizeof(char) * MAX_LINE_LEN);

    // if there is a -e tag open and parse the entire file and store words in exception_list linked list.
    if(e_file != 0){
        if (line == NULL || token == NULL){
            fprintf(stderr,"Something went wrong with malloc.");
        }

        FILE *data_ep = fopen(argv[e_file],"r");
        token[0] = '\0';
        if (data_ep == NULL){
            fprintf(stderr,"unable to open %s\n",argv[e_file]);
            exit(1);
        }
        while(fgets(token,sizeof(char)*LIST_MAXLEN_TEXT,data_ep)){
            len = strlen(token) - 1;
            if(token[len] =='\n') token[len] = '\0';
            token[0] = tolower(token[0]);
            exception_list = add_end(exception_list,new_node(token));
        }
        // Close expception_list
        fclose(data_ep);
        //print_words(exception_list);
    }

    // add sentences to sentence_list and words with line_number to word_list
    int line_num = 0;
    FILE *data_fp = fopen(argv[main_file],"r");
    if(data_fp == NULL){
        fprintf(stderr,"unable to open %s\n",argv[main_file]);
        exit(1);
    }
    token[0] = '\0';
    while(fgets(line,sizeof(char)*MAX_LINE_LEN,data_fp)){
        len = strlen(line) - 1;
        if(line[len] == '\n') line[len] = '\0';

        sentence_list = add_end(sentence_list,new_node(line)); 
        token = strtok(line, " ");
        // add words by checking in they are in exclude and add in_order
        while(token != NULL){
            // if in exclude_list then don't add
            if(!(check_exclude(token, exception_list))){
                token[0] = tolower(token[0]);
                word_list = add_inorder(word_list,new_w(token,line_num));
            }
            token = strtok(NULL, " ");
        }
        line_num += 1;
    }
    fclose(data_fp);

    print_main(word_list,sentence_list);
    //print_words(sentence_list);
    
    // free all 3 linked lists
    free_list(sentence_list);
    free_list(exception_list);
    free_list_w(word_list);
    // free the malloced line and token
    free(token);
    free(line);
    exit(0);
/* 
 * Showing some simple usage of the linked-list routines.
#ifdef DEBUG
    _demo();
#endif
*/
}
