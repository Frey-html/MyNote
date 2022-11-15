### 基础学习
* [Unity教程](https://www.bilibili.com/video/BV1HX4y1V71E?p=5&vd_source=8636d68797fa4651942df4dc09db7987)
#### 1.基本界面学习
* Scene窗口快捷键 ![[游戏开发/附件/SharedScreenshot.jpg]]
	[Unity中的快捷键](https://blog.csdn.net/LJH_Gemini/article/details/89052132)

* 反射机制 [Unity C#基础：反射](https://blog.csdn.net/q493201681/article/details/82623802)
	[C#中的反射到底用在哪，通俗解释（unity）、反射的概念及用法](https://blog.csdn.net/qq_32175379/article/details/113880100?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-113880100-blog-82623802.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-113880100-blog-82623802.pc_relevant_default&utm_relevant_index=5)
	[C# 编译机器码过程原理之再谈反射](https://www.cnblogs.com/netlock/p/14177564.html#:~:text=C%23,%E6%BA%90%E4%BB%A3%E7%A0%81%E5%88%B0%E6%9C%BA%E5%99%A8%E7%A0%81%E8%BF%87%E7%A8%8B%EF%BC%9A%201%E3%80%81%E6%BA%90%E4%BB%A3%E7%A0%81%E2%80%94%E2%80%942%E3%80%81%E7%BC%96%E8%AF%91%E5%99%A8%EF%BC%88vs%E8%87%AA%E5%B8%A6%E7%9A%84csc.exe%EF%BC%8C%E8%BF%98%E6%9C%89mono%E7%9A%84mcs.exe%EF%BC%8C%E3%80%90java%E7%BC%96%E8%AF%91%E5%99%A8javac.exe%E3%80%91%EF%BC%89%E2%80%94%E2%80%943%E3%80%81IL%E4%B8%AD%E9%97%B4%E8%AF%AD%E8%A8%80%E5%AD%97%E8%8A%82%E7%A0%81%E2%80%94%E2%80%944%E3%80%81CLR%E5%90%AF%E5%8A%A8JIT%E5%8D%B3%E6%97%B6%E7%BC%96%E8%AF%91%E2%80%94%E2%80%945%E3%80%81%E5%B0%86IL%E7%BC%96%E8%AF%91%E4%B8%BA%E5%8F%AF%E4%BB%A5%E7%9C%9F%E6%AD%A3%E5%9C%A8CPU%E4%B8%8A%E8%BF%90%E8%A1%8C%E7%9A%84%E6%9C%BA%E5%99%A8%E7%A0%81%E3%80%82)
	场景本质是配置文件，Unity工作机制就是利用反射(_一个运行的程序查看本身或者其它程序的元数据的行为就叫做反射_)，动态的创建GameObject对象并且关联各种C#脚本对象在其上

#### 遇到的问题
#### unityHub无法正常打开
[Unity2020或Unity2019安装后无法启动](https://blog.csdn.net/lzw0321/article/details/118540014#:~:text=%E5%8E%9F%E5%9B%A0%20%E9%80%9A%E8%BF%87%E6%9F%A5%E7%9C%8Bunity%20hub%E7%9A%84%E6%97%A5%E5%BF%97%E5%8F%91%E7%8E%B0Unity%20%E5%90%AF%E5%8A%A8%E7%9A%84%E6%97%B6%E5%80%99%E4%BC%9A%E6%A3%80%E6%9F%A5,liences%EF%BC%8C%E5%A6%82%E6%9E%9C%E4%B8%8D%E5%90%88%E8%A7%84%E5%88%99%E6%97%A0%E6%B3%95%E5%90%AF%E5%8A%A8%E3%80%82%20unity%20hub%E7%9A%84%E6%97%A5%E5%BF%97%E5%AD%98%E6%94%BE%E5%9C%A8%EF%BC%9AC%3AUsersqingAppDataRoaming%5B%26Un%26%5DityHublogs%EF%BC%8C%E6%8C%89%E4%B8%8BWin%2BR%EF%BC%8C%E5%9C%A8%E8%BF%90%E8%A1%8C%E4%B8%AD%E8%BE%93%E5%85%A5%EF%BC%9A%25USERPROFILE%25AppDataRoaming%5B%26Un%26%5DityHublogs%EF%BC%8C%E5%9B%9E%E8%BD%A6%E5%B0%B1%E5%8F%AF%E4%BB%A5%E6%89%93%E5%BC%80unity%20hub%20log%E7%9B%AE%E5%BD%95%E3%80%82)
[如何把Unity卸载干净？](https://blog.csdn.net/weixin_46218781/article/details/104959896)