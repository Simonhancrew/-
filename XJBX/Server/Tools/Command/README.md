## Linux后台开发相关的命令

### grep

可以查找某些关键词

```
han@han-server:/home/space/Coding$ ls -r
 XJBX        Simonhancrew.github.io   PedestrianAttributeRecognition   MachineLearningAlgorithm   demo                 AwesomeRust     Awesomecpp   ACL4SSR
 workspace   shell                   'Object Detection'                FundementalADT             CenterTrack-master   AwesomePython   AfterGoPL
 v2rayL      PytorchLearning          MarketDuke                       Dive-into-DL-PyTorch       back                 Awesomego       ADT
han@han-server:/home/space/Coding$ ls | grep she
shell
```

### find

查找，支持模糊匹配

```
find . ‐name FILE_NAME 
find /etc ‐maxdepth 1 ‐name passwd     ##查找/etc/下名称中带有passwd的文件，查找一层
find /mnt ‐size 20K       ##查找/mnt文件大小近似20k的文件
find /mnt ‐size +20K      ##查找/mnt文件大小大于20k的文件
find /mnt ‐size ‐20K      ##查找/mnt文件大小小于20k的文件
find /etc ‐maxdepth 2 ‐mindepth 2 ‐name *.conf  ##查找/etc/下名称中带有*.conf的文件，且只查找第二层
find /mnt ‐type d         ##按type查找/mnt中目录
find /mnt ‐type f         ##按type查找/mnt中文件
find /mnt ‐cmin 10        ##查找/mnt中十分钟左右修改的
find /mnt ‐cmin +10       ##查找/mnt中十分钟以上修改的
find /mnt ‐cmin ‐10       ##查找/mnt中十分钟以内修改的
find /mnt ‐ctime 10       ##查找/mnt中十天左右修改的
find /mnt ‐ctime +10      ##查找/mnt中十天以上修改的
find /mnt ‐ctime ‐10      ##查找/mnt中十天以内修改的
```

### uptime

查看机器的启动时间、登录用户、平均负载等情况，通常用于在线上应急或者技术攻关中，确定操作系统的重启时间。

```
han@han-server:/home/space/Coding$ uptime 
 09:57:29 up 8 days, 22:26,  2 users,  load average: 0.00, 0.00, 0.00
```

### ulimit

Linux系统对每个登录的用户都限制其最大进程数和打开的最大文件句柄数。为了提高性能，可以根据硬件资源的具体情况设置各个用户的最大进程 数和打开的最大文件句柄数。可以用ulimit -a来显示当前的各种系统对用户使用资源的限制:

```
han@han-server:/home/space/Coding$ ulimit -a
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 47257
max locked memory       (kbytes, -l) 65536
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 47257
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```

设置用户的最大进程数： ulimit -u 1024
设置用户可以打开的最大文件句柄数： ulimit -n 65530

### curl http

```
curl ‐i "http://www.sina.com" # 打印请求响应头信息
curl ‐I "http://www.sina.com" # 仅返回http头
curl ‐v "http://www.sina.com" # 打印更多的调试信息
curl ‐verbose "http://www.sina.com" # 打印更多的调试信息
curl ‐d 'abc=def' "http://www.sina.com" # 使用post方法提交http请求
curl ‐sw '%{http_code}' "http://www.sina.com" # 打印http响应码
```

### ps

ps用一于显示系统内的所有进程。

```
han@han-server:/home/space/Coding$ ps -ef | grep nginx
root       66654       1  0 7月17 ?       00:00:00 nginx: master process ./sbin/nginx
root       66655   66654  0 7月17 ?       00:00:00 nginx: worker process
han       162007  161882  0 10:00 pts/0    00:00:00 grep --color=auto nginx
```

### pidstat进程资源

pidstat用于监控全部或指定的进程占用系统资源的情况，包括CPU、内存、磁盘I/O、程切换、线程数等数据。

`pidstat ­-urd -­p 进程号`

```
han@han-server:/home/space/Coding$ pidstat -urd -p 66654
Linux 5.4.0-77-generic (han-server) 	2021年07月21日 	_x86_64_	(4 CPU)
#cpu -u
10时02分09秒   UID       PID    %usr %system  %guest   %wait    %CPU   CPU  Command
10时02分09秒     0     66654    0.00    0.00    0.00    0.00    0.00     1  nginx
#mem -r
10时02分09秒   UID       PID  minflt/s  majflt/s     VSZ     RSS   %MEM  Command
10时02分09秒     0     66654      0.00      0.00    4604     392   0.00  nginx
# i/o -d
10时02分09秒   UID       PID   kB_rd/s   kB_wr/s kB_ccwr/s iodelay  Command
10时02分09秒     0     66654     -1.00     -1.00     -1.00       0  nginx
```

### free

此命令用于显示系统内存的使用情况，包括总体内存、己经使用的内存；还可用于显示系统内核使用的缓冲区，包括缓冲（buffer）和缓存（cache） 等。

```
han@han-server:/home/space/Coding$ free
              总计         已用        空闲      共享    缓冲/缓存    可用
内存：    12156732     2709632     2853360      154336     6593740     8970964
交换：     2097148           0     2097148
han@han-server:/home/space/Coding$ free -lh
              总计         已用        空闲      共享    缓冲/缓存    可用
内存：        11Gi       2.6Gi       2.7Gi       150Mi       6.3Gi       8.6Gi
低：         11Gi       8.9Gi       2.7Gi
高：           0B          0B          0B
交换：       2.0Gi          0B       2.0Gi
```

### vmstat

此命令显示关于内核线程、虚拟内存、磁盘I/O 、陷阱和CPU占用率的统计信息。

```
han@han-server:/home/space/Coding$ vmstat
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b 交换 空闲 缓冲 缓存   si   so    bi    bo   in   cs us sy id wa st
 0  0      0 2855676  97416 6496376    0    0     1     5   10   11  0  0 99  0  0

```

### mpstat CPU信息

此命令用于实时监控系统CPU的一些统计信息，这些信息存放在/proc/stat文件中，在多核CPU系统里，不但能查看所有CPU的平均使用信息，还能 查看某个特定CPU的信息。

```
han@han-server:/home/space/Coding$ mpstat -P ALL
Linux 5.4.0-77-generic (han-server) 	2021年07月21日 	_x86_64_	(4 CPU)

10时05分35秒  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
10时05分35秒  all    0.30    0.00    0.18    0.04    0.00    0.01    0.00    0.00    0.00   99.47
10时05分35秒    0    0.31    0.01    0.17    0.08    0.00    0.01    0.00    0.00    0.00   99.42
10时05分35秒    1    0.28    0.00    0.23    0.02    0.00    0.00    0.00    0.00    0.00   99.46
10时05分35秒    2    0.31    0.01    0.18    0.02    0.00    0.01    0.00    0.00    0.00   99.47
10时05分35秒    3    0.28    0.00    0.15    0.02    0.00    0.01    0.00    0.00    0.00   99.53
```

### iostat IO状态

该命令用于监控CPU占用率、平均负载值及I/O读写速度等

```
han@han-server:/home/space/Coding$ iostat 
Linux 5.4.0-77-generic (han-server) 	2021年07月21日 	_x86_64_	(4 CPU)

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           0.30    0.00    0.19    0.04    0.00   99.47

Device             tps    kB_read/s    kB_wrtn/s    kB_dscd/s    kB_read    kB_wrtn    kB_dscd
loop0             0.01         0.02         0.00         0.00      11652          0          0
loop1             0.00         0.00         0.00         0.00       1062          0          0
loop10            0.00         0.00         0.00         0.00          4          0          0
loop2             0.00         0.00         0.00         0.00        349          0          0
loop3             0.00         0.00         0.00         0.00       1054          0          0
loop4             0.00         0.00         0.00         0.00        339          0          0
loop5             0.00         0.00         0.00         0.00       1044          0          0
loop6             0.00         0.00         0.00         0.00        337          0          0
loop7             0.00         0.00         0.00         0.00       1069          0          0
loop8             0.00         0.00         0.00         0.00       1073          0          0
loop9             0.00         0.00         0.00         0.00       1067          0          0
sda               0.04         1.03         0.20         0.00     794052     153240          0
sdb               1.66         3.52        19.79       118.39    2716121   15286821   91458044
```

### df硬盘使用情况

```
han@han-server:/home/space/Coding$ df -h
文件系统        容量  已用  可用 已用% 挂载点
udev            5.8G     0  5.8G    0% /dev
tmpfs           1.2G  3.6M  1.2G    1% /run
/dev/sdb2       117G   43G   68G   39% /
tmpfs           5.8G     0  5.8G    0% /dev/shm
tmpfs           5.0M  4.0K  5.0M    1% /run/lock
tmpfs           5.8G     0  5.8G    0% /sys/fs/cgroup
/dev/loop0      100M  100M     0  100% /snap/core/11316
/dev/loop2       56M   56M     0  100% /snap/core18/2074
/dev/loop4       56M   56M     0  100% /snap/core18/2066
/dev/loop3       65M   65M     0  100% /snap/gtk-common-themes/1514
/dev/loop1      100M  100M     0  100% /snap/core/11187
/dev/loop5      219M  219M     0  100% /snap/gnome-3-34-1804/72
/dev/loop6       52M   52M     0  100% /snap/ffmpeg/821
/dev/loop7       66M   66M     0  100% /snap/gtk-common-themes/1515
/dev/loop8      219M  219M     0  100% /snap/gnome-3-34-1804/66
/dev/loop9       81M   81M     0  100% /snap/ffmpeg/1286
/dev/sdb1       511M  7.9M  504M    2% /boot/efi
/dev/sda5       458G   98G  337G   23% /home/space
tmpfs           1.2G   20K  1.2G    1% /run/user/124
tmpfs           1.2G   52K  1.2G    1% /run/user/1000
```

### du目录文件大小

```
han@han-server:/home/space/Coding$ du -lh --max-depth=1
21M	./AwesomeRust
136K	./shell
40K	./Awesomego
1.4M	./AfterGoPL
540K	./demo
1.1G	./MarketDuke
2.3M	./PedestrianAttributeRecognition
8.0K	./workspace
932K	./Simonhancrew.github.io
1.5M	./Awesomecpp
19M	./ADT
80M	./Dive-into-DL-PyTorch
22M	./ACL4SSR
484K	./FundementalADT
1.4M	./MachineLearningAlgorithm
1.4M	./PytorchLearning
25M	./Object Detection
219M	./back
8.0K	./.ipynb_checkpoints
56K	./AwesomePython
31M	./CenterTrack-master
177M	./v2rayL
38M	./XJBX
1.7G	.
```

### nc

nc是NetCat的简称，在网络调试工具中享有“瑞士军刀”的美誉，此命令功能丰富、短小精悍、简单实用，被设计成一款易用的网络工具，可通过 TCP/LJDP传输数据。

```
-l    用于指定nc将处于侦听模式。指定该参数，则意味着nc被当作server，侦听并接受连接，而非向其它地址发起连接。 
-p    暂未用到（老版本的nc可能需要在端口号前加-p参数，下面测试环境是centos6.6，nc版本是nc-1.84，未用到-p参数） 
-s    指定发送数据的源IP地址，适用于多网卡机 
-u    指定nc使用UDP协议，默认为TCP 
-v    输出交互或出错信息，新手调试时尤为有用 
-w    超时秒数，后面跟数字
```

具体

```
nc ‐l 9999                        # 开启一个本地9999的TCP协议端口，由客户端主动发起连接，一旦连接必须由服务端发起关闭 nc ‐vw 2 129.204.197.215 11111     # 通过nc去访问129.204.197.215主机的11111端口，确认是否存活；可不加参数
nc ‐ul 9999                       # 开启一个本地9999的UDP协议端口，客户端不需要由服务端主动发起关闭 nc 129.204.197.215 9999 < test     # 通过129.204.197.215的9999TCP端口发送数据文件
nc ‐l 9999 > zabbix.file          # 开启一个本地9999的TCP端口，用来接收文件内容 # 测试网速
A机器操作如下：
nc ‐l 9999 > /dev/null # B机器开启数据传输
nc 129.204.197.215 9999 </dev/zero # A机器进行网络监控
sar ‐n DEV 2 100000
```

### nslookup

这是一款检测网络中DNS服务器能否正确解析域名的工具命令，并且可以输出

```
han@han-server:/home/space/Coding$ nslookup baidu.com
Server:		127.0.0.53
Address:	127.0.0.53#53

Non-authoritative answer:
Name:	baidu.com
Address: 220.181.38.148
Name:	baidu.com
Address: 39.156.69.79
```

### sar

sar是一个多功能的监控工具，使用简单，不需要管理员权限，可以输出每秒的网卡存取速度，适合线上排查问题时使用。 

```
han@han-server:/home/space/Coding$ sar -n DEV 1 1
Linux 5.4.0-77-generic (han-server) 	2021年07月21日 	_x86_64_	(4 CPU)

10时12分26秒     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s   %ifutil
10时12分27秒    wlp5s0      1.00      1.00      0.05      0.19      0.00      0.00      0.00      0.00
10时12分27秒    enp4s0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
10时12分27秒        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00

平均时间:     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s   %ifutil
平均时间:    wlp5s0      1.00      1.00      0.05      0.19      0.00      0.00      0.00      0.00
平均时间:    enp4s0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
平均时间:        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
```

### netstat
此命令显示网络连接、端口信息等，另外一个命令ss与netstat命令类似，不再单独介绍。

```
han@han-server:/home/space/Coding$ sudo netstat -nap | grep 66654
tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN      66654/nginx: master 
unix  3      [ ]         流        已连接     615834   66654/nginx: master  
unix  3      [ ]         流        已连接     615833   66654/nginx: master  
han@han-server:/home/space/Coding$ ps -ef | grep nginx
root       66654       1  0 7月17 ?       00:00:00 nginx: master process ./sbin/nginx
root       66655   66654  0 7月17 ?       00:00:00 nginx: worker process
han       162094  161882  0 10:14 pts/0    00:00:00 grep --color=auto nginx
```

### lsof

lists openfiles,万物皆文件

```
han@han-server:/home/space/Coding$ sudo lsof -i:80
COMMAND   PID USER   FD   TYPE  DEVICE SIZE/OFF NODE NAME
nginx   66654 root    6u  IPv4  616679      0t0  TCP *:http (LISTEN)
nginx   66655 root    3u  IPv4 1116358      0t0  TCP han-server:http->DESKTOP-NTBNO3S:55817 (ESTABLISHED)
nginx   66655 root    6u  IPv4  616679      0t0  TCP *:http (LISTEN)
nginx   66655 root   10u  IPv4 1116359      0t0  TCP han-server:http->DESKTOP-NTBNO3S:64684 (ESTABLISHED)
```

### 关于proc文件

Linux系统内核提供了通过/proc文件系统查看运行时内核内部数据结构的能力，也可以改变内核参数设置。 显示CPU信息：

```
cat /proc/cpuinfo 
```

显示内存信息：

```
cat /proc/meminfo
```

显示详细的内存映射信息： 

```
cat /proc/zoneinfo 
```

显示磁盘映射信息：

```
 cat /proc/mounts
```


查看系统平均负载命令：

```
 cat /proc/loadavg
```

