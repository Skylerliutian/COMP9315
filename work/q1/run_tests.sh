#!/bin/sh

if [ ! -d "tests" ]
then
	echo "You must run this script from your 'q1' directory"
	exit 1
fi

if [ ! -x "buckets" ]
then
	echo "You don't seem to have a program to test"
	exit 1
fi

for t in tests/t?
do
	./buckets `cat $t` 2>&1 | head -200 > $t.observed
	diff -wi $t.expected $t.observed > $t.diffs

	echo ""
	echo "===== $t ====="
	if [ ! -s "$t.diffs" ]
	then
		echo OK
	else
		echo FAILED
		echo ""
		echo "===== First ten lines of your output ====="
		head -n10 $t.observed
		echo ""
		echo "===== First ten lines of expected output ====="
		head -n10 $t.expected
		echo ""
		echo "See the file $t.diffs for diff output"
	fi
done
