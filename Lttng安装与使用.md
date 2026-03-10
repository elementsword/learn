### Lttng安装与使用 

#### 准备工作 

```
yum install kernel-devel.x86_64 numactl-devel.x86_64 popt-devel.x86_64 userspace-rcu-devel.x86_64 -y
```



#### 安装lttng-modules

```
wget https://lttng.org/files/lttng-modules/lttng-modules-latest-2.13.tar.bz2 &&
tar -xf lttng-modules-latest-2.13.tar.bz2 &&
cd lttng-modules-2.13.* &&
make -j128&&
sudo make modules_install &&
sudo depmod -a
```

#### 安装lttng-ust

```
wget https://lttng.org/files/lttng-ust/lttng-ust-latest-2.13.tar.bz2 &&
tar -xf lttng-ust-latest-2.13.tar.bz2 &&
cd lttng-ust-2.13.* &&
./configure &&
make -j128&&
sudo make install &&
sudo ldconfig
```

#### 安装lttng-tools

```
wget https://lttng.org/files/lttng-tools/lttng-tools-latest-2.13.tar.bz2 &&
tar -xf lttng-tools-latest-2.13.tar.bz2 &&
cd lttng-tools-2.13.* &&
./configure &&
make -j128&&
sudo make install &&
sudo ldconfig
```

遇到的问题 

![](图片/企业微信截图_20250305172227.png)

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH 

#### 使用步骤

##### 开启会话

```
lttng create my-kernel-session --output=/tmp/my-kernel-trace
```

设置会话属性 

```

lttng enable-channel --kernel channel0 --subbuf-size=8M --num-subbuf=8     --switch-timer=1000000 --read-timer=200000 --monitor-timer=500000
```

##### 显示可监视的tracepoint

```
lttng list --kernel
lttng list --kernel --syscall
```

##### 创建追踪规则

```
lttng enable-event -a -k //追踪内核所有的探测点和所有的系统调用事件(-k/--kernel)
lttng enable-event sched_switch,sched_wakeup -k //追踪探测点事件，这里我们追踪 sched_switch和sched_wakeup
lttng enable-event -a -k --tracepoint  //追踪所有的探测点事件
lttng enable-event -a -k --syscall     //追踪所有的系统调用
lttng enable-event aname -k --probe symbol+0x0   
lttng enable-event aname -k --probe 0xffff7260695  //允许通过内核符号（如函数名）加偏移量（symbol+offset）或直接指定内存地址（0xffff7260695）设置动态探针
lttng enable-event aname -k --function <symbol_name> //--function 选项：通过 Ftrace 的 API 跟踪特定内核函数的调用。当函数被调用时，事件会被 LTTng 记录
```

##### 增加输出上下文信息 

```
lttng add-context  -t pid -t nice -t tid -t perf:cpu:cycles -k //可以增加pid tid perf中的cpu周期 
```

##### 开始追踪

```
lttng start
```

##### 结束追踪

```
lttng destroy 
```

##### 查看会话状态

```
lttng status
```

##### 使用默认工具查看 

```
babeltrace /root/lttng-traces/auto-20250305-174828
```

![](图片/企业微信截图_20250306093905.png)