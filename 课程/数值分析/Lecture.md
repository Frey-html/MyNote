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


###  CH6 Direct method for linear system
#### 6.1 高斯消元法
*  高斯消元法
	* 化为上三角矩阵，逐步向上回代
	step 1: 如果$a_{11}$不等于0， $m_{i1}=a_{i1}^{(1)}/a_{11}^{(1)}$, i = 2 到 n
		增广矩阵$row_i$减去$m_{i1}\times row_1$
	step k:如果$a_{kk}^k$不等于零，$m_{ik}=a_{ik}^{(k)}/a_{kk}^{(k)}$, i 从k+1到n
		增广矩阵$row_i$减去$m_{ik}\times row_k$
	共k-1步后，增广矩阵变为上三角阵
	**pivot element:$a_{kk}^{(k)}$**
	
	* Backward-substitution 回代
	$x_n = b_n^{n}/a_{nn}^{(n)}$ $x_i=\frac{b_i^{(i)}-\sum _{j=i+1}^{n}a_{ij}^{(i)}x_j}{a_{ii}^{(i)}}$ i从n-1到1
	
	* 如果$a_{nn} = 0$(对于所有列来说)，则矩阵奇异没有唯一解（该行向量可被表达为其他行的线性组合）；b的向量在A的线性无关形成的空间（range space）时有无穷多个解，否则无解
	* $a_{nn}$等于0时可以向下找不等于0的行，在增广矩阵中换行
	
	* **计算复杂度** （不考虑加减法，只考虑乘除）：$n^3/3$
		* 消元
		消元第k步： n-k个除法 (n-k)(n-k+1)个乘法: $(n-k)(n-k+2)$
		共计：$\sum _{k=1}^{n-1}(n-k)(n-k+2) = \frac {n^3}{3}+\frac {n^2}{2}-\frac {5}{6}n$
		 * 回代
		 $1 + \sum _{i=1}^{n-1}{n-i+1}=\frac {n^2}{2}+\frac {n}{2}$
		

#### 6.2 Pivoting Strategy
* Pivoting Strategy
	问题：small pivot element 可能会引起大误差
	* **Complete Pivoting**
		* 需要额外的$O(n^3/3)$次比较
		从$a_{kk}^{(k)}$右下角找到一个比它大的交换，需要行列变换，比较次数多，列交换需要记录x的交换，计算完后换回来
	* **Partial Pivoting(maximal column pivoting)**
		* 需要额外的$O(n^2)$次比较
		选择能使$a_{kk}^{(k)}$最大的行与当前行交换，只需行变换
	* **Scaled Partial Pivoting**
		* 需要额外的$O(n^2)$次比较和$O(n^2)$次除法
		比较的是$a_{ik}^{(k)}$与（i>k）除以当前行最大的系数![[SharedScreenshot 6.jpg|400]] s可以只在一开始计算一次
	**有效数字：** 由尾数位确定，多余的0可以由指数位表示
		当有效数字有限时，如果pivot较小，则算出m较大，m乘上当前行的系数也较大，相减消元时容易出现大数吃小数

#### 6.5 Matrix Factorization 
* Matrix Factorization 矩阵分解
	* Matrix Form of Gaussian Elimination
	用矩阵运算的形式表示高斯消元法![[课程/数值分析/附件/SharedScreenshot.jpg|400]]![[课程/数值分析/附件/SharedScreenshot 1.jpg|400]]
	理解：$L_k^{-1}$矩阵为把第k行乘以相应系数加到它下面的行中，逐次相乘得到下三角矩阵![[SharedScreenshot 5.jpg|400]]
	高斯消元法：$L^{-1}A=U$得到$A=LU$
	将矩阵分解为单位下三角阵和单位上三角阵
	**为什么需要LU分解** ： A确定b不定时可以快速计算 对于任意一个向量b只需要计算Ly = b和 Ux = y ：LUx = b 化成了两次back substitution，降低复杂度
	[Doolittle分解法原理](https://zhuanlan.zhihu.com/p/115689677)
	[Doolittle分解步骤（LU分解）](https://blog.csdn.net/lol_ip/article/details/78491457)
	[主子式、顺序主子式、余子式、代数余子式](https://blog.csdn.net/yskyskyer123/article/details/87891051)
	

* **Theorem: 如果Ax = b可以使用高斯消元法且不经过行交换（也即顺序主子式不为0），那么A可以进行LU分解成下三角阵L和上三角阵U**
	**如果L是unitary（对角线全为一），则分解唯一**
	* 唯一性：反证法 
		$A=L_1U_1=L_2U_2$
		$U_1U_2^{-1}=L_1^{-1}L_2U_2U_2^{-1}=L_1^{-1}L_2=I$
		不需要行交换就可以进行高斯消元法说明矩阵非奇异，可逆
		而$L_1^{-1}L_2$是主对角线全是1的下三角阵，$U_1U_2^{-1}$为上三角阵，两者相等，故为单位阵，即LU分解唯一
	* ==Crout Factorazation：U的主对角线为1（方法为转置再LU分解再转置）==

#### 6.6 Special types  of matrix
* **Strictly diagonally dominant matrix 严格对角占优阵**
	* [严格对角占优矩阵 - 腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1459631)
	* 主对角线元素绝对值大于该行其他列元素绝对值之和
		性质：非奇异 高斯消元法不用换行换列 计算时stable舍入误差小
		**非奇异性证明**：反证
		[严格对角占优矩阵非奇异 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/512029879)
		高斯消元不用换行换列：步骤中的矩阵也是严格对角占优

* **Choleski method for positive definite matrix 正定阵(对称)**
	* 线代知识补充
		*[线性代数之——正定矩阵](https://zhuanlan.zhihu.com/p/93392382)
		[怎么解释「正定矩阵」](https://www.zhihu.com/question/304499772)
		[如何求矩阵的特征值和特征向量](https://blog.csdn.net/keshuaiChen/article/details/109371314)
		[矩阵中的主元:在消去过程中起主导作用的元素](https://blog.csdn.net/deniece1/article/details/101982463)
		[行列式的计算方法](https://blog.csdn.net/weixin_46664967/article/details/113621821)
		
		**特征向量不过就是线性变换中空间中的方向不变的向量罢了，变换了多少就把它叫特征值**
	
	* 教材：p401-p402 关于正定阵
		* 一般要求对称且$x^TAx>0$，但有些教材只要求后者，称对称的正定阵为对称正定阵
		* 排除非正定阵的条件：![[SharedScreenshot 7.jpg|400]]
		* 一个矩阵是正定阵当且仅当它所有主子矩阵是正定的
	* **正定阵的cholesky分解**
		* LU分解![[SharedScreenshot 8.jpg|400]]
		[矩阵分解 LDL^T分解](https://blog.csdn.net/billbliss/article/details/78560605)
		* 进一步变化：cholesky分解![[SharedScreenshot 9.jpg|400]]
		计算比LU分解更简便，只需计算一个下三角阵，通过转置得到上三角阵
		* 计算步骤![[SharedScreenshot 10.jpg|400]]
		
* **Tridiagonal linear system 三对角阵**
	

