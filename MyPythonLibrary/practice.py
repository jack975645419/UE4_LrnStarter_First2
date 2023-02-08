# -*- coding: utf-8 -*-

'''脚本说明： 
测试脚本
'''

import os, shutil

print(os.path.exists(r"K:\CPEd\DFM\Content\../Config"))
shutil.copyfile(r"E:\CI\cpe\SvnPrb\..\SvnPrb\x.log", r"E:\CI\cpe\SvnPrb\..\SvnPrb2\x.log")
open(r"E:\CI\cpe\SvnPrb\..\SvnPrb2\xa.log", "a") # 无中生有，产生一个0Kb文件

exit()
for subdir, dirs, files in os.walk(os.path.abspath(os.path.join(r"K:\CPEd\DFM\Content", "../Config"))):
    print (str(files))
    print (str(dirs))
    print (str(subdir))

print ("finished")