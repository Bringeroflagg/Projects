#!/usr/bin/env python

import sys
#import time
#start_time = time.time()

"""
    3 data variables used:
- exception_lists will be accessed when -e tag is included and contains all exception words.
- sentence_dict will be structured as --> {1:line_1, 2:line_2,...,n:line_n} and holds line_number
    as key and the sentence as the value.
- words_list will be structured as a 2d array with 3 items in every objects.
    They will be (word,line_num,count_of_this_word_at_this_line set to 0) and they will 
    look like --> [[word_1,line_num_1,count],...,[word_n,line_num_n,count]]
"""
def main():
    main_file = ""    
    file_err = "Include valid file path name(s)."
    exception_list = []

    if (len(sys.argv) < 2):
        print(file_err)
        sys.exit(1)
    elif len(sys.argv) < 3:
        main_file = sys.argv[1]
    elif len(sys.argv) == 4:

        # Input style: -e english.txt in0x.txt
        if sys.argv[1] == '-e':
            exception_list = parse_exception(sys.argv[2])
            main_file = sys.argv[3] 
        # Input style: in0x.txt -e english.txt
        elif sys.argv[2] == '-e':
            exception_list = parse_exception(sys.argv[3])
            main_file = sys.argv[1]
    else:
        print("Incorrect argument(s), %s" % file_err)
        sys.exit(1) 

    # Check if main_file is a text file by using string splicing
    if main_file[-4:] == '.txt':
        parse_words(main_file,exception_list)
    else:
        print(file_err)
        sys.exit(1)

def return_file_list(cur_file):
    # Try and open file, remove newlines and read and store all lines in an array of list. Then return list.
    try:
        fptr = open(cur_file,"r")
        fptr_list = fptr.read().splitlines()
        fptr.close()
    except FileNotFoundError:
        print("Include valid file name, cannot open: %s" % cur_file)
        sys.exit(1)

    return fptr_list

def parse_exception(exception_file):
    # Send to return_file_list to get a list containing only words in this case
    exception_li = return_file_list(exception_file)    
    # Sets all exception words in upper case using list comprehension
    exception_li = [word.upper() for word in exception_li]
    return (exception_li)

def parse_words(in_file,exception_list):
    # Return_file_list returns a list containing each line in the file without newline
    sentences_li = return_file_list(in_file)
    line_num = 1
    cur_line_list = []
    words_list = []
    sentence_dict = {}
    
    # Add cur_sentence to sentence_dict, then split line into a list, then send list and line_num
    # To add_words, finally incremnt line_num counter
    for line in sentences_li:
        sentence_dict[line_num] = line
        cur_line_list = line.split()
        words_list = add_words(cur_line_list,line_num,exception_list,words_list)
        line_num +=1
    
    #sentences_li.clear()
    print_words(words_list,sentence_dict)

def add_words(line_list,line_num,e_list,words_list):
    # Turn all words in line_list into upper case using list comprehension
    line_list = [word.upper() for word in line_list]    
    # Adds [cur_word, line_num, count] to list if word is not to be excluded and it doesnt already exist
    for cur_word in line_list:
        if cur_word not in e_list:
            if check_existence(cur_word,line_num,words_list):
                words_list.append([cur_word,line_num,0]) 
    return words_list

def check_existence(cur_word,line_num,words_list):
    # Check if cur_word and line_num exist, if they do then increment words_list[i][2], which holds count.
    if len(words_list) > 0:
        for i in range(len(words_list)):
            # gets the list within list and checks that [i] list if it contains that word
            if cur_word in words_list[i] and line_num == words_list[i][1]:
                words_list[i][2] += 1
                return False
    return True

def print_words(words_list,sentence_dict):
    # Sorts words_list by the first element in each list.   
    words_list.sort(key = lambda x: x[0])
    # find longest word
    max_word_len = 0
    for i in range(len(words_list)):
        if len(words_list[i][0]) > max_word_len:
            max_word_len = len(words_list[i][0])
    
    # Print word, then print spaces by multiplying space by (max_word - len(cur_word), then print sentence, finally
    # print line_number; if words_list[i][2] ==> count == 0 print (line_number), else print (line_number*)
    space = ' '
    for i in range(len(words_list)):
        if words_list[i][2] == 0:
            print("%s%s  %s (%d)" % (words_list[i][0], space * (max_word_len - len(words_list[i][0])) , sentence_dict[words_list[i][1]], words_list[i][1]))
        else:
            print("%s%s  %s (%d*)" % (words_list[i][0], space * (max_word_len - len(words_list[i][0])) , sentence_dict[words_list[i][1]], words_list[i][1]))
    # print("took",time.time() - start_time," to run")
    exit(0)

if __name__ == "__main__":
    main()
