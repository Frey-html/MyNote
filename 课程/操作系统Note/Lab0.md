### 浙江大学实验报告
---
课程名称：操作系统
实验项目名称：GDB & QEMU 调试 64 位 RISC-V LINUX
学生姓名：展翼飞  学号：3190102196
电子邮件地址：1007921963@qq.com
实验日期： 2024年9月 12日

#### 一、实验内容
##### 1.搭建实验环境
* 使用 WSL 2构建Ubuntu 22.04.3 LTS虚拟机，并安装编译内核所需要的交叉编译工具链和用于构建程序的软件包
```bash
$ sudo apt install gcc-riscv64-linux-gnu 
$ sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev \ gawk build-essential bison flex texinfo gperf libtool patchutils bc \ zlib1g-dev libexpat-dev git
#sudo 使用管理员权限 apt 包管理工具
```
在使用apt安装包时，遇到如下问题：
![[Pic/locate.jpg]]
在使用sudo apt-get update与sudo apt-get upgrade更新包列表与apt软件后安装命令正常运行

* 安装用于启动 riscv64 平台上的内核的模拟器 `qemu`
```bash
$ sudo apt install qemu-system-misc
```

* `gdb` 来对在 `qemu` 上运行的 Linux 内核进行调试
```bash
$ sudo apt install gdb-multiarch
```

#### 2. 获取 Linux 源码和已经编译好的文件系统
* 从 [https://www.kernel.org](https://www.kernel.org/) 下载最新的 Linux 源码 6.11-rc2，并拷贝至wsl用户目录中，并解压
```bash
$ tar -zxvf linux-6.11-rc7.tar.gz -C ~
```
![[Pasted image 20240911153733.png]]

* 使用 git 工具 clone仓库：https://github.com/ZJU-SEC/os24fall-stu。其中已经准备好了根文件系统的镜像
```bash
$ git clone https://github.com/ZJU-SEC/os24fall-stu.git 
$ cd os24fall-stu/src/lab0 
$ ls 
rootfs.img # 已经构建完成的根文件系统的镜像
```
![[Pasted image 20240911153901.png]]

#### 3.编译 Linux 内核
* 进入解压后的linux内核源码文件夹，编译linux内核
```bash
$ cd linux-6.11-rc7
$ make ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- defconfig
#在内核根目录下根据RISC-V默认配置生成一个名为 `.config` 的文件，包含了内核完整的配置，内核在编译时会根据 `.config` 进行编译
$ make ARCH=riscv CROSS_COMPILE=riscv64-linux-gnu- -j4
#使用4线程编译内核
```

* 编译后结果如下图所示![[Pasted image 20240911204913.png]]


#### 4.使用 QEMU 运行内核
编译内核后，在内核源码文件夹中使用QEMU运行内核：
```bash
$ qemu-system-riscv64 -nographic -machine virt -kernel ./arch/riscv/boot/Image -device virtio-blk-device,drive=hd0 -append "root=/dev/vda ro console=ttyS0" -bios default -drive file=../os24fall-stu/src/lab0/rootfs.img,format=raw,id=hd0
#`-nographic`: 不使用图形窗口，使用命令行
#`-machine`: 指定要 emulate 的机器为RISC-V VirtIO board
#`-kernel`: 指定内核 image为该路径下的linux内核
#`-device`: 指定要模拟的设备为virtio-blk-device，并指定硬盘设备hd0作为后端
#`-append cmdline`: 使用 cmdline 作为内核的命令行
#`-bios default`: 使用默认的 OpenSBI firmware 作为 bootloader
#`-drive, file=<file_name>`: 使用rootfs.img作为文件系统
```
结果如下图：
![[Pasted image 20240911205215.png]]
![[Pasted image 20240911205245.png]]


#### 5.使用 GDB 对内核进行调试
开启两个 Terminal Session，一个 Terminal 使用 QEMU 启动 Linux，另一个 Terminal 使用 GDB 与 QEMU 远程通信（使用 tcp::1234 端口）进行调试：
```bash
#Terminal 1
$ qemu-system-riscv64 -nographic -machine virt -kernel ./arch/riscv/boot/Image -device virtio-blk-device,drive=hd0 -append "root=/dev/vda ro console=ttyS0" -bios default -drive file=../os24fall-stu/src/lab0/rootfs.img,format=raw,id=hd0 -S -s
#- `-S`: 启动时暂停 CPU 执行
#- `-s`: `-gdb tcp::1234`的简写

#Terminal 2
$ gdb-multiarch ~/linux-6.11-rc7/vmlinux
(gdb) target remote :1234 # 连接 qemu 
(gdb) b start_kernel # 设置断点 
(gdb) continue # 继续执行 
(gdb) quit # 退出 gdb
```
结果：
Terminal 1启动后直接停止执行，直至Terminal 2连结qemu键入gdb continue指令后继续执行
![[Pasted image 20240911210502.png]]
运行到断点时：
![[Pasted image 20240911210650.png]]
gdb调试结束后：
![[Pasted image 20230921210252.png]]


Terminal 2通过gdb远程连接Terminal 1中的qemu进行调试：
![[Pasted image 20240911210722.png]]

#### 二、思考题
##### 1. 使用 `riscv64-linux-gnu-gcc` 编译单个 `.c` 文件
* 编写一个简单的.c文件，并拷贝至wsl
![[Pasted image 20230921225753.png|300]]
* 使用`riscv64-linux-gnu-gcc`编译得到编译产物a.out：
![[Pasted image 20240911210942.png]]

##### 2. 使用 `riscv64-linux-gnu-objdump` 反汇编 1 中得到的编译产物
```shell
$ riscv64-linux-gnu-objdump -d a.out  #编译指令-d显示程序可执行部分反汇编结果
```
其中main函数反汇编结果如下：
![[Pasted image 20230921234736.png|500]]


##### 3. 调试 Linux 时 :
1. 在 GDB 中查看汇编代码
使用gdb连接qemu后，使用指令`layout asm`查看汇编代码![[Pasted image 20230921235644.png|300]]
2. 在 0x80000000 处下断点
![[Pasted image 20230921235834.png]]
3. 查看所有已下的断点
![[Pasted image 20230921235943.png]]
4. 在 0x80200000 处下断点
![[Pasted image 20230922000024.png]]
5. 清除 0x80000000 处的断点
![[Pasted image 20230922000216.png]]
6. 继续运行直到触发 0x80200000 处的断点
![[Pasted image 20230922001044.png]]
7. 单步调试一次
![[Pasted image 20230922001247.png]]
8. 退出 QEMU


##### 4. 使用 `make` 工具清除 Linux 的构建产物
使用指令`$ make clean`即可清除当前文件夹下的构建产物，同时可以编写makefile中`clean:`的部分使用命令行命令指定需要清除的构建产物

##### 5. `vmlinux` 和 `Image` 的关系和区别是什么？
* vmlinux是Linux内核编译出来的原始的内核文件，elf格式，未做压缩处理，该映像包含整个内核和 debug 标识。该映像可用于定位内核问题，但体积太大缺乏压缩不能直接引导Linux系统启动。
* Image是Linux内核编译时，使用objcopy处理vmlinux后生成的二进制内核映像。它是压缩过的系统映像，可直接引导Linux系统启动，在启动过程中解压释放到内存中。


#### 三、讨论心得
1. 在使用wsl，docker之类工具新建linux操作系统环境时，使用apt-get获取软件前要注意使用apt-get update更新软件列表，避免unable to locate的错误
2. 在cpu核心数量与内存大小允许时，编译时可以增加多线程编译的指令来缩短编译时间
3. 操作系统内核源码在不同的硬件体系结构下既有跨架构的相同部分，也有依赖于体系结构的代码部分，根据不同环境要选择相应的编译工具链与配置