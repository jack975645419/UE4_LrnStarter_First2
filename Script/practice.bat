rem 为了避免开头损坏

chcp 65001
echo 中文测试 当前时间：%time: =0% 

set x='StableToCP 672072 |  2023-02-01 08:07:56 +0800 '
echo %x%



pause





rem 字符串没办法保留换行，下面的效果是： set x=a b c d e f h i j
set x=a b c ^
d e f ^
h i j 

rem dir | findstr "70 hel"

for /F "delims='" %%i in ( ' echo aaaaaa ' ) do ( 
    echo finddd %%i
    set result=%%i
)
goto End

sqlite3 .svn\wc.db "select * from nodes where checksum like '%%%1'"
sqlite3 .svn\wc.db "update nodes set presence='not-present' where checksum like '%%%1'"
svn cleanup
svn up

:End