#!/bin/bash

#chmod u+x concord2.py
#start=$SECONDS

fail_check[0]=$(./concord2.py in01.txt | diff out01.txt -)
fail_check[1]=$(./concord2.py in02.txt | diff out02.txt -)
fail_check[2]=$(./concord2.py in03.txt -e english.txt | diff out03.txt -)
fail_check[3]=$(./concord2.py in04.txt -e english.txt | diff out04.txt -)
fail_check[4]=$(./concord2.py in05.txt -e english.txt | diff out05.txt -)
fail_check[5]=$(./concord2.py in06.txt -e english.txt | diff out06.txt -)
fail_check[6]=$(./concord2.py in07.txt -e english.txt | diff out07.txt -)
fail_check[7]=$(./concord2.py in08.txt -e latin.txt | diff out08.txt -)
fail_check[8]=$(./concord2.py in09.txt -e latin.txt | diff out09.txt -)
fail_check[9]=$(./concord2.py in10.txt -e deutsch.txt | diff out10.txt -)
fail_check[10]=$(./concord2.py in11.txt -e english.txt | diff out11.txt -)
fail_check[11]=$(./concord2.py in12.txt -e english-2.txt | diff out12.txt -)
fail_check[12]=$(./concord2.py in13.txt -e english-2.txt | diff out13.txt -)
fail_check[13]=$(./concord2.py in14.txt -e english-2.txt | diff out14.txt -)
fail_check[14]=$(./concord2.py in15.txt -e latin.txt | diff out15.txt -)
fail_check[15]=$(./concord2.py in16.txt -e latin.txt | diff out16.txt -)
fail_check[16]=$(./concord2.py in17.txt -e latin-2.txt | diff out17.txt -)
fail_check[17]=$(./concord2.py in18.txt -e latin-2.txt | diff out18.txt -)
fail_check[18]=$(./concord2.py in19.txt -e deutsch-2.txt | diff out19.txt -)
fail_check[19]=$(./concord2.py in20.txt -e deutsch-2.txt | diff out20.txt -)


n=1
for i in "${fail_check[@]}"
do
    echo "$i"
    #echo ====================
    #if ["$i" == ""]
    #then
    #    echo Test "$n" passed
    #else
    #    echo Test "$n" failed
    #fi
    #echo ====================
    echo 

    ((n++)) || true

#end=$SECONDS
#total_s="$(end - start)"
#echo "Time: $total_s"

done
