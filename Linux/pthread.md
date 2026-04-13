# 线程

## 定义
线程是：进程中的一个执行单元，是操作系统进行 CPU 调度的基本单位。

## 与进程的关系
- 线程必须依附于进程存在。
- 一个进程可以有多个线程。
- 同一进程内线程共享：
  - 地址空间（堆、全局变量）
  - 打开的文件

## 头文件
以下操作需要头文件：
```c
#include <pthread.h>
````

## 创建线程：pthread_create

### 参数

* `pthread_t *thread`
  输出参数，返回线程 ID（可用 `_equal()` 检查是否相同）。

* `const pthread_attr_t *attr`
  线程属性（一般传 `NULL`，使用默认）。

* `void *(*start_routine)(void *)`
  线程执行函数（入口函数）。

* `void *arg`
  传给线程函数的参数（`void` 进函数再转换为需要的类型）。

### 返回值

* 返回 `0` 为成功。
* 非 `0` 为失败。

## 终止线程

* 线程函数正常 `return`。
* 主函数 `return` / 某处调用 `exit`。
* 线程调用 `pthread_exit`。
* 外部调用 `pthread_cancel`。

## 连接终止线程（joining）

* 调用 `pthread_join` 来等待对应 ID 的线程终止，并获取线程返回值的拷贝。
* 若线程不处于分离状态，必须使用 `pthread_join()` 来清理资源，防止僵尸线程出现。
* 如果不关心线程退出时的状态，可以调用 `pthread_detach()` 将其转换为分离状态，此时其退出会自动清理资源。

```
```

