#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_SIZE 20
#define MAX_SENTENCE_LENGTH 80
#define TOTAL_KEYWORDS 500
#define TOTAL_EXCEPTION_KEYWORDS 100

// Function Prototypes
void populate_exception(char *Path);
void parse_reg(char *Path);
int check_exclude(char *check_word);
void parse_exclude(char *e_file);
void spaces(int spaces);
void print_struct();
void to_upper(char *word);
int compareto(const void* word1, const void* word2);

// Holds number of elements in struct and exception_words, using these as 
// global variables because it is used all through out the program.
int MAIN_SIZE = 0;
int EXCEPTION_SIZE = 0;

// Struct for in0X.txt file
struct main_struct{
    char word[MAX_WORD_SIZE];
    char sentence[MAX_SENTENCE_LENGTH];
    int line_number;        
};

// main contains words, sentence, and line_num, exception_words contains exclusive words.
struct main_struct main1[TOTAL_KEYWORDS];
char exception_words[TOTAL_EXCEPTION_KEYWORDS][MAX_WORD_SIZE];

// main function, parseing arugments and sends file_path to appropriate functions.
void main(int argc,char *argv[]){

    // number to store file location in argv
    char exception_file[10];
    char *str = "Include a valid file path name.\n";
    int main_file = 0;
    int e_file = 0;
    int len;

    // argc count 1,2 and >= 3
    if(argc == 1){
        fprintf(stderr,"%s",str);
        exit(1);
    }else if(argc == 2){
        parse_reg(argv[1]);     
    }else{
        // -e english.txt in05.txt
        if(strcmp(argv[1],"-e") == 0){
            e_file = 2;
            main_file = 3;
        // in05.txt -e english.txt
        }else{
            e_file = 3;
            main_file = 1;
             }
        }

    // making sure arguments were recieved
    if(e_file != 0) populate_exception(argv[e_file]);
    if(main_file == 0){
        fprintf(stderr,"%s",str);
        exit(1);
        }

    // if exclude file is found.
    parse_reg(argv[main_file]);
    }

// Open exclude words text files, then stores the word in a 2d char array.
void populate_exception(char *Path){
    FILE *data_ep = fopen(Path,"r");
    char buffer[MAX_WORD_SIZE];
    int len = 0;
    if(data_ep == NULL){
        fprintf(stderr,"unable to open %s\n",Path);
        exit(1);
        }
    // if word ends in new line, replaces it with null character.
    while(fgets(buffer,sizeof(char)*MAX_WORD_SIZE,data_ep)){
        len = strlen(buffer) - 1;
        if(buffer[len] == '\n') buffer[len] = '\0';
        
        strncpy(exception_words[EXCEPTION_SIZE],buffer,MAX_WORD_SIZE);
        EXCEPTION_SIZE++;
        }
    fclose(data_ep);
}

/* 
 *  Recieves cur_line and that lines line number.
 *  The token break up the sentence into words, which is then added into struct main1
 *  which adds the word,sentence, and line_number. and also increments MAIN_SIZE   
 */   
void add_words(char *line,int line_num){
    char *token;
    int len = strlen(line);
    char line2[MAX_SENTENCE_LENGTH];
    strncpy(line2,line,MAX_SENTENCE_LENGTH);
    token = strtok(line, " ");

        // While token isnt null, then check if -e flag was used, by check the
        // EXCEPTION_SIZE, if not just add the word. If the -e flag was used
        // then send token to check_exclude, which compares it to all words in
        // exception_words to see if we should exclude it.
    while(token != NULL){
        if(EXCEPTION_SIZE >= 1){
            if(!(check_exclude(token))){
                        strncpy(main1[MAIN_SIZE].word,token,MAX_WORD_SIZE);
                        strncpy(main1[MAIN_SIZE].sentence,line2,MAX_SENTENCE_LENGTH);
                        main1[MAIN_SIZE].line_number = line_num;
                        MAIN_SIZE +=1; 
                                       }
        }else{
                    strncpy(main1[MAIN_SIZE].word,token,MAX_WORD_SIZE);
                    strncpy(main1[MAIN_SIZE].sentence,line2,MAX_SENTENCE_LENGTH);
                    main1[MAIN_SIZE].line_number = line_num;
                    MAIN_SIZE += 1;
                 }
        token = strtok(NULL, " ");   
    }
    line2[0] = '\0';
}

// open and parses in0x.txt files.
void parse_reg(char *Path){
    
    FILE *data_fp = fopen(Path,"r");
    char buffer[MAX_SENTENCE_LENGTH];
    int line_num = 1;
    char cur_line[MAX_SENTENCE_LENGTH];
    int len;
    
     if(data_fp == NULL){
        fprintf(stderr,"unable to open %s\n",Path);
        exit(1);
     }

    // gets line of 80 chars then replace last char to '\0'
    // then send lien and line_num to add_words, and finally increment line_num
    while(fgets(buffer,sizeof(char) * MAX_SENTENCE_LENGTH,data_fp)){
        len = strlen(buffer) - 1;
        buffer[len] = '\0';
        add_words(buffer,line_num);
        line_num++;
    }

    // check if anything was added into struct, if not then exit
    if(MAIN_SIZE == 0){
        exit(0);
    }
   
    print_struct();    
}

// checks if word should be exclude.
int check_exclude(char *check_word){
    for(int i = 0; i < EXCEPTION_SIZE;i++){
        if(strcmp(check_word,exception_words[i]) == 0) return 1;
    }

    return 0;
}

// prints spaces between word and sentence.
void spaces(int spaces){ 
    for(int i = 0; i < spaces; i++ ) printf(" ");
}

// recieves cur_word and prints it all in upper case
void to_upper(char *cur_word){
    int i = 0;
    char ch;
    while(cur_word[i]){
        ch = cur_word[i];
        printf("%c",toupper(ch));
        i++;
    }
}

/*  
 *  Copied from - stackoverflow.com - Link--
 *  stackoverflow.com/question/13372688/sorting-members-of-structure-array
 *  Method to help qsort by comparing 2 words alphabetically
*/
int compareto(const void* s1, const void* s2){
    struct main_struct *e1 = (struct main_struct *)s1;
    struct main_struct *e2 = (struct main_struct *)s2;
    return strcmp(e1->word, e2->word);
}

// Method to print struct contents.
void print_struct(){    
    
    //  Sort all struct by word alphabetically
    qsort(main1,MAIN_SIZE,sizeof(struct main_struct),compareto);

    //  Find largest word length to know how many spaces are needed
    //  between word and sentence
    int max_word = 0;
    for(int x = 0;x<MAIN_SIZE;x++){
        if(strlen(main1[x].word) > max_word){
         max_word = strlen(main1[x].word);
            }
        }
    //  to account for space between word and sentence.
    max_word += 2;

    // variable use to point to the next struct value.
    int w = 0; 
    
    /*
     *  First send cur_word to to_upper to print in upper case
     *  Then send to spaces to create space between word and sentence
     *  Then print the sentence
     *  Finally check whether the word and sentence are a duplicate
     *  by comparing the next line, if so add asterrisk and 
     *  skip the next line, otherwise print line_num and move on.
     */
 
    for(int i = 0;i<MAIN_SIZE;i++){
        
        to_upper(main1[i].word);
        spaces(max_word - strlen(main1[i].word));
        printf("%s",main1[i].sentence);    
        w = i + 1;
        
        if(strcmp(main1[i].word,main1[w].word) == 0){
            if(main1[i].line_number == main1[w].line_number){
                printf(" (%d*)\n",main1[i].line_number);
                i++;
            }else{
            printf(" (%d)\n",main1[i].line_number);                
                 }
        }else{
            printf(" (%d)\n",main1[i].line_number);
             }   
        }
      exit(0);
}
