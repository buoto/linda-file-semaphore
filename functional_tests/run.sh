#!/bin/bash
DIRECTORY=`dirname $0`
EXEC=linda

for i in {1..1}
do
    ./$EXEC < $DIRECTORY/$i.in | diff $DIRECTORY/$i.out - &&
        echo "###### Test nr $i passed. ######" ||
        echo "###### Test nr $i failed! ######"
done

