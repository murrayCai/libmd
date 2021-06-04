next lines should be a simple link
<http://www.baidu.com/>
<https://www.baidu.com/>
<ftp://www.baidu.com/>

next lines should **not be** a simple link
< http://www.baidu.com/>
<http://www.baidu.com/ >
<http://www.baidu.com />
<http://www.baidu.com   />
<www.baidu.com>
</1.php>
<2.php>


next lines should be a **link text**
[text](http://www.baidu.com)
[text](http://www.baidu.com   )
[text](   http://www.baidu.com   )
[ text](   http://www.baidu.com   )
[ text ](   http://www.baidu.com   )
[ te xt ](   http://www.baidu.com   )
[ t e xt ](   http://www.baidu.com   )
[text](http://www.baidu.com "title")
[text](http://www.baidu.com "ti tle")
[text](http://www.baidu.com "ti tl e")
[text](http://www.baidu.com "ti tl}) e")

next lines should not be a ***link text***
[text](http://www. baidu.com)
[text](http://www.baidu.com "ss)
[text](http://www.baidu.com "ss" 1)
[text]](http://www.baidu.com "ss")

next lines should be a *link image*
![img](http://www.baidu.com)
![img](http://www.baidu.com   )
![img](   http://www.baidu.com   )
![ img](   http://www.baidu.com   )
![ img ](   http://www.baidu.com   )
![ im g ](   http://www.baidu.com   )
![ i m g ](   http://www.baidu.com   )
![img](http://www.baidu.com "title")
![img](http://www.baidu.com "ti tle")
![img](http://www.baidu.com "ti tl e")
![img](http://www.baidu.com "ti tl}) e")
