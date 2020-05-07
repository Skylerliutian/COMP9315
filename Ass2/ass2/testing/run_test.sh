#! /usr/bin/env bash

echo "Run make"

make -B all

echo "Running tests on Bits ADT"

for file in tests/x1/*.stdin
do
  rm -f R.* "${file%.stdin}.out" "${file%.stdin}.err"
  ./x1 "$file" > "${file%.stdin}.out" 2> "${file%.stdin}.err"
  if [[ $(wc -c "${file%.stdin}.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.err"
  else
    diff -s "${file%.stdin}.out" "${file%.stdin}.exp"
    rm "${file%.stdin}.err"
  fi
done

echo "Running tests on tsig"

for file in tests/x2_3/*.stdin
do
  rm -f R.* "${file%.stdin}.out" "${file%.stdin}.err"
  cat "${file%.stdin}.spec" | xargs ./create "R"
  ./insert "R" < "${file%.stdin}.in"
  while IFS="" read -r p || [ -n "$p" ]
  do
    ./x2_3 <<< "$p" >> "${file%.stdin}.out" 2>> "${file%.stdin}.err"
  done < "$file"
  if [[ $(wc -c "${file%.stdin}.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.err"
  else
    diff -s "${file%.stdin}.out" "${file%.stdin}.exp"
    rm "${file%.stdin}.err"
  fi
done

for file in tests/x3_3/*.stdin
do
  rm -f R.* "${file%.stdin}.out" "${file%.stdin}.err"
  cat "${file%.stdin}.spec" | xargs ./create "R"
  ./insert "R" < "${file%.stdin}.in"
  while IFS="" read -r p1 && read -r p2 || [ -n "$p1" ]
  do
    ./x3_3 << EOF >> "${file%.stdin}.out" 2>> "${file%.stdin}.err"
$p1
$p2
EOF
  done < "$file"
  if [[ $(wc -c "${file%.stdin}.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.err"
  else
    diff -s "${file%.stdin}.out" "${file%.stdin}.exp"
    rm "${file%.stdin}.err"
  fi
done

for file in tests/x4/*.stdin
do
  rm -f R.* "${file%.stdin}.3.out" "${file%.stdin}.3.err"
  cat "${file%.stdin}.spec" | xargs ./create "R"
  ./insert "R" < "${file%.stdin}.in"
  ./stats "R" >> "${file%.stdin}.3.out" 2>> "${file%.stdin}.3.err"
  while IFS="" read -r p || [ -n "$p" ]
  do
    echo "R $p t" | xargs ./select >> "${file%.stdin}.3.out" 2>> "${file%.stdin}.3.err"
  done < "$file"
  if [[ $(wc -c "${file%.stdin}.3.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.3.err"
  else
    diff -s "${file%.stdin}.3.out" "${file%.stdin}.3.exp"
    rm "${file%.stdin}.3.err"
  fi
done

echo "Running tests on psig"

for file in tests/x2_4/*.stdin
do
  rm -f R.* "${file%.stdin}.out" "${file%.stdin}.err"
  cat "${file%.stdin}.spec" | xargs ./create "R"
  ./insert "R" < "${file%.stdin}.in"
  while IFS="" read -r p || [ -n "$p" ]
  do
    ./x2_4 <<< "$p" >> "${file%.stdin}.out" 2>> "${file%.stdin}.err"
  done < "$file"
  if [[ $(wc -c "${file%.stdin}.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.err"
  else
    diff -s "${file%.stdin}.out" "${file%.stdin}.exp"
    rm "${file%.stdin}.err"
  fi
done

for file in tests/x3_4/*.stdin
do
  rm -f R.* "${file%.stdin}.out" "${file%.stdin}.err"
  cat "${file%.stdin}.spec" | xargs ./create "R"
  ./insert "R" < "${file%.stdin}.in"
  while IFS="" read -r p1 && read -r p2 || [ -n "$p1" ]
  do
    ./x3_4 << EOF >> "${file%.stdin}.out" 2>> "${file%.stdin}.err"
$p1
$p2
EOF
  done < "$file"
  if [[ $(wc -c "${file%.stdin}.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.err"
  else
    diff -s "${file%.stdin}.out" "${file%.stdin}.exp"
    rm "${file%.stdin}.err"
  fi
done

for file in tests/x4/*.stdin
do
  rm -f R.* "${file%.stdin}.4.out" "${file%.stdin}.4.err"
  cat "${file%.stdin}.spec" | xargs ./create "R"
  ./insert "R" < "${file%.stdin}.in"
  ./stats "R" >> "${file%.stdin}.4.out" 2>> "${file%.stdin}.4.err"
  while IFS="" read -r p || [ -n "$p" ]
  do
    echo "R $p p" | xargs ./select >> "${file%.stdin}.4.out" 2>> "${file%.stdin}.4.err"
  done < "$file"
  if [[ $(wc -c "${file%.stdin}.4.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.4.err"
  else
    diff -s "${file%.stdin}.4.out" "${file%.stdin}.4.exp"
    rm "${file%.stdin}.4.err"
  fi
done

echo "Running tests on bsig"

for file in tests/x4/*.stdin
do
  rm -f R.* "${file%.stdin}.5.out" "${file%.stdin}.5.err"
  cat "${file%.stdin}.spec" | xargs ./create "R"
  ./insert "R" < "${file%.stdin}.in"
  ./stats "R" >> "${file%.stdin}.5.out" 2>> "${file%.stdin}.5.err"
  while IFS="" read -r p || [ -n "$p" ]
  do
    echo "R $p b" | xargs ./select >> "${file%.stdin}.5.out" 2>> "${file%.stdin}.5.err"
  done < "$file"
  if [[ $(wc -c "${file%.stdin}.5.err" | cut -d' ' -f 1) -ne 0 ]]
  then
    echo "errors in ${file%.stdin}.5.err"
  else
    diff -s "${file%.stdin}.5.out" "${file%.stdin}.5.exp"
    rm "${file%.stdin}.5.err"
  fi
done