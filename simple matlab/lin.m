% 等式约束：chs范围0 ~ 1 （f1都不要了）
% feature model for C1: FirewallOnly
X6 = X1
X7 = X1
X1 = X8 + X31 + X32 + X33 + X34
% feature model for C2: StrictFirewall
X10 = X2
X11 = X2
X2 = X35 + X36 + X37 + X38
% feature model for C3: SampledDPI
X13 = X3
X14 = X3
X3 = X39 + X40 + X41 
X39 + X40 + X41 = X16 + X17
X17 = X48 + X49 + X50 + X51
X17 = X56 + X57 + X58 + X59
% feature model for C4: FullDPI
X20 = X4
X21 = X4
X4 = X42 + X43 + X44
X42 + X43 + X44 = X23 + X24
X24 = X52 + X53 + X54 + X55
X24 = X60 + X61 + X62 + X63
% feature model for C5: FullDPI
X27 = X5
X28 = X5
X5 = X45 + X46 + X47
X45 + X46 + X47 = X30

% 等式约束：包含的physical feature与对应资产的节点分配
% X9 = X31 + X32 + X33 + X34
% X12 = X35 + X36 + X37 + X38
% X15 = X39 + X40 + X41
% X22 = X42 + X43 + X44
% X29 = X45 + X46 + X47
% X18 = X48 + X49 + X50 + X51
% X25 = X52 + X53 + X54 + X55
% X19 = X56 + X57 + X58 + X59
% X26 = X60 + X61 + X62 + X63

% 输入值：包含前者对后者的服务资源影响
% a = 0.1
% Ds = 1000
% im-f3-f3 = Ds
% im-f6-f8 = Ds
% im-f6-f4 = a * Ds
% im-f6-f7 = (1-a) * Ds
% im-f5-f4 = Ds

% 等式约束：C对三种资源的需求量，整数
% d-C1-fw = X9 * 1000
% d-C2-fw = X12 * 1000
% d-C3-fw = X17 * 900
% d-C4-fw = X24 * 900
% d-C3-rt = X17 * 1000
% d-C4-rt = X24 * 1000
% d-C3-idp = X17 * 100 + X16 * 1000
% d-C4-idp = X24 * 100 + X23 * 100
% d-C5-idp = X29 * 1000

% 输入值：单位NF资源占用量、服务能力
% cpu-vrt = 1
% cpu-vfw = 1
% cpu-idp = 1
% cpu-vm = 4
% mem-vrt = 0.1
% mem-vfw = 0.1
% mem-idp = 0.5
% mem-vm = 8
% rps-vrt = 1000
% rps-vfw = 1000
% rps-idp = 100
% rps-hrt = 10000
% rps-hfw = 5000

% 输入值：数据中心物理资源总量
% ctr-cpu = 10
% ctr-mem = 20
% edg-cpu = 3
% edg-mem = 10

% （非线性？）不等式约束：分配到节点上的服务占用的资源不超过该节点拥有的资源总量
% PNF
X56 * X17 * 1000 + X60 * X24 * 1000 <= 10000
X31 * (X31 + X32 + X33 + X34) * 1000 + X35 * (X35 + X36 + X37 + X38) * 1000 + X48 * X17 * 900 + X52 * X24 * 900 <= 5000
% VM + VNF 
used-n7-cpu = X28 * 4 + X64 + X65 + X66 <= 10
X28 * 8 + X64 * 0.1 + X65 * 0.1 + X66 * 0.5 <= 20
used-n8-cpu = X67 + X68 + X69 <= 3
X67 * 0.1 + X68 * 0.1 + X69 * 0.5 <= 10
used-n13-cpu = X70 + X71 + X72 <= 3
X70 * 0.1 + X71 * 0.1 + X72 * 0.5 <= 10
% VNF被分配的请求不大于实例化num个后能提供的处理能力
X57 * X17 * 1000 + X61 * X24 * 1000 <= X64 * 1000
X58 * X17 * 1000 + X62 * X24 * 1000 <= X67 * 1000
X59 * X17 * 1000 + X63 * X24 * 1000 <= X70 * 1000
X32 * (X31 + X32 + X33 + X34) * 1000 + X36 * (X35 + X36 + X37 + X38) * 1000 + X49 * X17 * 900 + X53 * X24 * 900 <= X65 * 1000
X33 * (X31 + X32 + X33 + X34) * 1000 + X37 * (X35 + X36 + X37 + X38) * 1000 + X50 * X17 * 900 + X54 * X24 * 900 <= X68 * 1000
X34 * (X31 + X32 + X33 + X34) * 1000 + X38 * (X35 + X36 + X37 + X38) * 1000 + X51 * X17 * 900 + X55 * X24 * 900 <= X71 * 1000
X39 * X17 * 100 + X16 * 1000 + X42 * X24 * 100 + X23 * 100 + X45 * (X45 + X46 + X47) * 1000 <= X66 * 100
X40 * X17 * 100 + X16 * 1000 + X43 * X24 * 100 + X23 * 100 + X46 * (X45 + X46 + X47) * 1000 <= X69 * 100
X41 * X17 * 100 + X16 * 1000 + X44 * X24 * 100 + X23 * 100 + X47 * (X45 + X46 + X47) * 1000 <= X72 * 100

% 输入值：包含前者对节点间的网络影响
% Dnet = 10 ~ 1000
% im-f2-src-sink = Dnet
% im-f3-src-f3 = Dnet
% im-f3-f3-sink = Dnet
% im-f6-src-f8 = Dnet
% im-f6-f8-f4 = a * Dnet
% im-f6-f8-f7 = (1-a) * Dnet
% im-f6-f4-sink = a * Dnet
% im-f6-f7-sink = (1-a) * Dnet
% im-f5-src-f4 = Dnet
% im-f5-f4-sink = Dnet

% 等式约束：C对资产间网络的需求量，整数
% d-C1-src-sink = X8 * im-f2-src-sink
% d-C1-src-f3 = X9 * im-f3-src-f3
% d-C1-f3-sink = X9 * im-f3-f3-sink
% d-C2-src-f3 = X12 * im-f3-src-f3
% d-C2-f3-sink = X12 * im-f3-f3-sink
% d-C3-src-f8 = X17 * im-f6-src-f8
% d-C3-f8-f4 = X17 * im-f6-f8-f4
% d-C3-f8-f7 = X17 * im-f6-f8-f7
% d-C3-f4-sink = X17 * im-f6-f4-sink
% d-C3-f7-sink = X17 * im-f6-f7-sink
% d-C3-src-f4 = X16 * im-f5-src-f4
% d-C3-f4-sink = X16 * im-f5-f4-sink
% d-C4-src-f8 = X24 * im-f6-src-f8
% d-C4-f8-f4 = X24 * im-f6-f8-f4
% d-C4-f8-f7 = X24 * im-f6-f8-f7
% d-C4-f4-sink = X24 * im-f6-f4-sink
% d-C4-f7-sink = X24 * im-f6-f7-sink
% d-C4-src-f4 = X23 * im-f5-src-f4
% d-C4-f4-sink = X23 * im-f5-f4-sink
% d-C5-src-f4 = X30 * im-f5-src-f4
% d-C5-f4-sink = X30 * im-f5-f4-sink

% 不等式约束：act两个资产请求之间的链路是否被激活，取值0~1
% X8 <= act-C1-src-sink <= im-f2-src-sink
% X9 <= act-C1-src-f3 <= im-f3-src-f3
% X9 <= act-C1-f3-sink <= im-f3-f3-sink
% X12 <= act-C2-src-f3 <= im-f3-src-f3
% X12 <= act-C2-f3-sink <= im-f3-f3-sink
% X17 <= act-C3-src-f8 <= im-f6-src-f8
% X17 <= act-C3-f8-f4 <= im-f6-f8-f4
% X17 <= act-C3-f8-f7 <= im-f6-f8-f7
% X17 <= act-C3-f4-sink <= im-f6-f4-sink
% X17 <= act-C3-f7-sink <= im-f6-f7-sink
% X16 <= act-C3-src-f4 <= im-f5-src-f4
% X16 <= act-C3-f4-sink <= im-f5-f4-sink
% X24 <= act-C4-src-f8 <= im-f6-src-f8
% X24 <= act-C4-f8-f4 <= im-f6-f8-f4
% X24 <= act-C4-f8-f7 <= im-f6-f8-f7
% X24 <= act-C4-f4-sink <= im-f6-f4-sink
% X24 <= act-C4-f7-sink <= im-f6-f7-sink
% X23 <= act-C4-src-f4 <= im-f5-src-f4
% X23 <= act-C4-f4-sink <= im-f5-f4-sink
% X30 <= act-C5-src-f4 <= im-f5-src-f4
% X30 <= act-C5-f4-sink <= im-f5-f4-sink

% e是否被用于两个资产之间的流（与论文中不同）
% f-e1-C1 = X1
% f-e5-C1 = X1
% f-e8-C1 = X1
% f-e9-C1 = X1
% f-e6-C1 = X31
% f-e7-C1 = X32
% f-e4-C1 = X33
% f-e12-C1 = X34
% f-e1-C2 = X2
% f-e5-C2 = X2
% f-e8-C2 = X2
% f-e10-C2 = X2
% f-e6-C2 = X35
% f-e7-C2 = X36
% f-e4-C2 = X37
% f-e12-C2 = X38
% f-e2-C3 = X3
% f-e5-C3 = X3
% f-e8-C3 = X3
% f-e11-C3 = X3
% f-e4-C3 = X40 + X50 + X58
% f-e7-C3 = X39 + X49 + X57
% f-e12-C3 = X41 + X51 + X59
% f-e6-C3 = X48
% f-e2-C4 = X4
% f-e5-C4 = X4
% f-e8-C4 = X4
% f-e9-C4 = X4
% f-e4-C4 = X43 + X54 + X62
% f-e7-C4 = X42 + X53 + X61
% f-e12-C4 = X44 + X55 + X63
% f-e6-C4 = X52
% f-e3-C5 = X5
% f-e5-C5 = X5
% f-e7-C5 = X5
% f-e4-C5 = X43
% f-e7-C5 = X42

% 链路容量限制（暂时不要）
% link = 10000
% uplink = 20000
% f-e1-C1 * d-C1-src-sink + f-e1-C2 * d-C2-src-sink <= link
% f-e2-C3 * d-C3-src-sink + f-e2-C4 * d-C4-src-sink <= link
% f-e3-C5 * d-C5-src-sink <= sink
% f-e9-C1 * d-C1-src-sink + f-e9-C1 * d-C4-src-sink <= sink
% f-e10-C2 * d-C2-src-sink <= sink
% f-e11-C3 * d-C3-src-sink <= sink
% 2 * (f-e8-C1 * d-C1-src-sink + f-e8-C2 * d-C2-src-sink + f-e8-C3 * d-C3-src-sink + f-e8-C4 * d-C4-src-sink) <= sink
% 。。。

% 链路延迟约束（暂时不要）

% C和f失败开销
% fail-C1 = 8
% fail-C2 = 16
% fail-C3 = 32
% fail-C4 = 64
% fail-C5 = 128
% fail-C1-f2 = 4
% fail-C1-f3 = 8
% fail-C2-f3 = 8
% fail-C3-f4 = 16
% fail-C3-f5 = 4
% fail-C3-f6 = 8
% fail-C4-f4 = 16
% fail-C4-f5 = 8
% fail-C4-f6 = 4
% fail-C5-f5 = 32

% 资源使用开销
% corecost-ctr = 0.1
% corecost-edg = 0.2

% 不等式约束：是否使用了节点
X73 = 1
X74 >= X31
X74 >= X35
X74 >= X48
X74 >= X52
X74 <= X31 + X35 + X48 + X52
X75 >= X57
X75 >= X61
X75 >= X32
X75 >= X36
X75 >= X49
X75 >= X53
X75 >= X39
X75 >= X42
X75 >= X45
X75 <= X57 + X61 + X32 + X36 + X49 + X53 + X39 + X42 + X45
X76 >= X58
X76 >= X62
X76 >= X33
X76 >= X37
X76 >= X50
X76 >= X54
X76 >= X40
X76 >= X43
X76 >= X46
X76 <= X58 + X62 + X33 + X37 + X50 + X54 + X40 + X43 + X46
X77 >= X59
X77 >= X63
X77 >= X34
X77 >= X38
X77 >= X51
X77 >= X55
X77 >= X41
X77 >= X44
X77 >= X47
X77 <= X59 + X63 + X34 + X38 + X51 + X55 + X41 + X44 + X47

% 目标函数
cf = 8 * (1 - X1) + 16 * (1 - X2) + 32 * (1 - X3) + 64 * (1 - X4) + 128 * (1 - X5)
ff = 4 * (1 - X8) + 8 * (1 - (X31 + X32 + X33 + X34)) + 8 * (1 - (X35 + X36 + X37 + X38)) + 4 * (1 - X16) + 16 * (1 - (X39 + X40 + X41)) + 8 * (1 - X17) + 16 * (1 - (X42 + X43 + X44)) + 8 * (1 - X23) + 4 * (1 - X24) + 32 * (1 - X30)
re = 0.1 * X28 * 4 + X64 + X65 + X66 + 0.2 * (X67 + X68 + X69 + X70 + X71 + X72) + X73 + X74 + X75 + X76 + X77
min(cf + ff + re)
