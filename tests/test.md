=============== TITLE TEST ============
# title1 111111111111
## title2
### title3
 #### title4
  ##### title5
  ###### title6
## title2 *font italic*111
### title3 **font bold**2222
#### title4 ***font bold italic***3333

================ FONT ===================
title2 *font italic*111
title3 **font bold**2222
title4 ***font bold italic***3333
/*should italic*/
/*should be italic**/
/*should be italic***/
/**should be italic*/
/**should be bold**/
/**should be bold***/
/***should be italic*/
/***should be bold**/
/***should be bold italic***/

================ SPLIT ===================
next lines should be split line
---
-----
- - -
- - - -
- -  -   -   -
-- -- - -  -
   -  - -
***
* * * *
* ** ** ***
   * * * *** **
___
 _ _ _ _
  __ __ __
next lines <should not be split line
--
- -
---1
-    -
1-----

================ LINK ===================
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
![img](https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg)
![img](https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg   )
![img](   https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg   )
![ img](   https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg   )
![ img ](   https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg   )
![ im g ](   https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg   )
![ i m g ](   https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg   )
![img](https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg "title")
![img](https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg "ti tle")
![img](https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg "ti tl e")
![img](https://img-nos.yiyouliao.com/inforec-20210603-4a2786143b70f7db230df6515642d4e7.jpg "ti tl}) e")

================ CODE ===================
/** test code **/
```eg1
regular 3 slash prefix
```
````eg2
4 slash prefix
````
`````eg3
4 slash prefix
`````
    eg4 start with 4 space
    11111

   ```eg5
CODE WITH 3 SPACE PREFIX
   ```

   ```eg6
prefix space len > suffix space len
```````
   ```eg7
   prefix space len == suffix space len
   ```````
   ```eg8
   prefix space len < suffix space len
    ``````` should not be suffix
   ```

```eg9
prefix tag len < suffix tag len
``````
``````eg10
prefix tag len == suffix tag len
``````
``````eg11
prefix tag len > suffix tag len
`````should not be suffix
``````
``````eg12
<next empty line should be line code

``````

```PHP
<?php
    phpinfo();
```

================ TABLE ===================
next lines should be a 4x4 table only headers
should|be|table|header
-|-|-|-

next lines should be a 4x4 table
i-should|b*e|ta*ble|h-/eader
-|-|-|-
should|be|table|row
be|table|row
should|be|table|row|but|this|content|is|not|visiable

next lines should not be table
less|col|table|header
-|-|-|-|-
should|not|be|table|row

next lines should not be table
-|-|-|-
because|no|table|header



================ REF ===================
>ref1
   >ref1
>>ref2
>>>ref3
>>>>ref4
>>>>>ref5
>>>>>>ref6
>>>>>>>ref7
>>>>>>>>ref8
>>>>>>>>>ref9
>>>>>>>>>>ref10
>>>>>>>>>>>ref11
>>>>>>>>>>>>ref12
>>>>>>>>>>>>>ref13
>>>>>>>>>>>>>>ref14
>>>>>>>>>>>>>>>ref15
>>>>>>>>>>>>>>>>ref16

>>>>>>> >>ref9
> >>>>>> >>>ref10
>>  >>>>>>> >>ref11
>>>> >>>>>>>>ref12
>>>>>>>     >>>>>>ref13
>>>>>>>>>>>>  >>ref14

