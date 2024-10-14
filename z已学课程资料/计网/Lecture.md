### CH1 Intro
#### 网络简介
* 网络模式：
	* Client-server 客户端和服务器
	* Peer-to-peer：多个个体形成松散的组织互相交换信息
* 两种传信技术：
	* Broadcast links, broadcast networks
		* broadcasting, multicasting（多播）
	* Point to point
		* unicasting（单播）
* 网络类型分类（依据范围）
	* Personal Area Network（PAN）如蓝牙
	* Local Area Network(LAN) 广播式
	* Metropolitan Area Network(MAN)
	* Wide Area Network(WAN) 点到点
* Wifi，蓝牙一般为2.4GHZ，功率不能太大否则容易互相干扰

* 两种通讯途径建立方式：虚电路子网和数据报子网

#### 网络软件
OSI模型与TCP/IP模型
![[Pasted image 20220915203831.png]]
![[Pasted image 20220915203919.png]]
![[Pasted image 20220915203953.png]]
OSI参考模型是学术上和法律上的国际标准，是完整的权威的网络**参考模型**。而TCP/IP参考模型是事实上的国际标准，即现实生活中被广泛使用的网络参考模型
参考：[[协议详解]]
#### 面向连接和无连接协议

#### 服务原语 service primitives
* Listen connect receive send disconnect

* Internet起源：
	* ARPANET (advanced research projects agency)
		* 为了改善树状结构的电话网络，避免关键节点被摧毁时无法通信（电话交换局）
* Internet Ethernet WirelessLAN



---
### CH2  Physical Layer
* tips: K M G，在表示文件，内存，硬盘大小等为2进制（2^10, 2^20, 2^30），表示网速时为10进制（10^3 10^6 10^9） bps

* 调制解调器：完成数字信号与模拟信号的转换
	* 调幅 调频 相对调相 绝对调相

* **带宽**：通信频率范围 或者 传信速率
* **波特Baud和比特率bps**
	* 2400hz = 2400 baud 一个码元携带4bits时比特率为9600bps
* **最大信息传输速率** 

* 通信子网：
	虚电路子网和数据包子网
* 分组交换方式：从电话系统到计算机网络
	电话系统是连续的bit流
	计算机网络将数据打包，存储转发 store and foward


### CH3 The Data Link Layer
* **framing** 成帧 确定帧开始和结束
	character count
	**byte stuffing(character stuffing)**
	**bit stuffing**
	physical layer coding violation