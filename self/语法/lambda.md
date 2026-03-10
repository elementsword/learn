## 🧩 一、Lambda 表达式的完整语法形式

```
[ 捕获列表 ] ( 参数列表 ) -> 返回类型 {
    函数体
}
```

每个部分的作用如下👇

| 部分          | 作用                         | 是否可省略         |
| ------------- | ---------------------------- | ------------------ |
| `[捕获列表]`  | 指定 lambda 如何获取外部变量 | ❌ 必须有（哪怕空） |
| `(参数列表)`  | 传入参数（类似普通函数）     | ✅ 可省略（无参数） |
| `-> 返回类型` | 明确指定返回类型             | ✅ 一般可省略       |
| `{函数体}`    | lambda 的实际执行逻辑        | ❌ 必须有           |

| 捕获方式     | 含义                                           |
| ------------ | ---------------------------------------------- |
| `[]`         | 不捕获任何外部变量                             |
| `[=]`        | 以 **值** 捕获所有使用到的外部变量（复制一份） |
| `[&]`        | 以 **引用** 捕获所有使用到的外部变量           |
| `[a, b]`     | 只值捕获 `a`、`b`                              |
| `[&a, b]`    | 引用捕获 `a`，值捕获 `b`                       |
| `[=, &b]`    | 默认值捕获，但 `b` 引用捕获                    |
| `[&, a]`     | 默认引用捕获，但 `a` 值捕获                    |
| `[this]`     | 捕获当前对象指针                               |
| `[=, *this]` | 捕获当前对象的副本（C++17 起）                 |

```
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int factor = 2;

    std::vector<int> nums = {1, 2, 3, 4};

    std::for_each(nums.begin(), nums.end(),
                  [=](int &x) { x *= factor; }); // 值捕获 factor

    for (int n : nums)
        std::cout << n << " ";  // 输出：2 4 6 8
}

```

## 🧩 一、悬空引用（Dangling Reference）

### ❌ 错误示例

```
#include <iostream>
#include <functional>

std::function<void()> make_lambda() {
    int value = 42;
    return [&]() { std::cout << value << std::endl; };  // 引用捕获
}

int main() {
    auto f = make_lambda();
    f(); // ⚠️ 未定义行为，value 已经销毁
}
```

🧨 **问题原因**：

- `value` 是局部变量；
- lambda 用 `&` 捕获它；
- 函数返回后，`value` 生命周期结束；
- lambda 仍持有悬空引用（指向已销毁内存）。

✅ **解决方案**：

- 改用值捕获：`[value]`；
- 或将 `value` 放到外层作用域。

------

## 🧩 二、悬空 this（Dangling this）

### ❌ 错误示例

```
#include <thread>
#include <iostream>

class Worker {
public:
    void start() {
        std::thread t([this]() {
            std::cout << "Working..." << std::endl;
        });
        t.detach();
    }
};

int main() {
    Worker w;
    w.start(); // ⚠️ 对象马上销毁，线程仍在运行！
}
```

🧨 **问题原因**：

- lambda 捕获了 `this` 指针；
- `Worker` 对象在主线程结束时销毁；
- 分离线程（`detach`）仍在执行；
- lambda 中访问的 `this` 指针悬空。

✅ **解决方案**：

- 改为捕获对象副本 `[self = *this]`（C++17 起）；
- 或用 `std::shared_ptr<Worker>` 管理生命周期；
- 或保证线程在对象析构前结束。

```
std::thread t([self = *this]() { self.doWork(); });
```

------

## 🧩 三、多线程数据竞争（Data Race）

### ❌ 错误示例

```
#include <thread>
#include <iostream>

int main() {
    int counter = 0;
    std::thread t([&]() {  // 引用捕获
        for (int i = 0; i < 10000; ++i)
            counter++;
    });
    for (int i = 0; i < 10000; ++i)
        counter++;
    t.join();
    std::cout << counter << std::endl;  // ⚠️ 未定义行为
}
```

🧨 **问题原因**：

- `counter` 被多个线程同时写入；
- 没有同步机制（锁或原子）；
- 导致数据竞争（data race）。

✅ **解决方案**：

- 使用互斥锁保护；
- 或改用原子变量 `std::atomic<int>`；
- 或值捕获，在各线程中分别维护局部副本。

------

## 🧩 四、延迟执行导致的捕获值过期

### ❌ 错误示例

```
#include <iostream>
#include <vector>
#include <functional>

int main() {
    std::vector<std::function<void()>> tasks;

    for (int i = 0; i < 3; ++i) {
        tasks.push_back([&]() { std::cout << i << " "; });
    }

    for (auto &t : tasks) t();  // ⚠️ 输出：3 3 3
}
```

🧨 **问题原因**：

- 所有 lambda 都引用捕获同一个 `i`；
- 循环结束时 `i` == 3；
- 执行时访问的都是同一个变量。

✅ **解决方案**：
 改为 **值捕获当前的 i 副本**：

```
tasks.push_back([i]() { std::cout << i << " "; });
```

🔹 输出：`0 1 2`
