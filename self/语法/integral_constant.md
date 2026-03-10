# `std::integral_constant` 文档说明

## 1. 定义

`std::integral_constant` 是 C++ 标准库 `<type_traits>` 提供的一个模板类，用于 **将一个值封装成类型**，以便在 **编译期模板元编程** 中使用。

```
#include <type_traits>

template<typename T, T v>
struct integral_constant {
    static constexpr T value = v;      // 封装的值
    using value_type = T;              // 封装值的类型
    using type = integral_constant;    // 类型本身的别名
    constexpr operator T() const noexcept { return v; } // 可隐式转换为 T
};
```

------

## 2. 模板参数

| 模板参数 | 说明                                    |
| -------- | --------------------------------------- |
| `T`      | 值的类型，如 `int`, `size_t`, `bool` 等 |
| `v`      | 要封装的值（编译期常量）                |

------

## 3. 成员

| 成员                                    | 类型     | 说明                     |
| --------------------------------------- | -------- | ------------------------ |
| `static constexpr T value`              | T        | 封装的编译期常量值       |
| `using value_type = T`                  | 类型别名 | 封装值的类型             |
| `using type = integral_constant`        | 类型别名 | 指向自身类型             |
| `constexpr operator T() const noexcept` | T        | 可将类型实例隐式转换为值 |

------

## 4. 特点

1. **类型 + 值**：
   - 类型层面可以进行模板推导和编译期判断
   - 值可以通过 `.value` 或转换操作符访问
2. **编译期常量**：
   - 值在编译期就确定，无运行时开销
   - 可用于数组大小、模板参数、`static_assert` 条件等
3. **模板元编程基础工具**：
   - `std::true_type` = `std::integral_constant<bool, true>`
   - `std::false_type` = `std::integral_constant<bool, false>`
   - Boost.MPL、Boost.Lockfree 等模板库广泛使用

------

## 5. 使用示例

### 基本示例

```
#include <type_traits>
#include <iostream>

using MyConst = std::integral_constant<int, 42>;

int main() {
    std::cout << "value: " << MyConst::value << "\n"; // 输出 42

    int x = MyConst{}; // 隐式转换为 int
    std::cout << "x: " << x << "\n"; // 输出 42
}
```

### 编译期判断

```
#include <type_traits>

static_assert(std::integral_constant<int, 5>::value == 5, "编译期检查");
```

### 模板参数使用

```
template<typename N>
struct ArrayHolder {
    int arr[N::value]; // N 必须是 std::integral_constant
};

ArrayHolder<std::integral_constant<int, 10>> holder; // 数组大小 = 10
```

------

## 6. 与 Boost.Lockfree 的关系

在 Boost.Lockfree 中：

```
extract_arg_or_default_t<
    BoundArgs,
    TypeTag,
    std::integral_constant<IntegralType, default_>
>::value
```

- `TypeTag` → 参数名（类型，不含值）
- `BoundArgs` → 参数包，可能含用户值
- 第三参数 → 默认值封装成 `std::integral_constant`
- `.value` → 提取实际值，无论用户有没有传都能得到一个编译期常量

------

## 7. 总结

> `std::integral_constant` = **类型 + 编译期值**，核心用途是让模板元编程可以在类型层面携带并操作值，同时提供 `.value` 访问编译期常量。