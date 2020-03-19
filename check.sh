#!/usr/bin/env bash


# e1 - 20
# e2 - 15
# e3 - 15
# u - 10
# c - 30
# b - 20
# - adaug memory leakuri

# rm-uri
# mkdiruri
#make clean
#make build

mkdir ./tests/outputs/
make clean
make build

#nr = numar teste
nr=9

#stval = val pt bonus
sigma=250

input_dir="./tests/inputs/"
output_dir="./tests/outputs/"
ref_dir="./tests/refs/"
ZERO=0
VALGRIND_ARGS="--leak-check=full --error-exitcode=2"

result=0
for entry in `seq 0 $nr`; do	
	var=${input_dir}test${entry}
  	for file in ${var}*; do
		input_file=$file
  	done

	if [ $entry -eq 0 ]; then
		declare -a arr=("e1")
		declare -a cmm=("--e1")
		declare -a p=(4)
	fi

	if [ $entry -eq 1 ]; then
		declare -a arr=("e1" "e2" "e3")
		declare -a cmm=("--e1" "--e2" "--e3")
		declare -a p=(4 5 3)
	fi
	if [ $entry -eq 2 ]; then
		declare -a arr=("e1")
		declare -a cmm=("--e1")
		declare -a p=(4)
	fi
	if [ $entry -eq 3 ]; then
		declare -a arr=("u")
		declare -a cmm=("--u")
		declare -a p=(2)
	fi
	if [ $entry -eq 4 ]; then
		declare -a arr=("c")
		declare -a cmm=("--c")
		declare -a p=(6)
	fi
	if [ $entry -eq 5 ]; then
		declare -a arr=("c" "uc" "uce3")
		declare -a cmm=("--c" "--u --c" "--u --c --e3")
		declare -a p=(6 7 9)
	fi
	if [ $entry -eq 6 ]; then
		declare -a arr=("e2uu" "e3uu")
		declare -a cmm=("--e2 --u --u" "--e3 --u --u")
		declare -a p=(4 4)
	fi
	if [ $entry -eq 7 ]; then
		declare -a arr=("e1" "e1e2" "e1e2e3" "e1e3")
		declare -a cmm=("--e1" "--e1 --e2" "--e1 --e2 --e3" "--e1 --e3")
		declare -a p=(3 3 5 3)
	fi
	if [ $entry -eq 8 ]; then
		declare -a arr=("e2e3e1u" "st2")
		declare -a cmm=("--e2 --e3 --e1 --u" "--st2")
		declare -a p=(6 10)
	fi
	if [ $entry -eq 9 ]; then
		declare -a arr=("e1e2e3uc" "st10")
		declare -a cmm=("--e1 --e2 --e3 --u --c" "--st10")
		declare -a p=(12 10)
	fi
	for i in "${!arr[@]}"; do
		output_file=$(echo "${output_dir}test${entry}.out.${arr[i]}")
		ref_file=$(echo "${ref_dir}test${entry}.ref.${arr[i]}")
		valgrind $VALGRIND_ARGS ./tema1 ${cmm[i]}  < $input_file &> /dev/null
       		EXIT_CODE=$?
		DOI=2
        	if [[ $EXIT_CODE -eq $DOI ]]
        	then
        	        echo "Memory leaks"
        	else
        	        echo "No memory leaks"
        	fi
	 	./tema1 ${cmm[i]} < $input_file > $output_file
		toAdd=${p[i]}
		diff $output_file $ref_file > /dev/null
		EXIT_CODE=$?
		if [[ $EXIT_CODE -eq $ZERO ]]
		then
              		echo "Test $entry ${arr[i]} __________________ PASSED [$toAdd p]"
			result=$((result + toAdd))
		else
			echo "Test $entry ${arr[i]} __________________ FAILED"
        	fi
	done
done

echo "Result = $result"
rm -r ./tests/outputs/

