# -*- coding: utf-8 -*-

'''脚本说明： 形成蓝盾流程的自定义互斥锁
调整 MUTEX_OP 表示该脚本的所处的位置

暂时不考虑写冲突的事情
后续调试好了之后，把print功能去掉，避免影响机器性能
'''
import os, json, time, random

BK_CI_BUILD_NUM     = ${{BUILD_NUM}}          # 构建号 （-1表示重置json）
BK_CI_PIPELINE_ID   = "${{PIPELINE_ID}}"        # 流水线
NODES               = "${{NODES}}"              # 机器名称列表用 逗号隔开
DELAY_START         = ${{DELAY_START}}          # 延迟启动，默认是0

_LOCK = 1 # 表示进入的位置
_UNLOCK = 2 #表示退出的位置

MUTEX_OP = _LOCK if "${{LOCK}}" == "true" else _UNLOCK
SLEEP_TIME = 60
NODES_LIST = NODES.split(',')
NODES_NUM = len(NODES_LIST)
LOGS = list()

FILE_NAME = f"Mutex_{BK_CI_PIPELINE_ID}.json"
LOG_NAME = f"{BK_CI_PIPELINE_ID}.{BK_CI_BUILD_NUM}.{'LOCK' if MUTEX_OP == _LOCK else 'UNLOCK'}.log"
OBJ = {"waiting": [], "using": []}
PY_MUTEX_FILE = f"PyRunLock_{BK_CI_PIPELINE_ID}.json"
FILE_PATH = "Lock.txt"

'''
# 为了避免冲突，需要对齐到 BK_CI_BUILD_NUM 时，启动
now = int(time.time())
print ("to avoid conflict, snap to seconds: " + str((BK_CI_BUILD_NUM % 20) * 3))
sleepToSnap = (now - (now % 60) + 60 + (BK_CI_BUILD_NUM % 20) * 3 - now) % 60 
# 为了方便理解，式子没有化简。如果不理解也没关系，它表示的是snap。每两次构建之间的snap间隔是3s，只考虑20次一起排队的情况，最长等待是 60s

time.sleep(sleepToSnap)
'''



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

def clamp(num, minimum, maximum):
    return max(min(num, maximum), minimum)

def flush_log():
    write_file(LOG_NAME, '\n'.join(LOGS))

def logs(st):
    st = str(st)
    print (st)
    LOGS.append(st)

def sleeps(num = SLEEP_TIME):
    flush_log()
    time.sleep(num)


def exit_script():
    logs ("going to exit")
    flush_log()
    exit()

def whether_obj_valid(o: dict):
    return "waiting" in o.keys() and "using" in o.keys()

def add_waiting(num):
    if num not in OBJ["waiting"]:
        logs ("add waiting " + str(num))
        OBJ["waiting"].append(num)
    else:
        logs ("already exists waiting of: " + str(num))

def remove_waiting(num):
    if num in OBJ["waiting"]:
        logs ("remove waiting " + str(num))
        OBJ["waiting"].remove(num)
    else:
        logs ("no need to remove waiting of: " + str(num))

def is_waiting(num):
    return num in OBJ["waiting"]

def wait(num):
    OBJ["waiting"].append(num)

def empty_waiting():
    OBJ["waiting"] = list()

def minimal_waiting():
    return min(OBJ["waiting"])

def empty_using():
    OBJ["using"] = list()

def get_using_handle(num):
    if num in OBJ["using"]:
        return OBJ["using"].index(num)
    return -1

def remove_using(num):
    if num in OBJ["using"]:
        logs ("removing using of " + str(num))
        handle = OBJ["using"].index(num)
        OBJ["using"][handle] = 0
    else:
        logs ("no need to remove using of " + str(num))

# 返回 -1 表示用不了
def occupy(num)->int:
    while len(OBJ["using"]) < NODES_NUM:
        OBJ["using"].append(0)
    if len(OBJ["using"]) > NODES_NUM:
        OBJ["using"] = OBJ["using"][0:NODES_NUM]

    if 0 not in OBJ["using"]:
        return -1
    validHandle = OBJ["using"].index(0)
    OBJ["using"][validHandle] = num
    return validHandle

def change_sleep_time():
    # 最小等待中
    if minimal_waiting() == BK_CI_BUILD_NUM:
        SLEEP_TIME = 60
        print (f"set sleep time as {SLEEP_TIME} cause it is minimal waiting {BK_CI_BUILD_NUM}")
        return
    # 其它情况
    num = len(OBJ["waiting"]) + len(OBJ["using"])
    _1min = 60
    _8mins = 8 * 60
    base = clamp(num * 30, _1min, _8mins)
    offset = random.randrange(-5, 5)
    SLEEP_TIME = base + offset
    print (f"set sleep_time as base {base} = clamp( {num} * 30, 1min, 8min), and offset = {offset}, finally as {SLEEP_TIME}")

def sleeps_with_changing_time():
    change_sleep_time()
    sleeps()

# 完全重置，没有等待，也没有使用中
def reset_file():
    logs ("going to reset file")
    while True:
        try:
            empty_waiting()
            empty_using()
            force_write_file(FILE_NAME)
            return
        except:
            logs ("reset_file fails")
            sleeps()

def force_write_file(filepath):
    content = json.dumps(OBJ)
    while True:
        try:
            write_file(filepath, content)
            logs("writes: " + content)
            return
        except:
            logs ("write_file_error " + content + " and retry again ")
            sleeps()

def runnable():
    if read_file_plain(PY_MUTEX_FILE) == str(BK_CI_BUILD_NUM):
        time.sleep("")

# Python脚本互斥锁 的初始化
if not os.path.exists(PY_MUTEX_FILE):
    write_file(PY_MUTEX_FILE, "0")


# 延迟启动
if DELAY_START > 0:
    logs("DelayStart: " + str(DELAY_START))
    sleeps(DELAY_START)


# 存入初始文件
if BK_CI_BUILD_NUM == -1:
    needReset = True
elif not os.path.exists(FILE_NAME):
    needReset = True
else:
    # 预先确认结构
    content = None
    try:
        content = read_file_plain(FILE_NAME)    
        logs ("original content: " + content)
        OBJ = json.loads(content)
        needReset = not whether_obj_valid(OBJ)
    except:
        logs ("error content: " + content)
        # 强制进行初始化
        needReset = True

if needReset:
    reset_file()
if BK_CI_BUILD_NUM == -1:
    exit_script()

if MUTEX_OP == _LOCK:
    while True:
        # 重新读一次
        OBJ = json.loads(read_file_plain(FILE_NAME))
        
        # 如果曾经使用过，那么直接放行
        usingHandle = get_using_handle(BK_CI_BUILD_NUM)
        if usingHandle != -1:
            logs("using before, so go ahead!")
            logs ("setEnv NODE_INDEX {}".format(usingHandle))
            logs("setEnv NODE_NAME {}".format(NODES_LIST[usingHandle]))
            exit_script()

        # 加入等待
        if not is_waiting(BK_CI_BUILD_NUM):
            wait(BK_CI_BUILD_NUM)
            force_write_file(FILE_NAME)
        
        # 排队（waiting），排队后立即短休眠，能有效避免冲突
        if minimal_waiting() != BK_CI_BUILD_NUM:
            sleeps(2)
            continue
        
        # 占位（using）
        handle = occupy(BK_CI_BUILD_NUM)
        if handle == -1: # 占位失败
            sleeps_with_changing_time()
        else: # 占位成功
            remove_waiting(BK_CI_BUILD_NUM)
            force_write_file(FILE_NAME)
            logs (f"handle: {handle}")
            logs (NODES_LIST)
            logs (f"occupy successfully {handle} {NODES_LIST[handle]}")
            logs ("setEnv NODE_INDEX {}".format(handle))
            logs("setEnv NODE_NAME {}".format(NODES_LIST[handle]))
            exit_script() #【退出】

elif MUTEX_OP == _UNLOCK:
    while True:
        try:
            content = read_file_plain(FILE_NAME)
            logs ("going to unlock original content: " + content)
            OBJ = json.loads(content)
            remove_waiting(BK_CI_BUILD_NUM)
            remove_using(BK_CI_BUILD_NUM)
            force_write_file(FILE_NAME)
        except:
            logs ("read fails (80)")
            sleeps()
            continue
        exit_script() #【退出】
else:
    raise "invalid"

