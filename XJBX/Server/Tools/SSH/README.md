## SSH

ssh的一些操作其实还是挺简单的，重点关注与一下别名和免密登录

最基本的操作就是

```
ssh user@hostname
```

+ `user`:用户名

+ `hostname`：ip地址

第一次登录的时候会提示一大堆乱七八糟的东西，输入`yes`就可以了。远端服务器的信息会被存到`~/.ssh/known_host`文件中

当然你也可以选择特定的端口使用

```
ssh user@hostname -p 22
```

### 配置文件别名

一般配置在`~/.ssh/config`中，没有就自己创建一个

然后配置的别名,注意全程无冒号

```
Host myserver1
    HostName IP地址或域名
    User 用户名
```

之后登录就直接用

```
ssh myserver
```

### 免密登录

首先是要创建密钥,中间过程我是一直回车确定，使用默认配置的

```
ssh-keygen
```

执行结束之后，`~/.ssh/`下会多出两个密钥文件

+ `id_rsa`：私钥
+ `id_rsa.pub`：公钥

切记个人私钥不要泄露了，之后需要免密登录那个服务器就将公钥复制到

```
~/.ssh/authorized_keys
```

也可以使用下面的命令一键添加

```
ssh-copy-id myserver
```

### SCP

传文件的命令，也比较基础。

```
scp source destination
```

将`source`路径下的文件复制到`destination`中

一次复制多个文件：

```bash
scp source1 source2 destination
```

复制文件夹：

```
scp -r ~/tmp myserver:/home/acs/
```

将本地家目录中的`tmp`文件夹复制到`myserver`服务器中的`/home/acs/`目录下。

```
scp -r ~/tmp myserver:homework/
```

将本地家目录中的tmp文件夹复制到myserver服务器中的~/homework/目录下。

另外也是可以指定端口号的

指定服务器的端口号：

```
scp -P 22 source1 source2 destination
```

> note:  `scp`的`-r -P`等参数尽量加在`source`和`destination`之前。

### 使用`scp`配置其他服务器的`vim`和`tmux`

```
scp ~/.vimrc ~/.tmux.conf myserver:
```

