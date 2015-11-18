#!/bin/bash
N=5
# THREAD_NUM=4

for file in ./randtrack ./randtrack_global_lock ./randtrack_tm ./randtrack_list_lock ./randtrack_element_lock ./randtrack_reduction
do
    for thread_num in 1 2 4
    do
        echo "FILE: $file THREAD_NUM: $thread_num"
        rm ./.times
        for i in `seq 1 $N`;
        do
            /usr/bin/time -a -p -o ./.times ${file} ${thread_num} 100 > /dev/null
            #echo $i
        done
        echo "Average user :"
        cat ./.times | grep user | sed 's/user //' | awk "{s+=\$1} END {print s/$N}"
        echo "Average real :"
        cat ./.times | grep real | sed 's/real //' | awk "{s+=\$1} END {print s/$N}"
    done
done
