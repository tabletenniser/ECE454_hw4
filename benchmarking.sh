#!/bin/bash
N=10
rm ./.times
for i in `seq 1 $N`;
do
    /usr/bin/time -a -p -o ./.times ./randtrack_reduction 4 50 > /dev/null
    echo $i
done
echo "Average user :"
cat ./.times | grep user | sed 's/user //' | awk "{s+=\$1} END {print s/$N}"
echo "Average real :"
cat ./.times | grep real | sed 's/real //' | awk "{s+=\$1} END {print s/$N}"
