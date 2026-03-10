## C++ 智能指针种类

C++ 标准库主要提供三种智能指针：

| 智能指针          | 所属头文件 | 功能特点                          | 使用场景                   |
| ----------------- | ---------- | --------------------------------- | -------------------------- |
| `std::unique_ptr` | `<memory>` | 独占所有权，不能拷贝，只能移动    | 对象唯一拥有，生命周期明确 |
| `std::shared_ptr` | `<memory>` | 引用计数，多个指针共享对象所有权  | 多个对象需要共享同一资源   |
| `std::weak_ptr`   | `<memory>` | 配合 `shared_ptr`，不增加引用计数 | 避免循环引用               |

unique_ptr

```c++
#include <memory>
#include <iostream>

struct A { 
    A() { std::cout << "A ctor\n"; }
    ~A() { std::cout << "A dtor\n"; }
};

int main() {
    std::unique_ptr<A> p1 = std::make_unique<A>(); // 推荐使用make_unique
    // std::unique_ptr<A> p2 = p1; // 错误，不允许拷贝
    std::unique_ptr<A> p2 = std::move(p1); // 转移所有权
    if(!p1) std::cout << "p1 is null\n";
}

result：
A ctor
p1 is null
A dtor
```

shared_ptr

```
#include <memory>
#include <iostream>

struct A {
    ~A() { std::cout << "A destroyed\n"; }
};

int main() {
    std::shared_ptr<A> sp1 = std::make_shared<A>();
    std::shared_ptr<A> sp2 = sp1; // 引用计数+1
    std::cout << sp1.use_count() << "\n"; // 输出2
} // 两个shared_ptr超出作用域，对象自动释放


result:
2
A destroyed
```

weak_ptr

```
#include <iostream>
#include <memory>

struct B; // 前向声明

struct A {
    std::shared_ptr<B> bptr; 
    ~A() { std::cout << "A destroyed\n"; }
};

struct B {
    std::weak_ptr<A> aptr; // 🔑 改成 weak_ptr
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();

    a->bptr = b; 
    b->aptr = a; // 不会增加引用计数

    std::cout << "end of main\n";
}

result:
end of main
A destroyed
B destroyed
```

