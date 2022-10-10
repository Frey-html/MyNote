**1.**
(1)
	![[1.1.drawio.png]]
(2)
![[1.2.drawio.png|350]]
___
**2.**
(1) z w
(2) x y
(3) z
___
**3.**
(1) 
原式：$\lambda v.(\lambda x.\lambda y.z)(\lambda z. z~v~w)$
局部无名表达法：$\lambda 0.(\lambda .\lambda .z)(\lambda .0~1~w)$

(2)
原式：$x~(\lambda x.x)~x~(\lambda x.y)$
局部无名表达法：$x~(\lambda .0)~x~(\lambda .y)$

(3)
原式：$\lambda y.(\lambda x.x~y~z)~y~y$
局部无名表达法：$\lambda .(\lambda .0~1~z)~0~0$

(4)
局部无名表达法：$\lambda .0(\lambda .1(\lambda .2~0~w))$
一般表达法：$\lambda x.x(\lambda y.x(\lambda z.x~z~w))$

(5)
局部无名表达法：$\lambda .(\lambda .1(0~0))(\lambda .1(0~0))$
一般表达法：$\lambda x.(\lambda y.x(y~y))(\lambda z.x(z~z))$
___
**4.**
(1)
原式：$\lambda y.\lambda z.(\lambda x.\lambda y.x)~y~z$
正规形式：$\lambda y.\lambda z.\lambda t.yz$

(2)
原式：$(\lambda p.p~x~y)(\lambda x.\lambda y.x)$
正规形式：$x$

(3)
原式：$(\lambda x.x~x)(\lambda y.\lambda z.y~z)$
正规形式：$\lambda t.\lambda z.t~z$
