**HW: p.54 #13, 15; p.64 #3, p.65 #19**
___
**Q13**
*$b-a=3$
有 $2^n\geq10^4\times3$
所以 $n\geq14$ $p_{14}=1.3247$
___
**Q15**
对于任意一项$p_n$，有$p_n\geq\int_1^n\frac{1}{x}dx$
而$lim_{n\rightarrow\infty}\int_1^n\frac{1}{x}dx=\int_1^{\infty}\frac{1}{x}=lnx|^{\infty}_1$
显然不收敛
___
**Q3**
即求$p_0$和根$p=21^{\frac{1}{3}}$附近倒数，若g(x)绝对值小于1，则导数越小迭代越快
a : $g'(x)=\frac{20}{21}-\frac{2}{x^3}$ 而$g'(p_0)<-1$不收敛
b : $g'(x)=\frac{2}{3}-\frac{42}{3x^2}$ $g'(p_0)<-1$ 不收敛
c : $-1<g'(1)<0$ 收敛
d ：不收敛
___
**Q19**
**a**:
$g(x)=0.5x+\frac{1}{x}$ $g'(x)=0.5-\frac{1}{x^2}$
$x>\sqrt{2}$ 时 $0<g'(x)<0.5$
且有$g(\sqrt2)=\sqrt2$ $x>\sqrt{2}$ 时$g(x)<x$
则收敛，又有$\sqrt2$为根，则其为唯一根，即$x>\sqrt{2}$收敛到$\sqrt2$
**b:**
$x_1=\frac{x_0^2+2}{2x_0}$ 则有$x_1-\sqrt2=\frac{x_0^2-2\sqrt2x_0+2}{2x_0}$
$x_1-\sqrt2=\frac{(x_0-\sqrt2)^2}{2x_0}>0$
$x_1>\sqrt2$
**c:**
由a知$x_0>\sqrt{2}$时收敛到$\sqrt2$
又有$0<x_0<\sqrt2$时$x_1>\sqrt2$，则序列从$x_1$开始也收敛
$x_0=\sqrt2$时已经收敛
综上$x_0>0$时收敛到$\sqrt2$