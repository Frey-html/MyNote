### 使用WebMagic爬取小说

#### 一、环境搭建
* 开发工具： IDEA
* 爬虫工具：WebMagic 不使用Maven，将下载的jar包在IDE的项目中导入

#### 二、编写代码
##### 0.理解WebMagic处理流程
![[Pasted image 20231020174140.png|300]]
* 只需编写自己的PageProcessor提取网页信息，Pipline对信息进行持久化

##### 1.MyPageProcessor类
* 继承自PageProcessor，实现process函数定义如何处理页面信息
* 本次爬虫所爬取的页面分为列表页和文章页，列表页需要抓取文章页url的地址并放入处理队列，文章页需要抓取文本内容而无需额外抓取页面内url地址
* 使用Xpath与正则表达式提取所在网页Html内的所需信息
![[Pasted image 20231020174609.png]]

##### 2.MyPipline类
* 继承自Pipline，实现process函数定义如何持久化爬取信息
* 使用文件输出流将爬取内容转化为txt文件中的文本内容
![[Pasted image 20231020174811.png]]


#### 3.测试与结果
* MyPageProcesser中可调整线程数与休眠时间，爬取时Console输出已经爬取到的文章页的标题![[Pasted image 20231020175001.png|300]]
* 在MyPipline中可定义输出文件地址，默认为`"C:\\Users\\Frey\\Desktop\\MyNovel.txt"`，需要根据用户修改地址
* 结果文件截图![[Pasted image 20231020175218.png|300]]