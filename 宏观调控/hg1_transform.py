# -*- coding: utf-8 -*-

'''脚本说明： 
'''

import os, sys

# str版本：
def read_file_plain(filepath):
    outf = open(filepath, "r", encoding='utf-8', errors='ignore') 
    content = outf.read()
    outf.close()
    return content
    
def write_file(filepath, content):
    outf = open(filepath, "w", encoding='utf-8', errors='ignore')
    outf.write(content)
    outf.close()

content = read_file_plain("宏观调控.py")

content = content.replace("${{BUILD_NUM}}", sys.argv[1])
content = content.replace("${{PIPELINE_ID}}", sys.argv[2])
content = content.replace("${{NODES}}", sys.argv[3])
content = content.replace("${{DELAY_START}}", sys.argv[4])
content = content.replace("${{LOCK}}", sys.argv[5])
content = content.replace("${{PREFER_MASK_STR}}", sys.argv[6])

write_file(sys.argv[7], content)

