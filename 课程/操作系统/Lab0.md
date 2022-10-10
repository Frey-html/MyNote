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
```
	
```
1. **编译Linux内核**
2. **使用QEMU运行内核**
3. **使用gdb对内核进行调试**
二、思考题

三、讨论心得
1. 在docker内使用git clone失败