## perf使用说明 

-p pid：只分析指定进程的性能。

-C cpu-id：只分析指定cpu的性能 

--call-graph 静态调用（default） dwarf 需要 debuginfo 可以看到 调用栈堆 （user-kernel）

-F frequent：调整采样频率（Hz），默认 1000Hz，可以降低或提高以减少开销。

-a, 即--all-cpus：启用系统级性能监控，采集所有 CPU 的数据。默认情况下，`perf top`仅监控当前 CPU 或指定进程绑定的 CPU。

-z  每次刷新时重置历史数据，避免累积采样结果干扰实时分析。

-E , --entries=20 仅展示采样数最高的前 20 个函数，帮助快速定位热点代码。

-e 选项用于指定监控的硬件性能监控单元（PMU）事件。

-U, --hide_user_symbols  隐藏用户空间符号 

--stdio 禁用交互式界面，直接生成文本报告。适用于自动化脚本或批量处理场景。

#### 1.perf top

默认情况下，它会对整个系统进行采样，并显示 CPU 最常执行的函数（热点）。 实时刷新 

![](图片/企业微信截图_20250304135122.png)

##### perf top --options 下列为options

-c  1000 则表示每当目标事件累计发生 1000 次时，`perf`会采集一次样本数据。

-d 选项用于指定界面刷新之间的时间间隔

--ignore-vmlinux 选项在 perf top 中用于禁用对 vmlinux 文件的自动加载。

-D 导出符号表 

-b, --branch-any `-是`perf top`中用于分支采样的硬件事件选项，其核心功能是启用对任意已执行分支指令的采样

##### perf top 的交互式操作按键提供实时界面动态调整功能

- [q]：立即退出性能监控会话。
- [a]：注释当前符号（Annotate current symbol），显示符号的汇编代码或源码级性能热点。
- [L]：调整显示的百分比阈值（Change percent limit），例如按 `L` 后输入 `0.5` 仅显示占比 ≥0.5% 的条目。
- [/] ：按符号名称过滤，输入关键字后仅显示匹配条目。
- [k] ：只查看内核态的消耗 
- [d] ：该操作会将视图限定在当前选中的 DSO（如某个共享库）内部，仅显示该库内函数或符号的性能数据（如 CPU 占用率、采样计数等），从而帮助用户快速定位该模块的性能瓶颈。
- [V] ：**V 键**用于切换 Verbose（详细）模式

#### 2.perf stat

`perf stat` 用于 **统计程序运行时的总体性能指标**，如指令数、缓存命中率、分支预测错误率等。

虚拟机测试结果 

![](图片/企业微信截图_20250304171228.png)

实体机测试结果 

![](图片/企业微信截图_20250304171429.png)

-r N：重复运行 **N 次**，取平均值，提高测量精度

-e event：手动选择要统计的 **硬件/软件事件**。

:u（user）**仅统计用户态** 指令

:k（kernel）**仅统计内核态** 指令

例子

```bash
perf stat -e instructions:u,instructions:k ./my_program
```

-i, --no-inherit选项的作用是 **不让子进程继承父进程的计数器**，即 **只统计当前进程的性能数据，不统计它的子进程**。

#### 3.perf record采样并保存数据

`perf record` 用于 **采样程序运行过程中的热点数据**，并保存到文件（默认 `perf.data`）中，方便后续分析。

![](图片/企业微信截图_20250304180357.png)

-o 将结果重定向到指定文件 （默认perf.data）

```
perf record -o perf_output.txt sleep 5
```

#### 4.`perf report` —— **分析 `perf record` 采样数据**

`perf report` 解析 `perf record` 生成的 `perf.data` 文件，并展示采样热点。

![](图片/企业微信截图_20250305095029.png)

-i  指定report所需要的文件 （默认perf.data）

```
perf report -i perf_output.txt
```

#### 5.`perf mem`：分析内存访问延迟

`perf mem` 主要用于分析 **内存读/写访问的性能**，可以捕获 **高延迟的 load/store 操作**，帮助优化数据局部性和缓存命中率

命令：

```bash
perf mem record ./your_program

perf mem report //perf.data
```

![](图片/企业微信截图_20250304175426.png)

-t load/store 选择记录的选项 默认都记录 

Overhead 占比 

Samples 发生次数 

Local Weight 本地发生次数 

Memory Access 指内存访问类型，如读取（Load）或写入（Store）

Symbol 触发内存访问事件的符号（通常是函数名称）

Shared Object 表示发生内存访问事件的共享库或动态链接库（如 `.so` 文件）

Data Symbol 与内存访问事件相关的数据符号

Data Object 数据对象类型 

Snoop 表示缓存一致性协议中的“窥探”操作，通常用于多核处理器

TLB Access 表示对 **转换后备缓冲区（TLB）** 的访问。 

#### 6.`perf c2c`：分析多线程缓存一致性问题--->cpu要求需要 **PEBS（Precise Event-Based Sampling）**，并且需要 **Load Latency Sampling（LLS）** 以及 **Precise Store Sampling（PSS）**

`perf c2c` 用于分析 **Cache-to-Cache 共享冲突**，主要关注 **L1、L2、L3 缓存一致性协议（MESI 协议）** 下的数据共享情况。
它可以帮助发现：

- **伪共享（False Sharing）**：多个线程修改不同变量，但这些变量共享同一个 cache line，导致缓存冲突。
- **真实共享（True Sharing）**：多个线程访问同一变量，导致频繁的 **缓存一致性协议（MESI 协议）** 开销。
- **远程缓存访问**：某个 CPU 需要访问另一个 CPU 缓存中的数据，导致延迟增加。

命令：

```
perf c2c record ./your_program
perf c2c report
```

![](图片/企业微信截图_20250304175855.png)

-d lcl/rmt

- `lcl`（local）：只显示本地 HITM 事件
- `rmt`（remote）：只显示远程 HITM 事件

上方栏位的分别意思 

##### **缓存行与竞争相关**

- **Index**：缓存行的唯一标识编号。
- **Address**：内存地址，指向引发竞争的缓存行首地址。
- **Node**：NUMA 节点编号，显示该缓存行归属的处理器节点（用于多核系统）。
- **PA cnt**（Physical Address Count）：同一缓存行在不同物理地址的映射次数，反映可能的共享或冲突。

##### **命中与访问统计**

- **Hitm**：缓存行处于 **Modified 状态**的命中次数，直接反映核心间缓存行竞争。
- **Total**：该缓存行的总事件数（包含所有访问类型）。
- **LclHitm**：本地 NUMA 节点的 Modified 状态命中次数。
- **RmtHitm**：远程 NUMA 节点的 Modified 状态命中次数。
- **LclHit**/**RmtHit**：本地 / 远程节点的缓存命中次数（非 Modified 状态）。

##### **内存操作分类**

- **Loads**：加载操作（内存读取）的次数。
- **Stores**：存储操作（内存写入）的次数。
- **L1Hit**/**L1Miss**：L1 缓存命中 / 未命中的次数。
- **FB**（Fill Buffer）：缓存未命中时通过填充缓冲区的访问次数。
- **L1**/**L2**：L1/L2 缓存相关操作的统计。

### 火焰图 (Flame Graph)

火焰图是一种用于可视化程序性能数据的工具，尤其在性能分析和调优中非常有用。它通常用于展示 CPU 使用情况、函数调用栈以及代码执行时间，从而帮助开发人员识别性能瓶颈。

### 冷焰图 (Cold Flame Graph)

冷焰图，顾名思义，主要用于反映系统中的冷路径（即不常调用的路径或非常少使用的代码路径）。与火焰图不同，冷焰图主要聚焦于性能较差、低使用率的函数或代码路径，通常用于找出潜在的性能优化机会。

火焰图和冷焰图生成

下载

```bash
git clone https://github.com/brendangregg/Flamegraph.git
cd Flamegraph
sudo yum install perf
```

采集系统信息

```bash
perf record -F 99 -a -g -- sleep 60
```

通过脚本生成折叠栈格式

```
perf script > out.perf
```

生成火焰图和冷焰图

```
./stackcollapse-perf.pl out.perf > out.folded
./flamegraph.pl  out.folded > flamegraph.svg
./flamegraph.pl --color=black out.folded > cold_flamegraph.svg
```



