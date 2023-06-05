#### L1 Fundamentals of Quantitative Design
**概念**：
    * ooo:out of order 乱序执行
	* super scaler：超标量 同时多条指令进ex
Architecture:
	1. ISA 2.Computer orgnization 3.System Design

* 成倍的核心未必带来成倍的效率，需要一定的协调和同步成本（Amdahl's law）

**CPU发展的三个关键**
1. ILP(instruction level parallel) Wall
2. Memory Wall
3. Power Wall

* risc与cisc：
[什么是RISC-V - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/49176102)

#### L2 Fundamentals of Quantitative Design
* 根据指令和数据流数量分类计算机架构：
  SISD MISD SIMD MIMD 
  
* Capacitive load容性负载：
	cpu内部均为晶体管和电阻，没有电容，晶体管有电极和半导体材料，导通时类似于电容充电
* Rule of thumb：
	电压降低需要频率降低，晶体管导通与电压变化需要时间，避免信号不能被正确传递

* 设计多核，降低单核频率和电压，可以提高性能能耗比

* **Reliability**
	* MTTF: mean time ro failure
	* MTTR: mean time to repair
	* FIT: failure in time = 1 / MTTF
	* MTBF: mean time between failure = MTTF + MTTR
	* availability = MTTF / MTBF

* Time
	* Wall clock time: elapsed time 进程运行总时间
	* CPU time: cpu计算时间，不包含IO = user time + system time

* Mips
	* mips的计算是和benchmark相关的，不同体系架构带来不同指令集，有些复杂的多周期指令需要不止一个时钟周期












### CH2 Memory hierarchy
* Random access memory
	* DRAM dynamic
		* high density, low power, cheap, slow
		* 需要时常刷新
	* SRAM static
		* low density, high power, expensive, fast
		* 内容直到关掉电源前会一直持续，无需刷新
main memory: dram
cpu cache: sram (dram的充放电会干扰cpu运算)

* 对于嵌入式的实时系统而言，重要的是时间一致性，一般会避免使用cache（避免cache hit 和cache miss penalty 带来的时间不一致）

* Cache三种映射方法 ：directly mapped, fully associated, set associated

* split cache 和 mixed cache
	指令cache的命中率一般比数据cache命中率高
	数据存储可能比较分散，或者数据连续但数量很多，导致miss rate高

* Q：每个进程独立的虚拟内存和cache中的地址如何对应？每个进程也会通过软中断跳转到操作系统的内存空间，以及进程切换时cache如何处理？
	* 可以给操作系统单独分一个cache



### 补充
* 汇编分支循环的效率受cpu的跳转预测方式影响（预测跳或者预测不跳）
	但现在的编译器形成汇编代码一般都会把分支判断放在循环最后（只有结尾一个branch）