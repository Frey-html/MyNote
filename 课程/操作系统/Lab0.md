### 浙江大学实验报告
---
课程名称：操作系统
实验项目名称：Rinux环境搭建和内核编译
学生姓名：展翼飞  学号：3190102196
电子邮件地址：1007921963@qq.com
实验日期： 2022年 9 月 26 日

#### 一、实验内容
##### 1.**搭建Docker环境** 
* 导入docker镜像
	![[导入镜像.jpg|500]]
	```bash
	cat oslab.tar | docker import - oslab:2022
	#使用cat命令加管道 使oslab.tar作为docker import的输入导入，命名为 
    #oslab：2022
	docker images
	#导入docker镜像后查看docker镜像
	chmod a+rw ...
	#给所有用户对docker.sock文件添加读写权限
	```
* 创建容器
![[创建容器.jpg]]	
```bash
	docker run --name oslab -it oslab:2022 /bin/bash
	# --name 指定容器名为oslab -it交互式操作，运行在终端/bin/bash
```


##### 2.**获取Linux源码和已经编译好的文件系统**
* 克隆lab0仓库
![[无法克隆gitee.jpg]]
  遇见问题： docker内git clone命令无法解析地址
  解决： 在docker外克隆后复制到容器内
![[在docker外clone拷贝进docker.jpg]]
```bash
	docker ps 
	#显示所有在运行的容器
	docker cp 源地址 目标容器：目标地址
	#将文件从docker外拷贝到容器内
```
* 安装wget工具
![[wget过程中发现docker不能联网.jpg]]
	遇见问题：docker容器内无法联网
	解决：docker rm删除当前容器，新建容器加入 --net host 与宿主机共享网络配置
	![[解决：带net命令重新创建容器.jpg]]
* 下载linux源码并解压
	![[下载解压完毕.jpg]]
```bash
	tar -zxvf filename
	#tar：用于建立，还原，加入，解开备份（打包）文件
	#-z 通过gzip压缩或解压 -x 从备份文件还原文件
	#-v 显示详细过程 -f 指定备份文件
```


3. **编译Linux内核**
* 配置环境变量
![[配置环境变量.jpg]]
```bash
	export RISCV=/opt/riscv
	#export 设置环境变量 RISCV 仅在当前终端和当前用户生效
	export PATH=$PATH:$RISCV/bin
	#将$RISCV/bin添加到PATH中 
```
* 生成配置
![[使用riscv默认配置.jpg]]
指定内核编译配置为RISC-V平台的默认配置
* 开始编译内核
	![[内核编译选项.jpg]]
![[编译选项2.jpg]]
指定交叉编译工具链，进行多线程编程，线程数为4

4. **使用QEMU运行内核**
![[QEMU调试命令.jpg]]
```bash
	qemu-system-riscv64 #使用qemu完成riscv64架构的模拟
	-nographic #不使用图形化窗口
	-machine virt #指定要模拟的机器为RISCV VirtIO board
	-kernel #指定对应架构的内核镜像 
	-device #指定要模拟的设备 virtio-blk-device指定的为 storage device
	#即存储设备 相应的总线bus为virtio-bus hd0:第一个硬盘
	-append cmdline #使⽤cmdline作为内核的命令⾏
	-bios default #使⽤默认的 OpenSBI firmware 作为 bootloader
	#ttyS0 串行端口终端
	-drive #指定文件系统
```

5. **使用gdb对内核进行调试**
* 使用QEMU启动linux内核
![[terminal1启动内核并暂停cpu.jpg]]
```bash
	-S #启动时暂停cpu执行
	-s #-gdb tcp::1234 的简写，使用gdb调试，可用tcp 1234端口连接
```
* 打开另一个Terminal Session，连接docker使用gdb连接QEMU
![[尝试gdb.jpg]]
	此处显示(No debugging symbols found)，则需重新编译内核
* 修改内核MAKEFILE
![[加入编译选项.jpg]]
* 重新编译内核
![[重新编译.jpg]]

三、讨论心得
1. 在docker内使用git clone失败
	原本认为是gitee连接有问题，遂在docker容器外下载文件系统并拷贝入容器，但在后续实验过程中使用wget，apt等工具时均无法连接到网络，经过查询重新建立容器并加入-net host选项使docker容器与宿主机共享网络配置，解决了容器的联网问题。