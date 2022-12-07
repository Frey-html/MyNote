### Python爬虫
* 爬虫的结构[Python 爬虫介绍](https://www.runoob.com/w3cnote/python-spider-intro.html#:~:text=%E7%88%AC%E8%99%AB%20%EF%BC%9A%E4%B8%80%E6%AE%B5%E8%87%AA%E5%8A%A8%E6%8A%93%E5%8F%96%E4%BA%92%E8%81%94%E7%BD%91%E4%BF%A1%E6%81%AF%E7%9A%84%E7%A8%8B%E5%BA%8F%EF%BC%8C%E4%BB%8E%E4%BA%92%E8%81%94%E7%BD%91%E4%B8%8A%E6%8A%93%E5%8F%96%E5%AF%B9%E4%BA%8E%E6%88%91%E4%BB%AC%E6%9C%89%E4%BB%B7%E5%80%BC%E7%9A%84%E4%BF%A1%E6%81%AF%E3%80%82,Python%20%E7%88%AC%E8%99%AB%E6%9E%B6%E6%9E%84%E4%B8%BB%E8%A6%81%E7%94%B1%E4%BA%94%E4%B8%AA%E9%83%A8%E5%88%86%E7%BB%84%E6%88%90%EF%BC%8C%E5%88%86%E5%88%AB%E6%98%AF%E8%B0%83%E5%BA%A6%E5%99%A8%E3%80%81URL%E7%AE%A1%E7%90%86%E5%99%A8%E3%80%81%E7%BD%91%E9%A1%B5%E4%B8%8B%E8%BD%BD%E5%99%A8%E3%80%81%E7%BD%91%E9%A1%B5%E8%A7%A3%E6%9E%90%E5%99%A8%E3%80%81%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%EF%BC%88%E7%88%AC%E5%8F%96%E7%9A%84%E6%9C%89%E4%BB%B7%E5%80%BC%E6%95%B0%E6%8D%AE%EF%BC%89%E3%80%82)
* 关于cookie[Cookie 从哪里来，网站用它来干嘛?](https://www.zhihu.com/question/22396128)
* **参考教程**：
	* [网络爬虫是什么](http://c.biancheng.net/python_spider/what-is-spider.html)
	* [尚硅谷_爬虫_爬虫相关概念介绍](https://www.bilibili.com/video/BV1Db4y1m7Ho?p=52&vd_source=8636d68797fa4651942df4dc09db7987)

#### 练习
##### 爬百度首页
* 使用urllib.request
	先urlopen发送请求得到相应，再对相应进行read().decode("UTF-8")得到html文件内容并解码到UTF-8，最后print即可
* 遇见的问题：
	1. 使用url为https时不能正确返回完整html，而是包含noscript标签的简短html
		注意url的协议
	2. F12查看字符集为utf-8，但在解码为UTF-8后仍出现中文乱码
		python中字符存储方式为utf-8，且对字符串string和字节型bytes做了区分，string encode到bytes，bytes decode到string
		不指定编码方式写入时，写入方式为w相当于wb且encode为系统默认编码（win为gbk，所以再用utf打开时会乱码）
	3. 爬取所得html与浏览器直接F12的html不一致（缺少cookie等）

##### urllib基础
* **基础知识**
	**[urllib详情](https://blog.csdn.net/liyuanjinglyj/article/details/118697545)**
	[HTTP头部详解](https://blog.csdn.net/sinat_34166518/article/details/83584910)
	[urlretrieve](https://blog.csdn.net/u012424313/article/details/82222188)
* **User-Agent反爬**：
	使用urlretrieve爬取wallheaven一张图片时 HTTP Error 403，该网页有反爬虫处理（使用https爬百度首页信息不完整也可以通过F12查看HTTP请求的User-Agent获得相应信息来模拟浏览器）![[屏幕截图 2022-12-07 165615 1.png]]
* 请求对象定制
	使用User-Agent字典和url在urllib.request.Request()方法中合成request对象，再传入urlopen
	注意Request方法第二个参数不是headers而是data，故需要指明变量和参数关系
	```python
request = urllib.request.Request(url=url, headers = headers)
	```

##### **编解码**
基础知识：
	[Python 字符串（string）和字节型（bytes）](https://blog.csdn.net/Robin_Pi/article/details/111603990#:~:text=Python%E7%9A%84%E5%AD%97%E7%AC%A6%E4%B8%B2%E7%B1%BB%E5%9E%8B%E6%98%AF%20str%20%EF%BC%8C%E5%9C%A8%20%E5%86%85%E5%AD%98%20%E4%B8%AD%E4%BB%A5%20Unicode%20%E8%A1%A8%E7%A4%BA%EF%BC%8C%E4%B8%80%E4%B8%AA%E5%AD%97%E7%AC%A6%E5%AF%B9%E5%BA%94%E8%8B%A5%E5%B9%B2%E4%B8%AA%E5%AD%97%E8%8A%82%20%E5%A6%82%E6%9E%9C%E8%A6%81%E5%9C%A8%E7%BD%91%E7%BB%9C%E4%B8%8A,%E5%88%B0%E7%A3%81%E7%9B%98%E4%B8%8A%EF%BC%8C%E5%B0%B1%E9%9C%80%E8%A6%81%E6%8A%8Astr%20%E7%BC%96%E7%A0%81%20%E4%BB%A5%E5%AD%97%E8%8A%82%E4%B8%BA%E5%8D%95%E4%BD%8D%E7%9A%84bytes%EF%BC%8C%E5%8D%B3%E7%BD%91%E7%BB%9C%E4%BC%A0%E8%BE%93%E5%92%8C%E6%8E%A5%E6%94%B6%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E9%83%BD%E6%98%AF%20bytes%20%EF%BC%8C%E5%A6%82%E8%BF%87%E6%83%B3%E8%A6%81%E8%A7%A3%E8%AF%BB%E5%88%99%E9%9C%80%E8%A6%81%E5%B0%86bytes%20%E8%A7%A3%E7%A0%81%20%E4%B8%BA%20str)
	[Python文件读写注意编码](https://www.cnblogs.com/lmygbl/p/10121909.html#:~:text=Python%E6%96%87%E4%BB%B6%E8%AF%BB%E5%86%99%E6%B3%A8%E6%84%8F%E7%BC%96%E7%A0%81.%20%E6%96%87%E4%BB%B6%E7%9A%84%E8%AF%BB%E5%86%99%E6%93%8D%E4%BD%9C.%201.python3%E4%B8%AD%E7%9A%84%E9%BB%98%E8%AE%A4%E7%9A%84%E7%BC%96%E7%A0%81%E8%A7%A3%E7%A0%81%E6%96%B9%E5%BC%8F%E4%B8%BAutf-8.%20windows%E9%BB%98%E8%AE%A4%E4%BD%BF%E7%94%A8%E7%9A%84%E7%BC%96%E7%A0%81%E6%96%B9%E5%BC%8F%E4%B8%BAgbk.%20%E5%BD%93%E8%AF%BB%E5%8F%96%E4%B8%80%E4%B8%AAwindows%E6%96%87%E4%BB%B6%20%28gbk%29%2C%E5%8F%AF%E4%BB%A5%E6%8C%87%E5%AE%9A%E8%AF%BB%E5%8F%96%E6%97%B6%E4%BD%BF%E7%94%A8%E7%9A%84%E7%BC%96%E7%A0%81encoding%3D%20%28%27gbk%27%29%E6%88%96%E8%80%85%E4%B8%8D%E9%9C%80%E8%A6%81%E6%8C%87%E5%AE%9A.%20with,rb%20a%20%28%E8%BF%BD%E5%8A%A0%E5%86%99%29%20with%20open%20%28%27a.txt%27%2C%27w%27%29%20as%20f%3A)![[屏幕截图 2022-12-06 231229 1.png]]
	[Unicode 和 UTF-8](https://zhuanlan.zhihu.com/p/51828216)
	[百分号编码（URL编码）](https://www.cnblogs.com/nyw1983/p/11755792.html#:~:text=%E5%AF%B9%E4%BA%8EUnicode%E5%AD%97%E7%AC%A6%EF%BC%8CRFC%E6%96%87%E6%A1%A3%E5%BB%BA%E8%AE%AE%E4%BD%BF%E7%94%A8utf-8%E5%AF%B9%E5%85%B6%E8%BF%9B%E8%A1%8C%E7%BC%96%E7%A0%81%E5%BE%97%E5%88%B0%E7%9B%B8%E5%BA%94%E7%9A%84%E5%AD%97%E8%8A%82%EF%BC%8C%E7%84%B6%E5%90%8E%E5%AF%B9%E6%AF%8F%E4%B8%AA%E5%AD%97%E8%8A%82%E6%89%A7%E8%A1%8C%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81%E3%80%82%20%E5%A6%82%E2%80%9C%E4%B8%AD%E6%96%87%E2%80%9D%E4%BD%BF%E7%94%A8UTF-8%E5%AD%97%E7%AC%A6%E9%9B%86%E5%BE%97%E5%88%B0%E7%9A%84%E5%AD%97%E8%8A%82%E4%B8%BA0xE4%200xB8,0xAD%200xE6%200x96%200x87%EF%BC%8C%E7%BB%8F%E8%BF%87Url%E7%BC%96%E7%A0%81%E4%B9%8B%E5%90%8E%E5%BE%97%E5%88%B0%E2%80%9C%25E4%25B8%25AD%25E6%2596%2587%E2%80%9D%E3%80%82)


#### 疑问
* 在复制粘贴是是否发生了编解码？为什么python中字符串"周杰伦"和"%E5%91%A8%E6%9D%B0%E4%BC%A6"在编解码时会有区别：
	因为url还需要进行url编码[百分号编码（URL编码） - Cloud% - 博客园 (cnblogs.com)](https://www.cnblogs.com/nyw1983/p/11755792.html#:~:text=%E5%AF%B9%E4%BA%8EUnicode%E5%AD%97%E7%AC%A6%EF%BC%8CRFC%E6%96%87%E6%A1%A3%E5%BB%BA%E8%AE%AE%E4%BD%BF%E7%94%A8utf-8%E5%AF%B9%E5%85%B6%E8%BF%9B%E8%A1%8C%E7%BC%96%E7%A0%81%E5%BE%97%E5%88%B0%E7%9B%B8%E5%BA%94%E7%9A%84%E5%AD%97%E8%8A%82%EF%BC%8C%E7%84%B6%E5%90%8E%E5%AF%B9%E6%AF%8F%E4%B8%AA%E5%AD%97%E8%8A%82%E6%89%A7%E8%A1%8C%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81%E3%80%82%20%E5%A6%82%E2%80%9C%E4%B8%AD%E6%96%87%E2%80%9D%E4%BD%BF%E7%94%A8UTF-8%E5%AD%97%E7%AC%A6%E9%9B%86%E5%BE%97%E5%88%B0%E7%9A%84%E5%AD%97%E8%8A%82%E4%B8%BA0xE4%200xB8,0xAD%200xE6%200x96%200x87%EF%BC%8C%E7%BB%8F%E8%BF%87Url%E7%BC%96%E7%A0%81%E4%B9%8B%E5%90%8E%E5%BE%97%E5%88%B0%E2%80%9C%25E4%25B8%25AD%25E6%2596%2587%E2%80%9D%E3%80%82)

