#!/usr/bin/env bash
rm CMakeCache.txt
cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release .
make -j3 || exit;
for i in a b c d e f g h i j k l m n o p q r s t u v w x y z
do
	echo Testing for ${i}.tlb...
	./olebow32.exe ../x86-testdata/${i}.tlb ../x86-testdata/${i}-3.txt
done
