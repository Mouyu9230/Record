# 线程（C++ std::thread）

## 定义

`std::thread` 是 C++11 提供的线程库，用于创建和管理线程，是对底层线程（如 pthread）的封装，具有类型安全和更高易用性。

---

## 与进程的关系

* 线程依附于进程存在。
* 一个进程可以包含多个线程。
* 同一进程内线程共享：

  * 地址空间（堆、全局变量）
  * 打开的文件
* 每个线程有独立的：

  * 栈空间
  * 寄存器状态

---

## 头文件

```cpp
#include <thread>
```

---

## 创建线程：std::thread

### 参数形式

```cpp
std::thread t(可调用对象, 参数...);
```

### 示例

```cpp
void func(int x) {}

std::thread t(func, 10);
```

---

## 常用成员函数

### join（连接线程）

* 阻塞当前线程，等待子线程执行完成
* 每个线程只能 `join` 一次

```cpp
t.join();
```

---

### detach（分离线程）

* 将线程设置为后台运行
* 主线程不再管理该线程
* 线程结束后自动释放资源

```cpp
t.detach();
```

---

### joinable（是否可连接）

* 判断线程是否还能 `join` 或 `detach`

```cpp
if (t.joinable()) {
    t.join();
}
```

---

### get_id（获取线程ID）

```cpp
std::thread::id id = t.get_id();
```

---

## 当前线程操作（this_thread）

```cpp
#include <thread>
#include <chrono>
```

### 获取当前线程ID

```cpp
std::this_thread::get_id();
```

---

### 线程休眠

```cpp
std::this_thread::sleep_for(std::chrono::seconds(1));
```

---

## 线程结束方式

* 线程函数执行完毕（正常 return）
* 主线程结束（未 join 的线程会导致程序终止）
* 调用 `std::terminate`（异常情况）

---

## 线程资源管理

### 必须处理的两种方式之一：

#### 1. join

```cpp
t.join();
```

#### 2. detach

```cpp
t.detach();
```

---

### 注意

* 若既不 `join` 也不 `detach`，程序会调用 `terminate` 直接崩溃
* 推荐优先使用 `join` 管理线程生命周期

---

## 参数传递

### 值传递

```cpp
void func(int x) {}
std::thread t(func, 10);
```

---

### 引用传递（必须使用 std::ref）

```cpp
#include <functional>

void func(int& x) {}
std::thread t(func, std::ref(x));
```

---

## 多线程管理（容器）

```cpp
#include <vector>
#include <thread>

std::vector<std::thread> threads;

for (int i = 0; i < 5; i++) {
    threads.emplace_back([](){
        // 任务
    });
}

for (auto& t : threads) {
    t.join();
}
```

---

## 常见问题

### ❌ 忘记 join / detach

```cpp
std::thread t(func);
// 程序结束会崩溃
```

---

### ❌ 错误创建方式

```cpp
std::thread t(func()); // 错：函数被调用
```

---

### ❌ 数据竞争

多个线程访问共享数据需配合：

```cpp
#include <mutex>
```

---

## 对比 pthread

| 项目   | std::thread | pthread |
| ---- | ----------- | ------- |
| 语言   | C++         | C       |
| 类型安全 | ✅           | ❌       |
| 使用难度 | 低           | 高       |
| 推荐   | 现代C++       | 系统级开发   |

---

## 一句话总结

`std::thread` = **C++ 标准线程工具，安全、简洁，用于替代 pthread**

