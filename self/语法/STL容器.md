### 1️⃣ `std::vector` —— 动态数组

**特点：**

- 底层使用连续内存（类似 `T* arr`）
- 支持随机访问（`O(1)`）
- 尾部插入 / 删除很快（`O(1)` 摊销）
- 中间插入 / 删除很慢（`O(n)`）
- 自动扩容（通常按 2 倍增长）

**常用函数：**

```
#include <vector>
#include <iostream>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3};
    v.push_back(4);       // 尾部插入
    v.pop_back();         // 尾部删除
    v.insert(v.begin()+1, 99);  // 在第二个位置插入
    v.erase(v.begin());   // 删除第一个元素

    for (auto x : v) cout << x << " ";
    cout << endl;
}
```

**注意事项：**

- 扩容时旧指针和迭代器会失效；
- 可用 `reserve(n)` 预先分配空间以减少扩容开销；
- 随机访问效率高，可用于实现动态数组或栈结构。

#### 1️⃣ 头部删除

在 `vector` 中，元素是连续存储的，类似一个数组：

```
索引： 0   1   2   3
元素： a   b   c   d
```

如果你执行：

```
v.erase(v.begin());
```

- 删除的是第一个元素 `a`。
- 后面的元素 `b, c, d` 都要往前移动一位，保证连续存储。
- **时间复杂度是 O(n)**，n 是 vector 当前的大小减 1。

所以 **头部删除对于 vector 来说效率不高**，尤其是元素很多时。

### 2️⃣ `std::deque` —— 双端队列

`deque` 全称 **double-ended queue**，支持在**两端高效插入和删除**。

1. **底层结构**

   - 并非单一连续数组，而是 **分块连续数组**：
     - 内部有多个小数组块，每块大小固定。
     - 通过一个指向块数组的指针数组管理。
   - 这样可以在两端插入删除而不用整体搬移大量元素。

2. **性能特点**
    | 操作 | 时间复杂度 |
    |------|------------|
    | 尾部插入/删除 | O(1) |
    | 头部插入/删除 | O(1) |
    | 中间插入/删除 | O(n) |
    | 随机访问 | O(1) |

> 注意：虽然随机访问是 O(1)，但不如 `vector` 那么快，因为可能需要两次内存寻址（指针数组 + 块内偏移）。

1. **迭代器失效规则**

- **尾部插入/删除**：如果不扩容，尾部操作不会使其它迭代器失效。
- **头部插入/删除**：可能使头部迭代器失效。
- **中间插入/删除**：会使插入/删除位置及其之后迭代器失效。
- **整体扩容**：和 `vector` 不同，deque 扩容不会移动所有元素，只是增加块，因此大部分迭代器仍然有效（除了受影响的块）。

------

🛠 使用示例

```
#include <deque>
#include <iostream>
using namespace std;

int main() {
    deque<int> d = {1, 2, 3};

    d.push_back(4);    // 尾部插入
    d.push_front(0);   // 头部插入

    d.pop_back();      // 尾部删除
    d.pop_front();     // 头部删除

    for (auto x : d) cout << x << " ";
    cout << endl;

    // 随机访问
    cout << "第2个元素: " << d[1] << endl;
}
```

------

✅ 适用场景

- 需要**两端插入删除**的队列
- 想保持**随机访问**的能力（比 list 快）
- 不适合频繁**中间插入删除**（O(n)）

### 3️⃣ `std::list` —— 双向链表

**特点：**

- 底层结构：双向链表

  ```
  prev <- [data] -> next
  ```

- 每个节点包含：

  - 数据
  - 指向前一个节点的指针
  - 指向后一个节点的指针

- **操作效率：**

  | 操作            | 复杂度                 |
  | --------------- | ---------------------- |
  | 插入/删除       | O(1)（在已知节点位置） |
  | 访问第 i 个元素 | O(n)                   |
  | 随机访问        | 不支持（只能顺序遍历） |

**迭代器规则：**

- 插入/删除不会使其他节点迭代器失效（只影响操作的节点）
- 可以安全地在遍历过程中插入/删除

**使用示例：**

```
#include <list>
#include <iostream>
using namespace std;

int main() {
    list<int> l = {1, 2, 3};

    l.push_back(4);     // 尾部插入
    l.push_front(0);    // 头部插入

    l.pop_back();       // 尾部删除
    l.pop_front();      // 头部删除

    // 中间插入
    auto it = l.begin();
    ++it;
    l.insert(it, 99);   // 在第二个位置插入

    for (auto x : l) cout << x << " ";
    cout << endl;
}
```

**适用场景：**

- 频繁在任意位置插入/删除
- 不关心随机访问性能
- 不适合用下标访问

### 1️⃣ std::set 和map 关联容器

| 容器       | 描述                         | 是否允许重复 | 访问方式       |
| ---------- | ---------------------------- | ------------ | -------------- |
| `set`      | 保存唯一键，按顺序排列       | 否           | 只能查找元素   |
| `multiset` | 保存键，允许重复，按顺序排列 | 是           | 查找所有相同键 |
| `map`      | 键值对，键唯一，按键排序     | 否           | 通过键访问值   |
| `multimap` | 键值对，允许重复键，按键排序 | 是           | 查找所有相同键 |

------

2️⃣ 底层实现

- **红黑树（RB-tree）**
  - 自平衡二叉搜索树（BST）
  - 插入/删除会自动调整，保证树高度 O(log n)
  - 访问/查找/插入/删除都是 O(log n)
- 每个节点包含：
  - `key` 或 `key + value`
  - 左/右子节点指针
  - 父节点指针 + 红/黑标记

------

3️⃣ 随机访问 vs 顺序访问

- **顺序访问**：

  ```
  for (auto it = s.begin(); it != s.end(); ++it) { ... }
  ```

  - 按 key 从小到大遍历
  - 使用迭代器顺序遍历

- **随机访问（下标访问）？**

  - **不支持**像 `vector` / `deque` 的 `[i]`
  - 因为红黑树是节点指针连接的，不是数组连续内存
  - 查找某个键只能用 `find(key)` → O(log n)

------

4️⃣ 常用操作

`set`

```
#include <set>
#include <iostream>
using namespace std;

int main() {
    set<int> s = {5, 1, 3};

    s.insert(4);   // 插入元素
    s.erase(1);    // 删除元素
    if (s.find(3) != s.end()) cout << "Found 3\n";

    for (auto x : s) cout << x << " "; // 自动排序输出
}
```

`map`

```
#include <map>
#include <iostream>
using namespace std;

int main() {
    map<string, int> m;

    m["apple"] = 3;
    m["banana"] = 5;

    m.erase("apple");
    cout << m["banana"] << endl; // 访问值

    for (auto &[k,v] : m) cout << k << ":" << v << " ";
}
```

### 1️⃣ 无序容器概览

| 容器                 | 描述               | 是否允许重复 | 访问方式       |
| -------------------- | ------------------ | ------------ | -------------- |
| `unordered_set`      | 唯一键集合         | 否           | 查找元素       |
| `unordered_multiset` | 允许重复键集合     | 是           | 查找所有相同键 |
| `unordered_map`      | 键值对，键唯一     | 否           | 通过键访问值   |
| `unordered_multimap` | 允许重复键的键值对 | 是           | 查找所有相同键 |

- **特点**：
  - **不排序**：元素在容器中是无序的
  - **底层是哈希表**：通过哈希函数快速定位
  - **查找/插入/删除平均 O(1)**（摊销）
  - 最坏情况 O(n)（哈希冲突严重时）

------

2️⃣ 哈希表原理

- 容器维护一个 **桶数组（buckets）**
- 每个元素通过哈希函数 `hash(key)` 决定落在哪个桶
- 桶内使用链表或红黑树处理冲突（C++17 开始，冲突过多会树化）

**示例：**

```
hash(key) % bucket_count → 桶索引
桶内可能有多个元素 → 链表或红黑树
```

- 查找 key：
  1. 计算哈希值 → 找到桶
  2. 遍历桶内元素 → 找到 key
- 平均时间复杂度 O(1)

------

3️⃣ 示例

`unordered_set`

```
#include <unordered_set>
#include <iostream>
using namespace std;

int main() {
    unordered_set<int> s = {5, 1, 3};

    s.insert(4);
    s.erase(1);

    if (s.find(3) != s.end()) cout << "Found 3\n";

    for (auto x : s) cout << x << " "; // 无序输出
}
```

#### `unordered_map`

```
#include <unordered_map>
#include <iostream>
using namespace std;

int main() {
    unordered_map<string,int> m;

    m["apple"] = 5;
    m["banana"] = 3;

    cout << m["apple"] << endl; // 5

    for (auto &[k,v] : m) cout << k << ":" << v << " "; // 无序输出
}
```