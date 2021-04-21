#!/usr/bin/env bash
for i in a b c d e f g h i j k l m n o p q r s t u v w x y z
do
	echo Testing for ${i}.tlb...
	./src/olebow.exe --sort --dependency ../x86-testdata/${i}.tlb ../x86-testdata/${i}-3.txt > ../x86-testdata/${i}-4.txt
done
