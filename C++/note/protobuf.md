在 C++ 中，Protobuf 的序列化和反序列化 API 主要来自 `google::protobuf::Message` 基类。最常用的 API 并不多，掌握下面这些基本就够了。

---

# 一、序列化（Serialize）

假设：

```proto
message LoginReq {
    string username = 1;
    string password = 2;
}
```

创建对象：

```cpp
chat::LoginReq req;

req.set_username("Tom");
req.set_password("123456");
```

---

## 1. SerializeToString（最常用）

序列化到 `std::string`。

```cpp
std::string buffer;

bool ok = req.SerializeToString(&buffer);

if (!ok) {
    // 序列化失败
}
```

得到：

```
LoginReq对象
      ↓
std::string（二进制）
```

**使用场景：**

* TCP Socket
* 网络发送
* 最常用

---

## 2. SerializeToArray

序列化到一块内存。

```cpp
char buf[1024];

bool ok = req.SerializeToArray(buf, sizeof(buf));
```

适用于：

* 固定缓冲区
* 自己管理内存

---

## 3. SerializeToOstream

直接写入文件流。

```cpp
std::ofstream ofs("data.bin", std::ios::binary);

req.SerializeToOstream(&ofs);
```

---

## 4. SerializePartialToString

```cpp
req.SerializePartialToString(&buffer);
```

作用：

即使缺少必填字段（主要针对 `proto2`），也允许序列化。

`proto3` 基本不用。

---

# 二、反序列化（Parse）

---

## 1. ParseFromString（最常用）

```cpp
chat::LoginReq req;

req.ParseFromString(buffer);
```

成功：

```cpp
cout << req.username() << endl;
cout << req.password() << endl;
```

这是网络编程中最常用的方法。

---

## 2. ParseFromArray

```cpp
chat::LoginReq req;

req.ParseFromArray(buf, len);
```

例如：

```cpp
recv(fd, buf, len, 0);

req.ParseFromArray(buf, len);
```

非常适合 socket。

---

## 3. ParseFromIstream

```cpp
std::ifstream ifs("data.bin", std::ios::binary);

chat::LoginReq req;

req.ParseFromIstream(&ifs);
```

读取文件。

---

## 4. ParsePartialFromString

```cpp
req.ParsePartialFromString(buffer);
```

允许解析不完整的数据。

一般很少使用。

---

# 三、获取序列化后的大小

---

## ByteSizeLong()

最常用。

```cpp
size_t len = req.ByteSizeLong();
```

例如：

```cpp
uint32_t bodyLen = req.ByteSizeLong();

char* data = new char[bodyLen];

req.SerializeToArray(data, bodyLen);
```

---

# 四、字段访问 API

假设：

```proto
message LoginReq{

    string username = 1;

    string password = 2;
}
```

---

## 设置字段

```cpp
req.set_username("Tom");

req.set_password("123456");
```

---

## 获取字段

```cpp
cout << req.username();

cout << req.password();
```

注意：

不是：

```cpp
get_username();
```

而是：

```cpp
username();
```

---

## 判断字段是否存在

Proto3：

```cpp
string
int
bool
```

默认都有默认值。

一般不用：

```cpp
has_username();
```

Proto2 才大量使用：

```cpp
has_xxx();
```

---

## 清空字段

```cpp
req.clear_username();
```

或者：

```cpp
req.Clear();
```

清空整个对象。

---

# 五、Repeated 字段

```proto
message Group{

    repeated string members = 1;
}
```

添加：

```cpp
group.add_members("Tom");

group.add_members("Jack");
```

获取数量：

```cpp
group.members_size();
```

访问：

```cpp
cout << group.members(0);
```

遍历：

```cpp
for (int i = 0; i < group.members_size(); i++) {

    cout << group.members(i);
}
```

---

# 六、嵌套 Message

```proto
message Address{

    string city = 1;
}

message User{

    Address address = 1;
}
```

设置：

```cpp
user.mutable_address()->set_city("北京");
```

读取：

```cpp
cout << user.address().city();
```

---

# 七、常用 API 总结

| API                    | 作用                 | 使用频率  |
| ---------------------- | ------------------ | ----- |
| `SerializeToString()`  | 序列化到 `std::string` | ⭐⭐⭐⭐⭐ |
| `SerializeToArray()`   | 序列化到内存             | ⭐⭐⭐⭐  |
| `SerializeToOstream()` | 写文件                | ⭐⭐    |
| `ParseFromString()`    | 从 `std::string` 解析 | ⭐⭐⭐⭐⭐ |
| `ParseFromArray()`     | 从内存解析              | ⭐⭐⭐⭐  |
| `ParseFromIstream()`   | 从文件解析              | ⭐⭐    |
| `ByteSizeLong()`       | 获取序列化后的大小          | ⭐⭐⭐⭐  |
| `Clear()`              | 清空整个对象             | ⭐⭐⭐   |
| `clear_xxx()`          | 清空字段               | ⭐⭐⭐   |
| `set_xxx()`            | 设置字段               | ⭐⭐⭐⭐⭐ |
| `xxx()`                | 获取字段               | ⭐⭐⭐⭐⭐ |
| `add_xxx()`            | 添加 repeated 元素     | ⭐⭐⭐⭐⭐ |
| `xxx_size()`           | 获取 repeated 数量     | ⭐⭐⭐⭐  |
| `mutable_xxx()`        | 获取可修改的嵌套消息         | ⭐⭐⭐⭐  |

---

## 八、聊天室项目中最常用的 API

对于 C++ 网络聊天室，实际开发中高频使用的主要就是下面这些：

```cpp
// 设置字段
req.set_username("Tom");
req.set_password("123456");

// 序列化
std::string data;
req.SerializeToString(&data);

// 获取序列化长度（用于消息头）
uint32_t len = req.ByteSizeLong();

// 发送 data.data(), data.size()

// 接收后反序列化
chat::LoginReq req2;
req2.ParseFromArray(buf, len);

// 读取字段
std::cout << req2.username() << std::endl;
std::cout << req2.password() << std::endl;
```

对于你的聊天室项目，**`SerializeToString()`、`ParseFromArray()`（或 `ParseFromString()`）、`ByteSizeLong()`、`set_xxx()`、`xxx()`、`add_xxx()`、`mutable_xxx()`** 基本覆盖了绝大多数业务场景。

