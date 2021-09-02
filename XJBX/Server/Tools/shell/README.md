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

### 2.10 test命令于判断符号[ ]

