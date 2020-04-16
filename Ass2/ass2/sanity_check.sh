#! /usr/bin/env bash


rm -f R.bsig R.data R.info R.psig R.tsig

echo "./create R 10000 6 1000"
./create R 10000 6 1000


echo "./insert R < R.in"
./insert R < R.in


echo "./stats R"
./stats R


echo "./select R '?,?,?,?,?,?' x | tail -6"
./select R '?,?,?,?,?,?' x | tail -6


echo "./select R '1234999,?,?,?,?,?' x"
./select R '1234999,?,?,?,?,?' x


echo "./select R '1234999,?,?,?,?,?' t"
./select R '1234999,?,?,?,?,?' t


echo "./select R '1234999,?,?,?,?,?' p"
./select R '1234999,?,?,?,?,?' p


echo "./select R '1234999,?,?,?,?,?' b"
./select R '1234999,?,?,?,?,?' b


echo "grep 'a3-241,a4-158,a5-407' R.data"
grep 'a3-241,a4-158,a5-407' R.data


echo "./select R '?,?,a3-241,a4-158,a5-407,?' x"
./select R '?,?,a3-241,a4-158,a5-407,?' x


echo "./select R '?,?,a3-241,a4-158,a5-407,?' t"
./select R '?,?,a3-241,a4-158,a5-407,?' t


echo "./select R '?,?,a3-241,a4-158,a5-407,?' p"
./select R '?,?,a3-241,a4-158,a5-407,?' p


echo "./select R '?,?,a3-241,a4-158,a5-407,?' b"
./select R '?,?,a3-241,a4-158,a5-407,?' b


rm -f R.bsig R.data R.info R.psig R.tsig
