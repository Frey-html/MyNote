**String：**
字符串常量，字符串长度不可变
用于存放字符的数组被声明为 final 的，因此只能赋值一次，不可再更改
每次对String的操作都会生成新的String对象

**StringBuffer**
任何对StringBuffer指向的字符串的操作不会产生新的对象StringBuffer对象有一定缓冲区容量，字符串大小没有超过容量时，不会分配新的容量，若超过会自动增加容量

**StringBuilder**
任何对StringBuilder指向的字符串的操作不会产生新的对象
StringBuffer 的所有公开方法都是 synchronized 修饰的，而 StringBuilder并没有，多线程时并不安全但效率相较StringBuffer更高