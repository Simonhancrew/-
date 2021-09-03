# 检查文件权限

if [ $# -ne 1 ]
then
    echo "arguments not valid"
    exit 1
elif ! [ -e ${1} ]
then
    echo "not exist"
    exit 2
else
    
    if [ -f ${1} ]
    then 
        echo "regular file"
    elif [ -d ${1} ]
    then
        echo "directory"
    fi
    
    if [ -r ${1} ]
    then
        echo "readable"
    fi
    
    if [ -w ${i} ]
    then
        echo "writable"
    fi
    
    if [ -x ${i} ]
    then
        echo "executable"
    fi
fi