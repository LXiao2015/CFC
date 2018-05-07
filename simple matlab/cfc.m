% 等式约束：chs范围0 ~ 1
% feature model for C1: FirewallOnly
chs-C1-src = chs-C1
chs-C1-sink = chs-C1
chs-C1-f1 = chs-C1
chs-C1-f1 = chs-C1-f2 + chs-C1-f3
% feature model for C2: StrictFirewall
chs-C2-src = chs-C2
chs-C2-sink = chs-C2
chs-C2-f1 = chs-C2
chs-C2-f1 = chs-C2-f3
% feature model for C3: SampledDPI
chs-C3-src = chs-C3
chs-C3-sink = chs-C3
chs-C3-f1 = chs-C3
chs-C3-f1 = chs-C3-f4
chs-C3-f4 = chs-C3-f5 + chs-C3-f6
chs-C3-f6 = chs-C3-f7
chs-C3-f6 = chs-C3-f8
% feature model for C4: FullDPI
chs-C4-src = chs-C4
chs-C4-sink = chs-C4
chs-C4-f1 = chs-C4
chs-C4-f1 = chs-C4-f4
chs-C4-f4 = chs-C4-f5 + chs-C4-f6
chs-C4-f6 = chs-C4-f7
chs-C4-f6 = chs-C4-f8
% feature model for C5: FullDPI
chs-C5-src = chs-C5
chs-C5-sink = chs-C5
chs-C5-f1 = chs-C5
chs-C4-f1 = chs-C4-f4
chs-C4-f4 = chs-C4-f5

% 等式约束：包含的physical feature与对应资产的节点分配
% chs-C1-src = al-n1-C1-src
% chs-C1-sink = al-n9-C1-sink
% chs-C2-src = al-n1-C2-src
% chs-C2-sink = al-n10-C2-sink
% chs-C3-src = al-n2-C3-src
% chs-C3-sink = al-n11-C3-sink
% chs-C4-src = al-n2-C4-src
% chs-C4-sink = al-n9-C4-sink
% chs-C5-src = al-n3-C5-src
% chs-C5-sink = al-n7-C5-sink
chs-C1-f3 = al-n6-C1-f3 + al-n7-C1-f3 + al-n8-C1-f3 + al-n13-C1-f3
chs-C2-f3 = al-n6-C2-f3 + al-n7-C2-f3 + al-n8-C2-f3 + al-n13-C2-f3
chs-C3-f4 = al-n7-C3-f4 + al-n8-C3-f4 + al-n13-C3-f4
chs-C4-f4 = al-n7-C4-f4 + al-n8-C4-f4 + al-n13-C4-f4
chs-C5-f4 = al-n7-C5-f4 + al-n8-C5-f4 + al-n13-C5-f4
chs-C3-f7 = al-n6-C3-f7 + al-n7-C3-f7 + al-n8-C3-f7 + al-n13-C3-f7
chs-C4-f7 = al-n6-C4-f7 + al-n7-C4-f7 + al-n8-C4-f7 + al-n13-C4-f7
chs-C3-f8 = al-n5-C3-f8 + al-n7-C3-f8 + al-n8-C3-f8 + al-n13-C3-f8
chs-C4-f8 = al-n5-C4-f8 + al-n7-C4-f8 + al-n8-C4-f8 + al-n13-C4-f8

% 输入值：包含前者对后者的服务资源影响
a = 0.1
Ds = 100 ~ 1000
im-f3-f3 = Ds
im-f6-f8 = Ds
im-f6-f4 = a * Ds
im-f6-f7 = (1-a) * Ds
im-f5-f4 = Ds

% 等式约束：C对三种资源的需求量，整数
d-C1-fw = chs-C1-f3 * im-f3-f3
d-C2-fw = chs-C2-f3 * im-f3-f3
d-C3-fw = chs-C3-f6 * im-f6-f7
d-C4-fw = chs-C4-f6 * im-f6-f7
d-C3-rt = chs-C3-f6 * im-f6-f8
d-C4-rt = chs-C4-f6 * im-f6-f8
d-C3-idp = chs-C3-f6 * im-f6-f4 + chs-C3-f5 * im-f5-f4
d-C4-idp = chs-C4-f6 * im-f6-f4 + chs-C4-f5 * im-f5-f4
d-C5-idp = chs-C5-f4 * im-f5-f4

% 输入值：单位NF资源占用量、服务能力
cpu-vrt = 1
cpu-vfw = 1
cpu-idp = 1
cpu-vm = 4
mem-vrt = 0.1
mem-vfw = 0.1
mem-idp = 0.5
mem-vm = 8
rps-vrt = 1000
rps-vfw = 1000
rps-idp = 100
rps-hrt = 100000
rps-hfw = 50000

% 输入值：数据中心物理资源总量
ctr-cpu = 1000
ctr-mem = 100000
edg-cpu = 100
edg-mem = 10000

% （非线性？）不等式约束：分配到节点上的服务占用的资源不超过该节点拥有的资源总量
% PNF
used-n5-rps = al-n5-C3-f8 * d-C3-rt + al-n5-C4-f8 * d-C4-rt
used-n6-rps = al-n6-C1-f3 * d-C1-fw + al-n6-C2-f3 * d-C2-fw + al-n6-C3-f7 * d-C3-fw + al-n6-C4-f7 * d-C4-fw
used-n5-rps <= rps-hrt
used-n6-rps <= rps-hfw
% VM + VNF
used-n7-cpu = chs-C5-sink * cpu-vm + num-n7-vrt * cpu-vrt + num-n7-vfw * cpu-vfw + num-n7-idp * cpu-idp
used-n7-mem = chs-C5-sink * mem-vm + num-n7-vrt * mem-vrt + num-n7-vfw * mem-vfw + num-n7-idp * mem-idp
used-n7-cpu <= ctr-cpu
used-n7-mem <= ctr-mem
used-n8-cpu = num-n8-vrt * cpu-vrt + num-n8-vfw * cpu-vfw + num-n8-idp * cpu-idp
used-n8-mem = num-n8-vrt * mem-vrt + num-n8-vfw * mem-vfw + num-n8-idp * mem-idp
used-n13-cpu = num-n13-vrt * cpu-vrt + num-n13-vfw * cpu-vfw + num-n13-idp * cpu-idp
used-n13-mem = num-n13-vrt * mem-vrt + num-n13-vfw * mem-vfw + num-n13-idp * mem-idp
used-n8-cpu <= edg-cpu
used-n8-mem <= edg-mem
used-n13-cpu <= edg-cpu
used-n13-mem <= edg-mem
% VNF被分配的请求不大于实例化num个后能提供的处理能力
al-n7-C3-f8 * d-C3-rt + al-n7-C4-f8 * d-C4-rt <= num-n7-vrt * rps-vrt
al-n8-C3-f8 * d-C3-rt + al-n8-C4-f8 * d-C4-rt <= num-n8-vrt * rps-vrt
al-n13-C3-f8 * d-C3-rt + al-n13-C4-f8 * d-C4-rt <= num-n13-vrt * rps-vrt
al-n7-C1-f3 * d-C1-fw + al-n7-C2-f3 * d-C2-fw + al-n7-C3-f7 * d-C3-fw + al-n7-C4-f7 * d-C4-fw <= num-n7-vfw * rps-vfw
al-n8-C1-f3 * d-C1-fw + al-n8-C2-f3 * d-C2-fw + al-n8-C3-f7 * d-C3-fw + al-n8-C4-f7 * d-C4-fw <= num-n8-vfw * rps-vfw
al-n13-C1-f3 * d-C1-fw + al-n13-C2-f3 * d-C2-fw + al-n13-C3-f7 * d-C3-fw + al-n13-C4-f7 * d-C4-fw <= num-n13-vfw * rps-vfw
al-n7-C3-f4 * d-C3-idp + al-n7-C4-f4 * d-C4-idp + al-n7-C5-f4 * d-C5-idp <= num-n7-idp * rps-idp
al-n8-C3-f4 * d-C3-idp + al-n8-C4-f4 * d-C4-idp + al-n8-C5-f4 * d-C5-idp <= num-n8-idp * rps-idp
al-n13-C3-f4 * d-C3-idp + al-n13-C4-f4 * d-C4-idp + al-n13-C5-f4 * d-C5-idp <= num-n13-idp * rps-idp
`2
% 输入值：包含前者对节点间的网络影响
Dnet = 10 ~ 1000
im-f2-src-sink = Dnet
im-f3-src-f3 = Dnet
im-f3-f3-sink = Dnet
im-f6-src-f8 = Dnet
im-f6-f8-f4 = a * Dnet
im-f6-f8-f7 = (1-a) * Dnet
im-f6-f4-sink = a * Dnet
im-f6-f7-sink = (1-a) * Dnet
im-f5-src-f4 = Dnet
im-f5-f4-sink = Dnet

% 等式约束：C对资产间网络的需求量，整数
d-C1-src-sink = chs-C1-f2 * im-f2-src-sink
d-C1-src-f3 = chs-C1-f3 * im-f3-src-f3
d-C1-f3-sink = chs-C1-f3 * im-f3-f3-sink
d-C2-src-f3 = chs-C2-f3 * im-f3-src-f3
d-C2-f3-sink = chs-C2-f3 * im-f3-f3-sink
d-C3-src-f8 = chs-C3-f6 * im-f6-src-f8
d-C3-f8-f4 = chs-C3-f6 * im-f6-f8-f4
d-C3-f8-f7 = chs-C3-f6 * im-f6-f8-f7
d-C3-f4-sink = chs-C3-f6 * im-f6-f4-sink
d-C3-f7-sink = chs-C3-f6 * im-f6-f7-sink
d-C3-src-f4 = chs-C3-f5 * im-f5-src-f4
d-C3-f4-sink = chs-C3-f5 * im-f5-f4-sink
d-C4-src-f8 = chs-C4-f6 * im-f6-src-f8
d-C4-f8-f4 = chs-C4-f6 * im-f6-f8-f4
d-C4-f8-f7 = chs-C4-f6 * im-f6-f8-f7
d-C4-f4-sink = chs-C4-f6 * im-f6-f4-sink
d-C4-f7-sink = chs-C4-f6 * im-f6-f7-sink
d-C4-src-f4 = chs-C4-f5 * im-f5-src-f4
d-C4-f4-sink = chs-C4-f5 * im-f5-f4-sink
d-C5-src-f4 = chs-C5-f5 * im-f5-src-f4
d-C5-f4-sink = chs-C5-f5 * im-f5-f4-sink

% 不等式约束：act两个资产请求之间的链路是否被激活，取值0~1
chs-C1-f2 <= act-C1-src-sink <= im-f2-src-sink
chs-C1-f3 <= act-C1-src-f3 <= im-f3-src-f3
chs-C1-f3 <= act-C1-f3-sink <= im-f3-f3-sink
chs-C2-f3 <= act-C2-src-f3 <= im-f3-src-f3
chs-C2-f3 <= act-C2-f3-sink <= im-f3-f3-sink
chs-C3-f6 <= act-C3-src-f8 <= im-f6-src-f8
chs-C3-f6 <= act-C3-f8-f4 <= im-f6-f8-f4
chs-C3-f6 <= act-C3-f8-f7 <= im-f6-f8-f7
chs-C3-f6 <= act-C3-f4-sink <= im-f6-f4-sink
chs-C3-f6 <= act-C3-f7-sink <= im-f6-f7-sink
chs-C3-f5 <= act-C3-src-f4 <= im-f5-src-f4
chs-C3-f5 <= act-C3-f4-sink <= im-f5-f4-sink
chs-C4-f6 <= act-C4-src-f8 <= im-f6-src-f8
chs-C4-f6 <= act-C4-f8-f4 <= im-f6-f8-f4
chs-C4-f6 <= act-C4-f8-f7 <= im-f6-f8-f7
chs-C4-f6 <= act-C4-f4-sink <= im-f6-f4-sink
chs-C4-f6 <= act-C4-f7-sink <= im-f6-f7-sink
chs-C4-f5 <= act-C4-src-f4 <= im-f5-src-f4
chs-C4-f5 <= act-C4-f4-sink <= im-f5-f4-sink
chs-C5-f5 <= act-C5-src-f4 <= im-f5-src-f4
chs-C5-f5 <= act-C5-f4-sink <= im-f5-f4-sink

% e是否被用于两个资产之间的流（与论文中不同）
f-e1-C1 = chs-C1
f-e5-C1 = chs-C1
f-e8-C1 = chs-C1
f-e9-C1 = chs-C1
f-e6-C1 = al-n6-C1-f3
f-e7-C1 = al-n7-C1-f3
f-e4-C1 = al-n8-C1-f3
f-e12-C1 = al-n13-C1-f3
f-e1-C2 = chs-C2
f-e5-C2 = chs-C2
f-e8-C2 = chs-C2
f-e10-C2 = chs-C2
f-e6-C2 = al-n6-C2-f3
f-e7-C2 = al-n7-C2-f3
f-e4-C2 = al-n8-C2-f3
f-e12-C2 = al-n13-C2-f3
f-e2-C3 = chs-C3
f-e5-C3 = chs-C3
f-e8-C3 = chs-C3
f-e11-C3 = chs-C3
f-e4-C3 = al-n8-C3-f4 + al-n8-C3-f7 + al-n8-C3-f8
f-e7-C3 = al-n7-C3-f4 + al-n7-C3-f7 + al-n7-C3-f8
f-e12-C3 = al-n13-C3-f4 + al-n13-C3-f7 + al-n13-C3-f8
f-e6-C3 = al-n6-C3-f7
f-e2-C4 = chs-C4
f-e5-C4 = chs-C4
f-e8-C4 = chs-C4
f-e9-C4 = chs-C4
f-e4-C4 = al-n8-C4-f4 + al-n8-C4-f7 + al-n8-C4-f8
f-e7-C4 = al-n7-C4-f4 + al-n7-C4-f7 + al-n7-C4-f8
f-e12-C4 = al-n13-C4-f4 + al-n13-C4-f7 + al-n13-C4-f8
f-e6-C4 = al-n6-C4-f7
f-e3-C5 = chs-C5
f-e5-C5 = chs-C5
f-e7-C5 = chs-C5
f-e4-C5 = al-n8-C4-f4
f-e7-C5 = al-n7-C4-f4

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
fail-C1 = 8, 16, 32, 64, 128
fail-C2 = 8, 16, 32, 64, 128
fail-C3 = 8, 16, 32, 64, 128
fail-C4 = 8, 16, 32, 64, 128
fail-C5 = 8, 16, 32, 64, 128
fail-C1-f2 = 4, 8, 16, 32, 64
fail-C1-f3 = 4, 8, 16, 32, 64
fail-C2-f3 = 4, 8, 16, 32, 64
fail-C3-f5 = 4, 8, 16, 32, 64
fail-C3-f6 = 4, 8, 16, 32, 64
fail-C3-f4 = 4, 8, 16, 32, 64
fail-C4-f4 = 4, 8, 16, 32, 64
fail-C4-f5 = 4, 8, 16, 32, 64
fail-C4-f6 = 4, 8, 16, 32, 64
fail-C5-f5 = 4, 8, 16, 32, 64

% 资源使用开销
corecost-ctr = 0.1
corecost-edg = 0.2

% 不等式约束：是否使用了节点
u-n5 = 1
u-n6 >= al-n6-C1-f3
u-n6 >= al-n6-C2-f3
u-n6 >= al-n6-C3-f7
u-n6 >= al-n6-C4-f7
u-n6 <= al-n6-C1-f3 + al-n6-C2-f3 + al-n6-C3-f7 + al-n6-C4-f7
u-n7 >= al-n7-C3-f8
u-n7 >= al-n7-C4-f8
u-n7 >= al-n7-C1-f3
u-n7 >= al-n7-C2-f3
u-n7 >= al-n7-C3-f7
u-n7 >= al-n7-C4-f7
u-n7 >= al-n7-C3-f4
u-n7 >= al-n7-C4-f4
u-n7 >= al-n7-C5-f4
u-n7 <= al-n7-C3-f8 + al-n7-C4-f8 + al-n7-C1-f3 + al-n7-C2-f3 + al-n7-C3-f7 + al-n7-C4-f7 + al-n7-C3-f4 + al-n7-C4-f4 + al-n7-C5-f4
u-n8 >= al-n8-C3-f8
u-n8 >= al-n8-C4-f8
u-n8 >= al-n8-C1-f3
u-n8 >= al-n8-C2-f3
u-n8 >= al-n8-C3-f7
u-n8 >= al-n8-C4-f7
u-n8 >= al-n8-C3-f4
u-n8 >= al-n8-C4-f4
u-n8 >= al-n8-C5-f4
u-n8 <= al-n8-C3-f8 + al-n8-C4-f8 + al-n8-C1-f3 + al-n8-C2-f3 + al-n8-C3-f7 + al-n8-C4-f7 + al-n8-C3-f4 + al-n8-C4-f4 + al-n8-C5-f4
u-n13 >= al-n13-C3-f8
u-n13 >= al-n13-C4-f8
u-n13 >= al-n13-C1-f3
u-n13 >= al-n13-C2-f3
u-n13 >= al-n13-C3-f7
u-n13 >= al-n13-C4-f7
u-n13 >= al-n13-C3-f4
u-n13 >= al-n13-C4-f4
u-n13 >= al-n13-C5-f4
u-n13 <= al-n13-C3-f8 + al-n13-C4-f8 + al-n13-C1-f3 + al-n13-C2-f3 + al-n13-C3-f7 + al-n13-C4-f7 + al-n13-C3-f4 + al-n13-C4-f4 + al-n13-C5-f4

% 目标函数
cf = fail-C1 * (1 - chs-C1) + fail-C2 * (1 - chs-C2) + fail-C3 * (1 - chs-C3) + fail-C4 * (1 - chs-C4) + fail-C5 * (1 - chs-C5)
ff = fail-C1-f2 * (1 - chs-C1-f2) + fail-C1-f3 * (1 - chs-C1-f3) + fail-C2-f3 * (1 - chs-C2-f3) + fail-C3-f5 * (1 - chs-C3-f5) + fail-C3-f4 * (1 - chs-C3-f4) + fail-C3-f6 * (1 - chs-C3-f6) + fail-C4-f4 * (1 - chs-C4-f4) + fail-C4-f5 * (1 - chs-C4-f5) + fail-C4-f6 * (1 - chs-C4-f6) + fail-C5-f5 * (1 - chs-C5-f5)
re = corecost-ctr * used-n7-cpu + corecost-edg * (used-n8-cpu + used-n13-cpu) + u-n5 + u-n6 + u-n7 + u-n8 + u-n13
min(cf + ff + re)
