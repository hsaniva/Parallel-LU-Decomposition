#!/bin/bash
make all
clear
function spin(){
	message=('Computing LU Decomposition...\' 'Computing LU Decomposition...|' 'Computing LU Decomposition.../' 'Computing LU Decomposition...-')
	while [ 1 ]
	do
		for i in {1..4}
		do
			echo -ne "\r${message[i]}"
			sleep 0.2
		done
	done
}
function bulkExecution(){
	for i in 1 2 4 8 16 32
	do
		echo $1 $i `./${2} $1 $i ${3}` >> $4
		sleep 2
	done
}

while [ 1 ]
do
	echo ""
	echo "=================================================="
	echo "|      LU Decomposition using pThread/OpenMP     |"
	echo "=================================================="
	echo -e "||               Avinash  Gaikwad               ||"
	echo "--------------------------------------------------"
	echo -ne "\n1. pThread implementation\n2. OpenMP implementation\n3. Sequential Implementation\n4. pThread vs. OpenMP(number of threads vs execution time)\n5. pThread vs. OpenMP vs. Sequential\n6. Verify LU Decomposition\n7. Show graphs for pThread vs. OpenMP\n8. Speed up of OpenMP and pThread\n9. Exit\n"
	read -p "Enter your choice " choice
	if [ $choice -eq 1 ]
	then
		read -p "Enter square matrix size " matSize
		read -p "Enter the number of threads " nThreads
		spin&
		pid=$!
		echo "--------------------------------------------------"
		echo -e "\nTotal time taken using pThreads" `./pthreadImpl $matSize $nThreads "pi"` "seconds"
		echo "--------------------------------------------------"
		kill $pid
	elif [ $choice -eq 2 ]
	then
		read -p "Enter square matrix size " matSize
		read -p "Enter the number of threads " nThreads
		spin&
		pid=$!
		echo "--------------------------------------------------"
		echo -e "\nTotal time taken using OpenMP" `./openMP $matSize $nThreads` "seconds"
		echo "--------------------------------------------------"
		kill $pid
	elif [ $choice -eq 3 ]
	then
		read -p "Enter square matrix size " matSize
		spin&
		pid=$!
		echo "--------------------------------------------------"
		echo -e "\nTotal time taken using Sequential algorithm" `./sequential $matSize` "seconds"
		echo "--------------------------------------------------"
		kill $pid
	elif [ $choice -eq 4 ]
	then
		read -p "Enter square matrix size " matSize
		spin&
		pid=$!
		rm -rf OutputFiles/pThread_timings.csv
		rm -rf OutputFiles/openMP_timings.csv
		bulkExecution $matSize "pthreadImpl" "pi" "OutputFiles/pThread_timings.csv"
		bulkExecution $matSize "openMP" "" "OutputFiles/openMP_timings.csv"
		kill $pid
	elif [ $choice -eq 5 ]
	then
		spin&
		pid=$!
		rm -rf OutputFiles/pThread_constThread_timings.csv
		rm -rf OutputFiles/openMP_constThread_timings.csv
		rm -rf OutputFiles/sequential_timings.csv

		for i in {1000..5000..1000}
		do
			echo $i 4 `./pthreadImpl $i 4 pi` >> OutputFiles/pThread_constThread_timings.csv
			sleep 0.5
			echo $i 4 `./openMP $i 4` >> OutputFiles/openMP_constThread_timings.csv
			sleep 0.5
			echo $i `./sequential $i` >> OutputFiles/sequential_timings.csv
			sleep 0.5
		done

		kill $pid
	elif [ $choice -eq 6 ]
	then
		read -p "Enter square matrix size " matSize
		spin&
		pid=$!
		echo ""
		echo "PThread Verification"
		./pthreadImpl $matSize 4 "pi" 1
		echo -e "\n\n"
		echo "OpenMP Verification"
		./openMP $matSize 4 1
		kill $pid
	elif [ $choice -eq 7 ]
	then
		python3 GraphPlots/size_vs_time.py&
		python3 GraphPlots/thread_vs_time.py&
	elif [ $choice -eq 8 ]
	then
		python3 GraphPlots/speedup.py&
	elif [ $choice -eq 9 ]
	then
		clear
		exit 0
	fi
done
