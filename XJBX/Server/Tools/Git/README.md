## Git使用指北

其实git在使用上是挺简单的，记住三个状态就可以了，已修改(ctrl + s)，已暂存（add），已提交（commit）

Git 项目拥有三个阶段：工作区、暂存区以及 Git 目录。

工作区是对项目的某个版本独立提取出来的内容。 这些从 Git 仓库的压缩数据库中提取出来的文件，放在磁盘上 供你使用或修改

暂存区是一个文件，保存了下次将要提交的文件列表信息，一般在 Git 仓库目录中。 按照 Git 的术语叫做“索 引”，不过一般说法还是叫“暂存区"

Git 仓库目录是 Git 用来保存项目的元数据和对象数据库的地方。 这是 Git 中最重要的部分，从其它计算机克隆 仓库时，复制的就是这里的数据。

基本的 Git 工作流程如下：

1. 基本的 Git 工作流程如下：
2. 将你想要下次提交的更改选择性地暂存，这样只会将更改的部分添加到暂存区。
3. 提交更新，找到暂存区的文件，将快照永久性存储到 Git 目录。

### 1.1. git基本概念
+ 工作区：仓库的目录。工作区是独立于各个分支的。
+ 暂存区：数据暂时存放的区域，类似于工作区写入版本库前的缓存区。暂存区是独立于各个分支的。
+ 版本库：存放所有已经提交到本地仓库的代码版本
+ 版本结构：树结构，树中每个节点代表一个代码版本。

## 1.2 git常用命令

1. `git config --global user.name xxx`：设置全局用户名，信息记录在`~/.gitconfig`文件中
2. `git config --global user.email xxx@xxx.com`：设置全局邮箱地址，信息记录在`~/.gitconfig`文件中
3. `git init`：将当前目录配置成git仓库，信息记录在隐藏的.git文件夹中
4. `git add XX`：将XX文件添加到暂存区
   + `git add .`：将所有待加入暂存区的文件加入暂存区
5. `git rm --cached XX`：将文件从仓库索引目录中删掉
6. `git commit -m "给自己看的备注信息"`：将暂存区的内容提交到当前分支
7. `git commit --amend`:重新覆盖上一次的提交
8. `git status`：查看仓库状态
9. `git diff XX`：查看XX文件相对于暂存区修改了哪些内容
10. `git log`：查看当前分支的所有版本
11. `git reflog`：查看HEAD指针的移动历史（包括被回滚的版本）
12. `git reset --hard HEAD^`或`git reset --hard HEAD~`:将代码库回滚到上一个版本
    - `git reset --hard HEAD^^`：往上回滚两次，以此类推
    - `git reset --hard HEAD~100`：往上回滚100个版本
    - `git reset --hard 版本号`：回滚到某一特定版本
13. `git reset --soft 版本号`：回滚，并且会把文件恢复到缓存区
14. `git checkout — XX`或`git restore XX`：将XX文件尚未加入暂存区的修改全部撤销
15. `git remote add origin git@git.acwing.com:xxx/XXX.git`：将本地仓库关联到远程仓库
16. `git push -u (第一次需要-u以后不需要)`：将当前分支推送到远程仓库
    - `git push origin branch_name`：将本地的某个分支推送到远程仓库
17. `git clone git@git.acwing.com:xxx/XXX.git`：将远程仓库XXX下载到当前目录下
18. `git checkout -b branch_name`：创建并切换到`branch_name`这个分支
19. `git branch`：查看所有分支和当前所处分支
20. `git checkout branch_name`：切换到`branch_name`这个分支
21. `git merge branch_name`：将分支`branch_name`合并到当前分支上
22. `git branch -d branch_name`：删除本地仓库的`branch_name`分支
23. `git branch branch_name`：创建新分支
24. `git push --set-upstream origin branch_name`：设置本地的`branch_name`分支对应远程仓库的`branch_name`分支
25. `git push -d origin branch_name`：删除远程仓库的`branch_name`分支
26. `git fetch`:访问远程仓库，从中拉取所有你还没有的数据，你将会拥有那个远程仓库中所有分支 的引用，可以随时合并或查看。
27. `git pull`：将远程仓库的当前分支与本地仓库的当前分支合并
    - `git pull origin branch_name`：将远程仓库的`branch_name`分支与本地仓库的当前分支合并
28. `git branch --set-upstream-to=origin/branch_name1 branch_name2`：将远程的`branch_name1`分支与本地的`branch_name2`分支对应
29. `git stash`：将工作区和暂存区中尚未提交的修改存入栈中
30. `git stash apply`：将栈顶存储的修改恢复到当前分支，但不删除栈顶元素
31. `git stash drop`：删除栈顶存储的修改
32. `git stash pop`：将栈顶存储的修改恢复到当前分支，同时删除栈顶元素
33. `git stash list`：查看栈中所有元素

一般先设置global config，之后做add,commit,push,clone,pull等操作

### 参考

肯定还有些许操作没有写完，但是可以在下面的书里面查到大多数的操作。

《Pro Git》 by Scott Chacon and Ben Straub