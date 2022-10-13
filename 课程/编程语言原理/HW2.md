**归纳定义证明**
* 给定判断$max(m;n;p)$的一个归纳定义，其中$m~nat$, $n~nat$, $p~nat$，且$p$是$m$和$n$中的较大者。证明，通过这个判断，每个$m$和$n$都与唯一的$p$相关。

给出归纳定义:
1. $\frac{a~nat}{max(zero;a;a)}$
3. $\frac{max(a;b;b)}{max(a;succ(b);succ(b))}$

要证每个m和n都与唯一的p相关
即：$\forall a~nat,b~nat,p_1~nat, p_2~nat:nat~max(a;b;p_1)\wedge max(a;b;p_2)\rightarrow p_1~is~p_2$
等价于 $\forall a~nat,b~nat,p_1~nat, :nat~max(a;b;p_1)\rightarrow \forall p_2~nat max(a;b;p_2)\rightarrow p_1~is~p_2$








