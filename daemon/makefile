# daemon 守护进程
## Docement
+ `daemond.c`学习如何创建一个守护进程
+ `daemond_parctice.c`创建一个守护进程，每隔1s获取一次系统时间，并输出至`date.txt`文件中

## Note
守护进程创建步骤：
1. 创建一个子进程 `fork()`
2. 子进程创建一个会话 `setsid()`
3. 修改进程默认工作目录 `chdir()`
4. 修改文件权限掩码值 `umask()`
5. 关闭文件描述符 0/1/2文件描述符 重定向至`/dev/null` `dup2()`
6. 实现守护进程功能
7. 退出守护进程

## LISENCE
The lisence is MIT.
