## pthread 条件变量（pthread_cond_t）

条件变量用于 **线程间等待某个条件成立**，常配合 **互斥锁 pthread_mutex_t** 使用。

典型用途：生产者-消费者模型、任务队列等待。

---

## 1. 基本类型

```c
pthread_cond_t cond;
pthread_mutex_t mutex;
```

---

## 2. 初始化 / 销毁

### 静态初始化

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

### 动态初始化

```c
pthread_cond_init(&cond, NULL);
pthread_mutex_init(&mutex, NULL);
```

### 销毁

```c
pthread_cond_destroy(&cond);
pthread_mutex_destroy(&mutex);
```

---

## 3. 核心函数

### 等待

```c
pthread_cond_wait(&cond, &mutex);
```

作用：

1. 释放 mutex
2. 阻塞等待 cond
3. 被唤醒后重新加锁 mutex

---

### 唤醒一个线程

```c
pthread_cond_signal(&cond);
```

---

### 唤醒所有线程

```c
pthread_cond_broadcast(&cond);
```

---

## 4. 标准使用模式（必须遵守）

### 等待线程（消费者）

```c
pthread_mutex_lock(&mutex);

while (condition == false) {
    pthread_cond_wait(&cond, &mutex);
}

// 条件成立后执行
pthread_mutex_unlock(&mutex);
```

关键点：

* 必须用 `while`（防止虚假唤醒）
* wait 期间自动释放锁

---

### 发送信号线程（生产者）

```c
pthread_mutex_lock(&mutex);

condition = true;

pthread_mutex_unlock(&mutex);
pthread_cond_signal(&cond);
```

---

## 5. 完整示例（简单任务队列）

```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

int ready = 0;

void* consumer(void* arg) {
    pthread_mutex_lock(&mutex);

    while (!ready) {
        pthread_cond_wait(&cond, &mutex);
    }

    printf("consumed\n");

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* producer(void* arg) {
    pthread_mutex_lock(&mutex);

    ready = 1;

    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, consumer, NULL);
    pthread_create(&t2, NULL, producer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
```

---

## 6. 常见问题

### 1）为什么必须 while，不用 if？

因为可能发生：

* 虚假唤醒（spurious wakeup）
* 多线程竞争导致条件再次不成立

---

### 2）为什么 signal 要在 unlock 后？

避免：

* 唤醒线程立即竞争锁导致阻塞
* 降低上下文切换开销

---

### 3）cond 本身是否保存状态？

不保存。
条件变量只是“通知机制”，真正状态由共享变量维护。

---

## 7. 一句话总结

* mutex：保护数据
* condition variable：等待“状态变化”的通知

---


