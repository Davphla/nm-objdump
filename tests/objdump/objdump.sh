#!/bin/sh

for i in $(find tests  -name "*.test" -type f);
do
  echo "Test [$i]:"
  LANG=en_US.UTF-8 objdump "$i" -fs > tests/real
  ./my_objdump "$i" > tests/test
  cmp --silent tests/real tests/test || code --diff --wait tests/real tests/test
  echo ""
done
