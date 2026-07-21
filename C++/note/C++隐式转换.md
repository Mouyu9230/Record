  一、什么是「隐式类型转换」
     
  C++ 在需要类型 A 的地方，如果传了一个类型 B，编译器会自动找办法把 B 转成
  A。比如：

  double x = 3;      // int 3 → double 3.0，编译器自动转
  void f(double x);
  f(10);              // int 10 → double 10.0，自动转

  这些是内置类型的隐式转换，C++ 一直这么干。

  二、自定义类型也有隐式转换

  编译器不只内置类型可以转，它对你自己的类也提供了一套隐式转换规则——单参数构造函
  数就是其中一条。

  规则很简单：

  ▎ 如果一个构造函数只接受一个参数（或者除默认参数外只接受一个），编译器会把它视
  ▎ 为「如何从参数类型转换到当前类类型」的说明。

  class Epoll {              // 假设没有 explicit
      Epoll(int max_events); // ← 编译器理解成："int → Epoll" 的转换
  };

  这意味着：

  Epoll e = 1024;         // OK，等价于 Epoll e(1024)
  Epoll e2 = 10;          // OK
  Epoll e3 = 1;           // OK，全都合法

  三、编译器的具体匹配过程

  当编译器看到 Epoll e = 1024; 时，它的思考过程是：

  1. 1024 是 int
  2. 赋值目标类型是 Epoll
  3. 查 Epoll 有没有能接受 int 的构造函数
  4. 有 Epoll(int max_events)
  5. 好，就用它创建一个临时 Epoll(1024)，然后赋给 e

  注意：这里的 = 不是赋值运算符，是拷贝初始化。编译器在这里自始至终只调了一次
  Epoll(int)，没有拷贝构造（C++17 保证省略临时对象）。

  四、这会在哪些地方造成意外问题

  场景 1：函数参数传错

  void process_event(Epoll& ep);

  int main() {
      int fd = 100;
      process_event(fd);   // 本意是传 fd，但参数类型是 Epoll&

      // 编译器看到: "fd 是 int，Epoll 有接受 int 的构造函数"
      // 生成: Epoll tmp(fd); process_event(tmp);
      // 编译通过 ✅，但你在一个临时 Epoll 上操作，函数内做的一切都是无效的
  }

  你本意想表达「去处理 fd 这个编号的连接」，但因为 process_event 的参数是
  Epoll&，编译器默默把 int 转成了
  Epoll，你的逻辑完全错了，但没有任何编译/运行时报错。

  场景 2：多参数但有默认值

  class Epoll {
      Epoll(int max_events = 1024, bool use_et = false);
      // 虽然声明了 2 个参数，但都有默认值，实际可以 0 个或 1 个参数构造
  };

  // 那么下面全合法：
  Epoll e1;           // OK
  Epoll e2 = 5;       // OK —— int → Epoll 隐式转换！

  // 甚至
  void handle(Epoll& ep);
  handle(0);          // OK —— 创建 Epoll(0, false)，完全不是你本意

  explicit 禁止了 int → Epoll 的隐式转换。

  场景 3：返回值隐式构造

  Epoll create() {
      return 1024;  // 没有 explicit: 合法，return Epoll(1024)
                    // 有 explicit:  编译错误
  }

  场景 4：运算符误匹配

  bool operator==(const Epoll& a, const Epoll& b);

  if (epoll == 0) {   // 没有 explicit: 合法！编译器把 0 转成 Epoll(0)
                      // 这种比较毫无意义，但不报错
  }

  ---
  五、加上 explicit 后编译器行为

  class Epoll {
      explicit Epoll(int max_events = 1024);
  };

  此时上面所有场景：

  Epoll e = 1024;       // ❌ 编译错误：不能隐式转换
  process_event(fd);    // ❌ 编译错误：int → Epoll 无合法转换
  handle(0);            // ❌ 编译错误
  return 1024;          // ❌ 编译错误
  epll == 0;            // ❌ 编译错误

  必须显式构造：

  Epoll e(1024);          // ✅ OK
  process_event(Epoll(fd)); // ✅ OK（你明确知道自己在构造临时对象）
  auto e = Epoll(1024);   // ✅ OK（= 右侧是显式构造，非隐式转换）

  六、一句话总结

  ┌──────────┬──────────────────────────────────────────────────────────────┐
  │   状态   │                             行为                             │
  ├──────────┼──────────────────────────────────────────────────────────────┤
  │ 无       │ 编译器看到 int 的地方，如果期望类型是 Epoll，它会偷偷帮你    │
  │ explicit │ new 一个，可能完全不是你本意                                 │
  ├──────────┼──────────────────────────────────────────────────────────────┤
  │ 有       │ 编译器只在显式调用构造函数时才构造对象，其他地方传 int       │
  │ explicit │ 就报错——安全，无歧义                                         │
  └──────────┴──────────────────────────────────────────────────────────────┘

  explicit 锁掉的是「编译器替你做决定」的路径。你需要 Epoll 的时候就写 
  Epoll(...)，其他地方不小心传了个 int 编译器会告诉你错了。 只牺牲了 Epoll e = 
  值 这种语法糖，换来了更安全的类型检查。

