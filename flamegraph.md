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

