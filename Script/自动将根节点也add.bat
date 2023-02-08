rem 为了避免开头损坏
@echo off
chcp 65001
echo 中文测试 当前时间：%time: =0% 

rem 危险的脚本：

set x=J:\TrunkEd\DFM\Others\Tools\a\b\c
set 

(svn info %x% | findstr not found) & (
    set flag=1

)
set x=%x%\..
svn info %x%
