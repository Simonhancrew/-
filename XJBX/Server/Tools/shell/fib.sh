#! /bin/bash

# 求斐波那契数列

a[0]=1
a[1]=1

read n


for ((i = 2 ; i <= ${n} ; i++))
do
	a[i]=`expr ${a[i-1]} + ${a[i-2]}` 
done

echo ${a[n]}