# README
本文件为`wait`和`waitpid`函数学习文件。
## 文件说明
+ `wait_test.c`、`wait_test`、 `child_process_test.c`和`child_process_test`为`wait`函数学习文件。
+ `waitpid_test.c`和`waitpid_test`为`waitpid`函数学习文件。
+ `practice_waitpid.c`、`practice_waitpid`、`waitpid_child_abnormal.c`、`waitpid_child_abnormal`、`waitpid_child_normal.c`、`waitpid_child_normal`和`practice_test.txt`为本函数练习。
## 练习题设计要求
+ 父进程fork 3个子进程，3个子进程第一个调用ps命令，第二个调用自定义程序(正常)，第三个调用自定义程序2(会出现段错误),父进程使用waitpid对子进程进行回收。
## LISENCE
The Edit is wl.