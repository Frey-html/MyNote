### Intro
* 计算机基本运算是加减乘除，复杂的运算需要采用逼近的方式

### CH1 Mathmatical Preliminaries
* 误差：
	* Truncation:递推式无穷时取有限项带来的误差
	* Roundoff：计算机浮点数精度有限所带来的误差(两种方法：chopping（直接舍去k位之后）和rounding（k+1位加5向下取整）)
* 相对误差$|p-p^*|$ 绝对误差$\frac{|p-p^*|}{|p|}$
* t 位significant digits 有效位数：
	* $\frac{|p-p^*|}{|p|}<5\times10^{-t}$
* 采用k位表达，chopping时相对误差最大$10^{-k+1}$ rounding时$5\times10^{-k}$
	roudding相对误差更小，可以满足k位有效位数
	
* roundoff如何影响到结果
	* 如$0.234\times10^1+1\times10^{-7}$ ==加法需要先对齐==，但是对齐后已经超过了可以表达的有效位数（大数吃小数，但相对误差影响不大）
	* 两个相近的数相减，导致有效位数变少
	* 大数除小数（已经有误差），放大误差
	* ==计算时要考虑公式是否合理==

#### 1.3 Algorithm and convergence
* stable：初值的微小改变导致最终结果的改变也微小
* C > 1，如果初始误差是$E_0$，n次迭代后是$E_n$,如果$E_n\approx CnE_0$
	则为线性，如果是指数关系则为指数级

