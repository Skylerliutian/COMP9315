#!/bin/sh
cp ~Documents/COMP9315/Ass1/pname.c .
cp ~Documents/COMP9315/Ass1/pname.source .
cp ~Documents/COMP9315/Ass1/data0.sql .


make
dropdb test
createdb test
psql test -f pname.sql
psql test -f data0.sql