#! /bin/bash	

input_file=$1
output_file=$2

# 文件重定向

read n < $input_file

sum=0

for ((i=1;i < n;i++))
do
	eq=`expr ${i} \* ${i}`
	sum=`expr ${sum} + ${eq}`
done


echo $sum > ${output_file}