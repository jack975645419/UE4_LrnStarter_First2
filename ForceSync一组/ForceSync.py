# -*- coding: utf-8 -*-


'''脚本说明： 
尝试做更加复杂的冲突解决工具

搜索 DEBUG_PATH 可以填写调试用的路径，并在VSCode中直接运行。

'''

print ("如果报错win32api找不到，那么先运行 pip install pywin32")
import os, socket, subprocess, win32api, sys
from subprocess import Popen, PIPE, STDOUT

#lines 版本：
def read_file_lines(filepath):
	outf = open(filepath, "r", encoding='GB2312', errors='ignore') 
	content = outf.read()
	content = content.split('\n')
	outf.close()
	return content
    
def write_file_lines(filepath, lines):
	outf = open(filepath, "w", encoding='GB2312', errors='ignore') 
	lines = '\n'.join(lines)
	outf.write(lines)
	outf.close()
 
# str版本：
def read_file_plain(filepath):
    outf = open(filepath, "r", encoding='GB2312', errors='ignore') 
    content = outf.read()
    outf.close()
    return content
    
def write_file(filepath, content):
    outf = open(filepath, "w", encoding='GB2312', errors='ignore')
    outf.write(content)
    outf.close()

''' def [cmd:str, path:str):
    return f'{cmd} {path}' '''

def executes(cmd:list): # 返回类型 ->list, int
    _p = Popen(cmd, stdout=PIPE, stderr=STDOUT)
    _lines = _p.stdout.readlines()

    _lines = [each.decode("GB2312").strip() for each in _lines]
    _p.wait()
    return _lines, _p.returncode

# 第一个值是类型，第二个值是路径
def splits(line: str):
    return line[:8], line[8:]

def reverts(paths: list):
    print (f"[HoldingReverting]")
    output.append(f"[HoldingReverting]")
    counter = 0
    lines = None
    for path in paths:
        if counter % 10 == 0:
            print (f"... {counter} / {len(paths)}")
        counter += 1

        # 即使absent也要尝试revert，避免D C 的问题
        ''' if not os.path.exists(path):
            output.append(f"[Absent] {path}")
            continue '''
        output.append(f"[Reverting] {path}")
        if os.path.isdir(path):
            lines, rcode = executes(["ForceSyncRunSvn.cmd", "RevertFolder", path])
        else:
            lines, rcode = executes(["ForceSyncRunSvn.cmd", "RevertFile", path])
        output.append(f"\t[ReturnCode] {rcode}")
        for i in lines:
            output.append(i)

def resolves(paths: list):
    print (f"[HoldingResolving]")
    output.append(f"[HoldingResolving]")
    counter = 0
    for path in paths:
        if counter % 10 == 0:
            print (f"... {counter} / {len(paths)}")
        counter += 1
        
        if not os.path.exists(path):
            output.append(f"[Absent] {path}")
            continue
        output.append(f"[Resolving] {path}")
        lines, rcode = executes(["ForceSyncRunSvn.cmd", "Resolve" , path])
        output.append(f"\t[ReturnCode] {rcode}")
        for i in lines:
            output.append(i)

def resolved(paths: list):
    print (f"[HoldingResolved]")
    output.append(f"[HoldingResolved]")
    counter = 0
    for path in paths:
        if counter % 10 == 0:
            print (f"... {counter} / {len(paths)}")
        counter += 1
        
        if not os.path.exists(path):
            output.append(f"[Absent] {path}")
            continue
        output.append(f"[Resolved] {path}")
        lines, rcode = executes(["ForceSyncRunSvn.cmd", "Resolved", path])
        output.append(f"\t[ReturnCode] {rcode}")
        for i in lines:
            output.append(i)


def unversions(path: str):
    if not os.path.exists(path):
        output.append(f"[Absent] {path}")
        return
    print(f"[CleanUnversioned] {path}")
    output.append(f"[CleanUnversioned] {path}")

    lines, rcode = executes(["ForceSyncRunSvn.cmd", "CleanUnversioned", workspace])
    output.append(f"\t[ReturnCode] {rcode}")
    for i in lines:
        output.append(i)

def update(path: str):
    if not os.path.exists(path):
        output.append(f"[Absent] {path}")
        return
    print (f"[Update] {path}")
    output.append(f"[Update] {path}")
    lines, rcode = executes(["ForceSyncRunSvn.cmd", "Update", path])
    output.append(f"\t[ReturnCode] {rcode}")
    for i in lines:
        output.append(i)

def just_clean(path: str):
    if not os.path.exists(path):
        output.append(f"[Absent] {path}")
        return
    print (f"[JustClean] {path}")
    output.append(f"[JustClean] {path}")
    lines, rcode = executes(["ForceSyncRunSvn.cmd", "JustClean", path])
    output.append(f"\t[ReturnCode] {rcode}")
    for i in lines:
        output.append(i)

if len(sys.argv) <= 1:
    print ("please input a path")
    #raise ("arv none")
    workspace = r"C:\CFMobile\Client\Assets\Models" # DEBUG_PATH
else:
    workspace = sys.argv[1]


output = list()
just_clean(workspace)

lines, rcode = executes(["ForceSyncRunSvn.cmd", "Stat", workspace])
to_reverts = list()
to_resolves = list()
to_be_resolved = list()
# 输出原文：
print ("starting")
counter = 0
REVERT_TYPES = ["A  +  C " , "M  +    " , "M       " , "A  +    " , "D     C " , "!     C " , "?     C "  , "R  +  C "]
for each in lines:
    typee, path = splits(each)
    output.append(f"[Type] {typee} [Path] {path}")
    
    # 仅仅处理：
    if typee in REVERT_TYPES:
        to_reverts.append(path)
        output.append(f"[Reverts] {path}")
    if typee == "C       ":
        to_resolves.append(path)
        output.append(f"[Conflicts] {path}")

    if counter % 10 == 0:
        print (f"... {counter} / {len(lines)}")
    counter += 1

    # 都忽略    
    ''' 
    if : # 是指错误地被删除
        to_be_resolved.append(path)
        output.append(f"[Resovled] {path}") 
           if typee == "M  +    ":
            print ("ignore too")
        if typee == ">   loca":
            print ("ignore")
        if typee == "?       ":
            print ("to ignore")
        if typee == "M       ":
            print ("ignore too")
    Summary 
    Text con '''


reverts(to_reverts)
resolves(to_resolves)
resolved(to_be_resolved)
unversions(workspace)
reverts([workspace])

lines, rcode = executes(["ForceSyncRunSvn.cmd", "Stat", workspace])
print ("immediately print")
for each in lines:
    print (each)

update(workspace)
output.append("\n\n") # 追加换行

if os.path.isfile("output.log"):
    appendable = read_file_lines("output.log")
    appendable += output
    output = appendable

write_file_lines("output.log", output)

print("finished. please go to check output.log")

