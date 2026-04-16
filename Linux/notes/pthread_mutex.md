`pthread.h` 中的锁主要是 **互斥锁 pthread_mutex_t**，用于保护临界区，避免多线程同时访问共享数据。

---

## 1. 基本类型

```c
pthread_mutex_t mutex;
```

---

## 2. 初始化

### 方式1：静态初始化

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

### 方式2：动态初始化（推荐）

```c
pthread_mutex_t mutex;

pthread_mutex_init(&mutex, NULL);
```

第二个参数是属性，一般用 `NULL`。

---

## 3. 加锁 / 解锁

```c
pthread_mutex_lock(&mutex);   // 加锁
// 临界区代码
pthread_mutex_unlock(&mutex); // 解锁
```

---

## 4. 销毁

```c
pthread_mutex_destroy(&mutex);
```

---

## 5. 示例（线程安全计数器）

```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
int counter = 0;

void* worker(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);

    printf("counter = %d\n", counter);
    return 0;
}
```

---

## 6. 常见变种锁

### 1）递归锁

同一线程可重复加锁

```c
pthread_mutexattr_t attr;
pthread_mutex_t mutex;

pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

pthread_mutex_init(&mutex, &attr);
```

---

### 2）尝试锁（非阻塞）

```c
if (pthread_mutex_trylock(&mutex) == 0) {
    // 成功获取锁
    pthread_mutex_unlock(&mutex);
} else {
    // 没拿到锁
}
```

---

### 3）限时锁

```c
pthread_mutex_timedlock(&mutex, &abs_time);
```

---

## 7. 使用规则（关键）

* 锁和解锁必须成对出现
* 避免死锁（多个锁顺序一致）
* 临界区尽量短
* 不要在持锁时做阻塞IO

---

