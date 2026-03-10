## 🧭 STL 算法模块总览

STL 的算法都定义在头文件 `<algorithm>`、`<numeric>`、`<functional>` 里。
 算法本身不依赖具体容器，而是操作 **迭代器区间 `[first, last)`**。

可以大致分为以下几类👇：

| 类别               | 代表函数                                               | 简述                     |
| ------------------ | ------------------------------------------------------ | ------------------------ |
| **非修改性算法**   | `find`, `count`, `for_each`, `all_of`                  | 只读取元素，不改变内容   |
| **修改性算法**     | `copy`, `fill`, `transform`, `remove`                  | 会改变或移动区间内的元素 |
| **排序与划分算法** | `sort`, `stable_sort`, `partition`, `nth_element`      | 控制元素顺序             |
| **集合与合并算法** | `set_union`, `set_intersection`, `merge`, `unique`     | 集合操作                 |
| **数值算法**       | `accumulate`, `inner_product`, `iota`                  | 定义在 `<numeric>`       |
| **堆算法**         | `make_heap`, `push_heap`, `pop_heap`, `sort_heap`      | 用于优先队列逻辑         |
| **最值与比较算法** | `min`, `max`, `min_element`, `lexicographical_compare` | 计算极值或比较序列       |

### 1️⃣ `for_each`

```
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    for_each(v.begin(), v.end(), [](int x){ cout << x << " "; });
}
```

输出：

```
1 2 3 4 5
```

**特点：**

- 接受 lambda 或函数；
- 不改变元素；
- 返回执行过的函数对象（可用于统计副作用）。

------

### 2️⃣ `find` / `find_if`

```
vector<int> v = {1, 3, 5, 7, 9};
auto it = find(v.begin(), v.end(), 5);
if (it != v.end())
    cout << "找到元素：" << *it;
```

或者带条件：

```
auto it2 = find_if(v.begin(), v.end(), [](int x){ return x > 6; });
```

**复杂度：** O(n)，线性查找。

------

### 3️⃣ `count` / `count_if`

```
int c1 = count(v.begin(), v.end(), 3);
int c2 = count_if(v.begin(), v.end(), [](int x){ return x % 2 == 1; });
cout << c1 << " " << c2;
```

输出：

```
1 5
```

------

### 4️⃣ `all_of` / `any_of` / `none_of`

非常常用，尤其搭配 lambda：

```
vector<int> v = {2, 4, 6, 8};

bool all_even = all_of(v.begin(), v.end(), [](int x){ return x % 2 == 0; });
bool has_odd = any_of(v.begin(), v.end(), [](int x){ return x % 2 == 1; });
bool none_negative = none_of(v.begin(), v.end(), [](int x){ return x < 0; });
```

输出：

```
all_even = true
has_odd = false
none_negative = true
```

------

### 5️⃣ `equal` / `mismatch`

```
vector<int> a = {1, 2, 3};
vector<int> b = {1, 2, 4};

cout << equal(a.begin(), a.end(), b.begin());  // false

auto diff = mismatch(a.begin(), a.end(), b.begin());
cout << "不同的位置: " << *diff.first << " vs " << *diff.second;  // 3 vs 4
```

## 🧩 二、修改性算法（Modifying Sequence Operations）

修改性算法会 **直接改变容器或区间内的元素**。
 它们通常作用在“可写”的迭代器区间上。

常见的修改性算法包括：

| 类别        | 典型函数                                   | 功能简介                               |
| ----------- | ------------------------------------------ | -------------------------------------- |
| 复制        | `copy`, `copy_n`, `copy_if`, `move`        | 把元素从一个区间复制或移动到另一个区间 |
| 填充        | `fill`, `fill_n`, `generate`, `generate_n` | 批量写入指定值或函数生成的值           |
| 替换        | `replace`, `replace_if`, `replace_copy`    | 替换元素值                             |
| 删除        | `remove`, `remove_if`, `unique`            | “逻辑删除”元素（不改变大小）           |
| 交换        | `swap`, `iter_swap`, `swap_ranges`         | 交换元素                               |
| 变换        | `transform`                                | 对每个元素应用函数（可输出到新容器）   |
| 反转 / 旋转 | `reverse`, `rotate`                        | 重新排列区间内元素                     |

------

### 1️⃣ 复制与移动

#### `copy(first, last, dest)`

将 `[first, last)` 区间的内容复制到从 `dest` 开始的位置。

```
vector<int> a = {1, 2, 3, 4, 5};
vector<int> b(5);

copy(a.begin(), a.end(), b.begin());
```

📌 要求：

- `dest` 至少有足够空间
- 不支持重叠区域（会出错）

#### `move(first, last, dest)`

类似 `copy`，但使用移动语义（`std::move`）。

------

### 2️⃣ 填充与生成

#### `fill(first, last, value)`

用同一个值填满区间。

```
vector<int> v(5);
fill(v.begin(), v.end(), 42); // 全部变成 42
```

#### `generate(first, last, func)`

用函数生成值。

```
int n = 0;
vector<int> v(5);
generate(v.begin(), v.end(), [&] { return ++n; });  // 1,2,3,4,5
```

------

### 3️⃣ 替换元素

#### `replace(first, last, old_value, new_value)`

替换所有等于 `old_value` 的元素。

```
vector<int> v = {1, 2, 3, 2, 4};
replace(v.begin(), v.end(), 2, 99); // 1,99,3,99,4
```

#### `replace_if(first, last, pred, new_value)`

使用谓词条件。

------

### 4️⃣ 删除元素（逻辑删除）

C++ 的删除算法 **不会改变容器的大小**，只会把“要保留的元素”移到前面，返回一个“新的末尾迭代器”。

#### `remove(first, last, value)`

移除所有等于 `value` 的元素。

```
vector<int> v = {1, 2, 3, 2, 4};
auto new_end = remove(v.begin(), v.end(), 2);
v.erase(new_end, v.end()); // 真正删除
```

💡 **remove + erase** 是标准用法：

> ```
> v.erase(remove(v.begin(), v.end(), val), v.end());
> ```

#### `unique(first, last)`

去除连续重复元素。

```
vector<int> v = {1,1,2,2,3,3,3};
auto new_end = unique(v.begin(), v.end());
v.erase(new_end, v.end()); // 1,2,3
```

------

### 5️⃣ 交换与反转

```
vector<int> v = {1, 2, 3};
reverse(v.begin(), v.end()); // 3,2,1
rotate(v.begin(), v.begin() + 1, v.end()); // 左旋1位
```

------

### 6️⃣ `transform`（变换）

最灵活的修改算法之一。

```
vector<int> v = {1, 2, 3};
vector<int> res(3);
transform(v.begin(), v.end(), res.begin(), [](int x){ return x * x; });
// res = {1,4,9}
```

或者“两个区间”版本：

```
transform(a.begin(), a.end(), b.begin(), res.begin(), plus<int>());
```