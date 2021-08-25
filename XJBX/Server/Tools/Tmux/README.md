## Tmux使用到放弃

Tmux的优势是在terminal的基础上多了分屏和session，这样在terminal爆退之后，session保留的话其实还是能跑很多内容的。

### 0 安装

```
sudo apt install tmux
```

### 1 概念

主要关注三个概念，session，window，panel。

1. session，看做一个进程
2. window，进程中的窗口
3. panel，window中开了一个terminal

### 2 前缀键

``` 
ctrl + b (松开) + key
```

这个前缀键的配置可以通过.tmux.conf重新设置

### 3 session

```bash
## 创建
tmux # 没有灵魂的新建一个session
tmux new -s <session-name> # 有灵魂的创建一个session

## 离开
tmux detach # 在session中，离开这个session，快捷键是ctrl + b->d

## 重新进入
tmux ls # 查看有几个session
tmux attach -t <session-name> # 回到有灵魂的session
tmux a # 没灵魂的直接回到最近的

## 关闭session
tmux kill-session -t <name> # ctrl + d

## 切换session
tmux switch -t <session-name> # ctrl + b -> s

## 重命名session
tmux rename-session -t <old-session-name> <new-session-name> # ctrl + b -> &
```

### 4 panel

选中文本的时候需要加shift键

```bash
## 切割窗格
tmux split-window  # 上下 ctrl + b -> "
tmux split-window -h # 左右 ctrl + b -> %

## 移动窗格
tmux select-pane -U
tmux select-pane -D
tmux select-pane -L
tmux select-pane -R
# ctrl + b -> <arrow key>

## 交换窗格位置
tmux swap-pane -U
tmux swap-pane -D

## 关闭当前panel
ctrl + b x
 
## 放大
ctrl + b z 

## 显示时间
ctrl + b t
```

### 5 window

用的少

```bash
## 创建窗口
tmux new-window -n <window-name> # ctrl + b c,

## 切换窗口
tmux select-window -t <window-name> 
# ctrl + b w 显示窗口列表可以通过 j ,k 上下进行选择窗口，然后回车进入指定的窗口。
# ctrl + b n 快速切换到下一个窗口。
# ctrl +b p 快速切换到上一个窗口。

## 重命名窗口
tmux rename-window <new-window-name> # ctrl + b , 该快捷键可以重新命名窗口

## 关闭窗口
tmux kill-window -t <window-name> # ctrl + b & 可以关闭当前的的窗口
```

### help&conf

tmux 中的很多操作都是通过快捷键去实现的 ，在 tmux 中我们可以通过 **ctrl + b ?** 来查找 tmux 的帮助文档

~下的 .tmux.conf支持修改

```bash
#below reset tmux prefix command key
set -g prefix C-x
unbind C-b
bind C-x send-prefix
#set swap pane key
bind-key k select-pane -U
bind-key j select-pane -D
bind-key h select-pane -L
bind-key l select-pane -R
```

