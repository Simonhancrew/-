## BASH语法

其实执行效率比较低，一般做一些简单的文件操作和不太复杂的点点点替换为bash脚本。

再复杂一点的建议考虑python。

### 1 权限和运行

Linux一般默认的是bash，一般开头可以考虑加一行注释,目前的体验是加不加没卵区别.这里单行注释就是#开头就行了。多行注释目前没用到过

```
#! bin/bash
```

然后作为可执行文件运行的话

```
chmod +x test.sh
./test.sh
```

另外的也ok

```
bash test.sh
```

### 2 基础语法

#### 2.1 变量

**变量的定义**比较简单，无论加不加引号，其实都是字符串。另外等号两边不要有空格，有就裂开。

```
first_name=simon
last_name="han"
QAQ='~!uehrtls'

echo ${first_name},${last_name},${QAQ}
```

使用**取值**都要加`$`，建议加`${}`,方便判定边界。

变量是可以**删除**的，`unset`可以删除变量，输出删除变量会输出空行

```
unset QAQ
echo $QAQ	
```

只读变量，不能修改，可以选用如下的设置一个只读变量

```
readonly QAQ
declare -r QAQ
```

#### 2.2 环境变量

1. 自定义变量（局部变量）
   子进程不能访问的变量
2. 环境变量（全局变量）
   子进程可以访问的变量

```
name=yxc  # 定义变量
export name  # 第一种方法
declare -x name  # 第二种方法
```

环境变量改成自定义变量也比较简单

```
export name=yxc  # 定义环境变量
declare +x name  # 改为自定义变量
```

#### 2.3 默认变量

文件参数变量属于默认变量，可以向脚本传递参数

```
$0 是文件名
$1 是第一个参数
$2 是第二个参数
....
```

还有一些乱七八糟的变量

| 参数       | 说明                                                         |
| ---------- | ------------------------------------------------------------ |
| $#         | 代表文件传入的参数个数，如上例中值为4                        |
| $*         | 由所有参数构成的用空格隔开的字符串，如上例中值为"$1 $2 $3 $4" |
| $@         | 每个参数分别用双引号括起来的字符串，如上例中值为"$1" "$2" "$3" "$4" |
| $$         | 脚本当前运行的进程ID                                         |
| $?         | 上一条命令的退出状态（注意不是stdout，而是exit code）。0表示正常退出，其他值表示错误 |
| $(command) | 返回command这条命令的stdout                                  |
| \`command` | 返回command这条命令的stdout                                  |

#### 2.4 字符串

字符串单引号双引号都可以，区别在于：

1. 单引号中的内容会原样输出，不会执行、不会取变量；
2. 双引号中的内容可以执行、可以取变量,有空格的时候，`${str}`一定要加双引号，不然有空格就被识别成多个字符串

```
输入：
	name=QAQ
	echo '$name'
	echo "$name"
输出：
	$name
	QAQ
```

获取字符串长度

```
echo ${#name}
```

提取子串,前闭后闭，长度最多到结尾，写的再大都没事，反正只会到结尾。

```
echo ${name:1:1}
```

#### 2.5 数组

只支持一维数组，也够用了，本来就是为了处理一些简单的任务.

```bash
# 数组用小括号表示，元素之间用空格隔开
arr=(1 2 3)
# 读取数组中某个元素
${array[index]}
# 读取整个数组
${array[@]}  # 第一种写法
${array[*]}  # 第二种写法
# 数组长度
${#array[@]}  # 第一种写法
${#array[*]}  # 第二种写法
```

#### 2.5 expr

就是一个表达式求值，要拿到他的返回值就要$(expr)或者\`expr  `,值得注意的是

1. 用空格隔开每一项
2. 用反斜杠放在shell特定的字符前面（发现表达式运行错误时，可以试试转义）
3. 对包含空格和其他特殊字符的字符串要用引号括起来
4. expr会在stdout中输出结果。如果为逻辑关系表达式，则结果为真，stdout为1，否则为0。
5. expr的exit code：如果为逻辑关系表达式，则结果为真，exit code为0，否则为1。

```
expr 表达式
```

有几种表达式：

**字符串表达式**

+ `length STRING`:返回STRING的长度
+ `index STRING CHARSET`:`CHARSET`中任意单个字符在`STRING`中最前面的字符位置，下标从1开始。如果在`STRING`中完全不存在`CHARSET`中的字符，则返回0。
+ `substr STRING POSITION LENGTH`:返回`STRING`字符串中从`POSITION`开始，长度最大为`LENGTH`的子串。如果`POSITION`或`LENGTH`为负数，0或非数值，则返回空字符串。

```bash
str="hello world"
echo `expr length "$str"`
echo `expr index "${str}" ak`
echo `expr substr "${str}" 2 3`
```

**整数表达式**

算术表达式优先级低于字符串表达式，高于逻辑关系表达式。

1. `+ -`:加减运算。两端参数会转换为整数，如果转换失败则报错
2. `* / %`:乘，除，取模运算。两端参数会转换为整数，如果转换失败则报错。
3. `() `:可以该表优先级，但需要用反斜杠转义

```
a=3
b=4
echo `expr $a + $b`  # 输出7
echo `expr $a - $b`  # 输出-1
echo `expr $a \* $b`  # 输出12，*需要转义
echo `expr $a / $b`  # 输出0，整除
echo `expr $a % $b` # 输出3
echo `expr \( $a + 1 \) \* \( $b + 1 \)`  # 输出20，值为(a + 1) * (b + 1)
```

**逻辑表达式**

1. `|`,如果第一个参数非空且非0，则返回第一个参数的值，否则返回第二个参数的值，但要求第二个参数的值也是非空或非0，否则返回0。如果第一个参数是非空或非0时，不会计算第二个参数。
2. `&`,如果两个参数都非空且非0，则返回第一个参数，否则返回0。如果第一个参为0或为空，则不会计算第二个参数。
3. `< <= = == != >= >`,比较两端的参数，如果为true，则返回1，否则返回0。”==”是”=”的同义词。”expr”首先尝试将两端参数转换为整数，并做算术比较，如果转换失败，则按字符集排序规则做字符比较。
4. `()` 可以该表优先级，但需要用反斜杠转义

```
a=3
b=4

echo `expr $a \> $b`  # 输出0，>需要转义
echo `expr $a '<' $b`  # 输出1，也可以将特殊字符用引号引起来
echo `expr $a '>=' $b`  # 输出0
echo `expr $a \<\= $b`  # 输出1

c=0
d=5

echo `expr $c \& $d`  # 输出0
echo `expr $a \& $b`  # 输出3
echo `expr $c \| $d`  # 输出5
echo `expr $a \| $b`  # 输出3
```

#### 2.6 read

基本就相当于cin，读到文件结束符时，exit code时1，否则是0.可以加参数

+ `-p`：一个字符串的提示信息
+ `-t`：秒数，输入等待时间

```
han@han-server:/home/space/share/bash$ read -t 3
han@han-server:/home/space/share/bash$ read -t 3 -p "input something"
input something3
han@han-server:/home/space/share/bash$ read -t 3 -p "input something: " name
input something: 3
han@han-server:/home/space/share/bash$ echo $name
3
```

#### 2.7 echo

输出字符串

```bash
# 普通字符串
echo "Hello AC Terminal"
echo Hello AC Terminal  # 引号可以省略

# 转义字符串
echo "\"Hello AC Terminal\""  # 注意只能使用双引号，如果使用单引号，则不转义
echo \"Hello AC Terminal\"  # 也可以省略双引号

# 显示变量
name=3
echo "my name is ${name}"

# 显示换行
echo -e "Hi\n"  # -e 开启转义

#显示不换行
echo -e "Hi \c" # -e 开启转义 \c 不换行
echo "hhh"

# 结果定向到文件
echo "Hello World" > output.txt  # 将内容以覆盖的方式输出到output.txt中

# 原样输出字符串，单引号
name=hh
echo '$name\"'

# 显示命令执行的结果
echo `ls`
```

#### 2.9 printf

一个格式化输出的命令，默认不在末尾添加换行

```bash
# 用法
printf format-string [arguments...]

# 例子
printf "%10d!\n" 123  # 占10位，右对齐
printf "%-10.2f!\n" 123.123321  # 占10位，保留2位小数，左对齐
printf "My name is %s\n" "icarry"  # 格式化输出字符串
printf "%d * %d = %d\n"  2 3 `expr 2 \* 3` # 表达式的值作为参数
```

####  2.10 test命令于判断符号[ ]

**逻辑运算符`&&`和`||`**

+ && 表示与，|| 表示或
+ 二者具有短路原则：
+ `expr1 && expr2`：当`expr1`为假时，直接忽略`expr2`
  `expr1 || expr2`：当`expr1`为真时，直接忽略`expr2`
+ 表达式的exit code为0，表示真；为非零，表示假。

**test命令**

test命令用于判断文件类型，以及对变量做比较。

test命令用exit code返回结果，而不是使用`stdout`。0表示真，非0表示假。

```
test 2 -lt 3  # 为真，返回值为0
echo $?  # 输出上个命令的返回值，输出0
```

**文件类型判断**

| 测试参数 | 含义           |
| -------- | -------------- |
| -e       | 文件是否存在   |
| -f       | 是否为文件     |
| -d       | 是否为文件目录 |

```
test -e thread_test.cpp
echo $?
# 还可以利用短路原则输出
test -e test2.sh && echo "exist" || echo "Not exist"
```

**文件权限判断**

| 测试参数 | 含义           |
| -------- | -------------- |
| -r       | 文件是否可读   |
| -w       | 文件是否可写   |
| -x       | 文件是否可执行 |
| -s       | 是否为非空文件 |

**整数间比较**

| 测试参数 | 代表意义       |
| -------- | -------------- |
| -eq      | a是否等于b     |
| -ne      | a是否不等于b   |
| -gt      | a是否大于b     |
| -lt      | a是否小于b     |
| -ge      | a是否大于等于b |
| -le      | a是否小于等于b |

```
test $a -eq $b  # a是否等于b
```

**字符串比较**

| 测试参数          | **代表意义**                                           |
| ----------------- | ------------------------------------------------------ |
| test -z STRING    | 判断STRING是否为空，如果为空，则返回true               |
| test -n STRING    | 判断STRING是否非空，如果非空，则返回true（-n可以省略） |
| test str1 == str2 | 判断str1是否等于str2                                   |
| test str1 != str2 | 判断str1是否不等于str2                                 |

**多重判定**

| **测试参数** | **代表意义**                                        |
| ------------ | --------------------------------------------------- |
| -a           | 两条件是否同时成立                                  |
| -o           | 两条件是否至少一个成立                              |
| !            | 取反。如 test ! -x file，当file不可执行时，返回true |

```bash
# -a 连接
test -r  thread_test.cpp -a -w thread_test.cpp 
```

**判断符号**

`[ ]`和test用法一样，一般用在条件判断语句中。需要注意的是

+ `[]`内的每一项都要用空格隔开（首位也要有空格）
+ 中括号内的变量，最好用双引号括起来
+ 中括号内的常数，最好用单或双引号括起来

```bash
[$name == "s han"] # 错误，等价于 [s han == "s han"]
["$name" == "s han"] # right
```

### 3 判断语句

#### if...then

```bash
#单层if
if condition
then
    语句1
    语句2
    ...
fi

## if-else
if condition
then
    语句1
    语句2
    ...
else
    语句1
    语句2
    ...
fi

# 多层if-elif-else
if condition
then
    语句1
    语句2
    ...
elif condition
then
    语句1
    语句2
    ...
elif condition
then
    语句1
    语句2
else
    语句1
    语句2
    ...
fi
```

#### case...esac,类似于switch

```
case $变量名称 in
    值1)
        语句1
        语句2
        ...
        ;;  # 类似于C/C++中的break
    值2)
        语句1
        语句2
        ...
        ;;
    *)  # 类似于C/C++中的default
        语句1
        语句2
        ...
        ;;
esac
```

例子

```
a=4

case $a in
    1)
        echo ${a}等于1
        ;;  
    2)
        echo ${a}等于2
        ;;  
    3)                                                
        echo ${a}等于3
        ;;  
    *)
        echo 其他
        ;;  
esac
```

### 4 循环语句

```bash
for i in $(seq 1 10)
do
    echo $i
done# for...in...do...done
for i in a 2 cc
do
    echo $i
done
# expr做it
for file in `ls`
do
    echo $file
done

# 输出1-10
for i in $(seq 1 10)
do
    echo $i
done
```

另外还有`for ((…;…;…)) do…done`格式

```
for ((expression; condition; expression))
do
    语句1
    语句2
done
```

比如输出1-10

```
for ((i=1; i<=10; i++))
do
    echo $i
done
```

**while…do…done循环**

```bash
# 文件结束符为Ctrl+d，输入文件结束符后read指令返回false。
while read -p "input something:" name
do 
	echo $name
done
```

**until…do…done循环**

```bash
# 示例，当用户输入yes或者YES时结束，否则一直等待读入。
until [ "${word}" == "yes" ] || [ "${word}" == "YES" ]
do
    read -p "Please input yes/YES to stop this program: " word
done
```

**break**

```bash
# 跳出当前一层循环，注意与C/C++不同的是：break不能跳出case语句。
while read name
do
    for ((i=1;i<=10;i++))
    do
        case $i in
            8)
                break
                ;;
            *)
                echo $i
                ;;
        esac
    done
done
```

**continue命令**

```bash
for ((i=1;i<=10;i++))
do
    if [ `expr $i % 2` -eq 0 ]
    then
        continue
    fi
    echo $i
done
```

### 5 函数

但return的返回值是exit code，取值为0-255，0表示正常结束。如果想获取函数的输出结果，可以通过echo输出到stdout中，然后通过$(function_name)来获取stdout中的结果。

```
[function] func_name() {  # function关键字可以省略
    语句1
    语句2
    ...
}
```

不获取 return值和stdout值

```
func() {
    name=hh
    echo "Hello $name"
}

func
# 输出
Hello hh
```

获取 return值和stdout值

```
func() {
    name=hh
    echo "Hello $name"

    return 123
}

output=$(func)
ret=$?

echo "output = $output"
echo "return = $ret"
```

**函数的输入参数**

在函数内，$1表示第一个输入参数，$2表示第二个输入参数，依此类推。

```
func() {  # 递归计算 $1 + ($1 - 1) + ($1 - 2) + ... + 0
    if [ $1 -le 0 ] 
    then
        echo 0
        return 0
    fi  

    sum=$(func $(expr $1 - 1))
    echo $(expr $sum + $1)
}

echo $(func 10)
```

### 6 exit命令

exit命令用来退出当前shell进程，并返回一个退出状态；使用$?可以接收这个退出状态。

exit命令可以接受一个整数值作为参数，代表退出状态。如果不指定，默认状态值是 0。

exit退出状态只能是一个介于 0~255 之间的整数，其中只有 0 表示成功，其它值都表示失败

```
#! /bin/bash

if [ $# -ne 1 ]  # 如果传入参数个数等于1，则正常退出；否则非正常退出。
then
    echo "arguments not valid"
    exit 1
else
    echo "arguments valid"
    exit 0
fi
```

### 7 文件重定向

每个进程默认打开3个文件描述符：

- `stdin`标准输入，从命令行读取数据，文件描述符为0
- `stdout`标准输出，向命令行输出数据，文件描述符为1
- `stderr`标准错误输出，向命令行输出数据，文件描述符为2

可以用文件重定向将这三个文件重定向到其他文件中。

| 命令               | 说明                                      |
| :----------------- | :---------------------------------------- |
| `command > file`   | 将`stdout`重定向到`file`中                |
| `command < file`   | 将`stdin`重定向到`file`中                 |
| `command >> file`  | 将`stdout`以追加方式重定向到`file`中      |
| `command n> file`  | 将文件描述符`n`重定向到`file`中           |
| `command n>> file` | 将文件描述符`n`以追加方式重定向到`file`中 |

```
echo -e "Hello \c" > output.txt  # 将stdout重定向到output.txt中
echo "World" >> output.txt  # 将字符串追加到output.txt中

read str < output.txt  # 从output.txt中读取字符串

echo $str  # 输出结果：Hello World
```

**同时重定向**stdin和stdout

```
chmod +x test.sh  # 添加可执行权限
./test.sh < input.txt > output.txt  # 从input.txt中读取内容，将输出写入output.txt中
cat output.txt  # 查看output.txt中的内容
7
```

### 8 引入外部文件

```\
source filename
```

