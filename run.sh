#!/bin/bash

#for j in `seq 300`;
#do
	for i in `seq 5`;
	do
		#./train $j model_init.txt seq_model_0$i.txt model_0$i.txt		
		./train $1 model_init.txt seq_model_0$i.txt model_0$i.txt		
	done

	./test modellist.txt testing_data1.txt result

	#echo $j
	python compute_acc.py testing_answer.txt result

#done
