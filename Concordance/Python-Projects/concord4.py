import sys
import re

main_list = []
# holds [[word,sentence,line_num,count]]

class Concordance:
    
    def __init__(self, filename, exceptions = ""):
        # set exception default to none

        self.filename = filename
        self.exception_file = exceptions
        # -- > debugging --> print(self.filename), print(self.exception_file)
        
        # send to parse filename and exception
        _parse_lines(self.filename,self.exception_file)
    
    def full_text(self):
        
        global main_list
        
        # find longest word in main_list
        max_word_len = 0
        for i in range(len(main_list)):
            if len(main_list[i][0]) > max_word_len:
                max_word_len = len(main_list[i][0])
        
        # sort words by the first element
        main_list.sort(key = lambda x : x[0])
        
        # return main_list(word + sentence + (line_number) as a list
        return_list = []
        spaces = ' '
        
        # append word + spaces + sentences + line number and check if count >= 1 then add a star next to line number
        for i in range(len(main_list)):
            if main_list[i][3] != 0:
                return_list.append(main_list[i][0] + spaces * (max_word_len - len(main_list[i][0]) + 2) + 
                    main_list[i][1] + " (" + str(main_list[i][2]) + "*)")
            else:
                return_list.append(main_list[i][0] + spaces * (max_word_len - len(main_list[i][0]) + 2) + 
                    main_list[i][1] + " (" + str(main_list[i][2]) + ")")
        
        return return_list


def _parse_lines(fpt,ept):

    file_err = "Include valid file path name(s)"
    exception_words = []
    exception_words = _parse_to_list(ept)

    # check if inXX.txt exists and if there is no -e file then don't need to go to exception_Words
    if len(fpt) < 1:
        print(file_err)
        sys.exit(1)

    if ept == None:
        _parse_main(fpt,exception_words)

    else:
        exception_words = _parse_to_list(ept)
        _parse_main(fpt,exception_words)

def _parse_to_list(ept):

    # check if a -e file was passed
    if ept == None:
        return []

    # open and put all lines in a list
    try:
        eptr = open(ept,"r")
        eptr_list = eptr.read().splitlines()
        eptr.close()

    except FileNotFoundError:
        print("Include valid exception file name: %s" % ept)
        sys.exit(1)
    
    return eptr_list

def _parse_main(fpt,exception_words):

    global main_list

    line_num = 1
    line_subbed = ""
    line_list = []

    # turn exception words to all upper
    e_list = [word.upper() for word in exception_words]

    # making sure no spaces are printed
    e_list.append("")

    # get a list of all lines in inXX.txt file
    parsed_list = _parse_to_list(fpt)

    for sentence in parsed_list:

        # remove delimters and split everything into single words
        line_subbed = re.sub(r"[\;\:]", "", sentence)
        line_subbed = re.sub(r"\.", "", line_subbed)
        line_subbed = re.sub(r"[\!\?]", "", line_subbed)
        line_subbed = re.sub(r"[\,\"]", "", line_subbed)
        txt_list = re.split(r"\s",line_subbed)
        
        # turn words into all upper case
        upper_word_list = [word.upper() for word in txt_list]

        # [word[0] , sentence[1] ,line_num[2],count[3]]
        for word in upper_word_list:
            if not (word in e_list) and not (_check_existence(word,line_num)):
                main_list.append([word, sentence, line_num, 0])
        line_num += 1

    return main_list

def _check_existence(cur_word,line_num):

    global main_list

    # if word and line number exists then increment the count which is the 4th element in the list at i
    if len(main_list) > 0:
        for i in range(len(main_list)):
            if cur_word == main_list[i][0] and line_num == main_list[i][2]:
                main_list[i][3] += 1
                return True

    return False
