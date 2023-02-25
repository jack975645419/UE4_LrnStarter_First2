# -*- coding: utf-8 -*-

'''脚本说明： 形成蓝盾流程的自定义互斥锁
调整 MUTEX_OP 表示该脚本的所处的位置

暂时不考虑写冲突的事情
后续调试好了之后，把print功能去掉，避免影响机器性能
特殊规则见 def custom_usable_condition(num):

【单元测试说明】
修改并使用 hg10_transform_run.cmd 生成批量的py脚本
使用 hg16_run.cmd 来运行，观察效果：
1. 全部都能正常退出，脚本不会报错
2. Mutex_testing.json 最终会归零
3. PyRunLock_testing.json 最终会归零

4. 修改好了该脚本后，要记得在 【工具_调查构建机磁盘空间】 线里做检验。

最终：要保管好这个脚本文件

'''
import os, json, time, random

BK_CI_BUILD_NUM     = ${{BUILD_NUM}}          # 构建号 （-1表示重置json）
BK_CI_PIPELINE_ID   = "${{PIPELINE_ID}}"        # 流水线
NODES               = "${{NODES}}"              # 机器名称列表用 逗号隔开
DELAY_START         = ${{DELAY_START}}          # 延迟启动，默认是0，单位是s
TOLERANCE           = ${{TOLERANCE}}          # 容忍时长，单位是min，默认是4320，表示3天，在sleep时触发，如果超过了容忍时长，将会取消本次的操作
PREFER_MASK_STR         = "${{PREFER_MASK_STR}}"        # 偏好，例如是 1,1,1,0,0 表示只使用前面提供的三台机器，如果没有提供，那么用1来补齐

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

if PREFER_MASK_STR == "":
    PREFER_MASK_STR = "1"
PREFER_MASK_LIST = PREFER_MASK_STR.split(",")
while len(PREFER_MASK_LIST) < NODES_NUM:
    PREFER_MASK_LIST.append("1")
PREFER_MASK_LIST = [int(each) for each in PREFER_MASK_LIST]

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
    st = time.strftime("%m-%d %H:%M:%S", time.localtime(time.time())) + " >> \n" + st
    print (st)
    LOGS.append(st)

def sleeps(num):
    flush_log()
    print (f"going to sleep {num}")
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

# 特殊条件：
# 加入特殊条件：Engine_Bake_3090_02 在夜间（21:00~10:00）才允许运行

def custom_usable_condition(nodeName):
    if nodeName == "Engine_Bake_3090_02" and (BK_CI_PIPELINE_ID == "Baking" or BK_CI_PIPELINE_ID == "BakingPC"):
        curHour = time.localtime().tm_hour
        return curHour >= 21 or curHour <= 10
    return True

# 返回 -1 表示用不了
def occupy(num)->int:
    while len(OBJ["using"]) < NODES_NUM:
        OBJ["using"].append(0)
    if len(OBJ["using"]) > NODES_NUM:
        OBJ["using"] = OBJ["using"][0:NODES_NUM]

    validHandle = -1

    for va in range(NODES_NUM):

        if PREFER_MASK_LIST[va] == 1 and OBJ["using"][va] == 0:
            if not custom_usable_condition(NODES_LIST[va]):
                logs (f"special condition not satisfied: {NODES_LIST[va]} {va}")
                continue
            validHandle = va
            break

    if validHandle == -1:
        return -1

    OBJ["using"][validHandle] = num
    return validHandle

def change_sleep_time():
    global SLEEP_TIME # 经验：如果要改变全局变量，那么就声明global
    # 最小等待中
    if minimal_waiting() == BK_CI_BUILD_NUM:
        SLEEP_TIME = 30
        logs (f"set sleep time as {SLEEP_TIME} cause it is minimal waiting {BK_CI_BUILD_NUM} = minimal_waiting {minimal_waiting()} and waiting = {str(OBJ['waiting'])}")
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
    sleeps(SLEEP_TIME)

# 是否可写，基于一个前提：写PY_MUTEX_FILE后2s后如果还是同样的值，意味着争取写的机会成功了
def writable():
    content = read_file_plain(PY_MUTEX_FILE)
    if content == "0" or content == str(BK_CI_BUILD_NUM):
        write_file(PY_MUTEX_FILE, str(BK_CI_BUILD_NUM))
        sleeps(2)
        content = read_file_plain(PY_MUTEX_FILE)
        return content == str(BK_CI_BUILD_NUM)
    else:
        logs ("not writable cause being used by: " + content)
        return False

def release_writable():
    content = read_file_plain(PY_MUTEX_FILE)
    if content != str(BK_CI_BUILD_NUM):
        raise f"not valid {content} {BK_CI_BUILD_NUM}"
    write_file(PY_MUTEX_FILE, "0")

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
            sleeps(SLEEP_TIME)

def force_write_file(filepath):
    content = json.dumps(OBJ)
    while True:
        try:
            write_file(filepath, content)
            logs("writes: " + content)
            return
        except:
            logs ("write_file_error " + content + " and retry again ")
            sleeps(SLEEP_TIME)

def try_to_write(filepath) -> bool:
    content = json.dumps(OBJ)
    try:
        write_file(filepath, content)
        logs("writes: " + content)
        return True
    except:
        logs ("write_file_error " + content + " and retry again ")
        sleeps(SLEEP_TIME)
        return False

def runnable():
    if read_file_plain(PY_MUTEX_FILE) == str(BK_CI_BUILD_NUM):
        time.sleep("")

def unlock_it_and_exit(with_exception:bool = False):
    global OBJ
    while True:
        if not writable():
            sleeps(5) #只是争取写的权限，可以短暂睡眠
            continue

        try:
            content = read_file_plain(FILE_NAME)
            logs ("going to unlock original content: " + content)
            OBJ = json.loads(content)
            remove_waiting(BK_CI_BUILD_NUM)
            remove_using(BK_CI_BUILD_NUM)
            if not try_to_write(FILE_NAME): # 没写成功将会重新走unlock流程
                release_writable()
                continue
        except:
            logs ("read fails (80)")
            release_writable()
            sleeps(SLEEP_TIME)
            continue

        release_writable()

        if with_exception:
            raise "time is up!!"

        exit_script() #【退出】

def time_to_up():
    return TOLERANCE > 0 and time.time() - PY_START_TIME > TOLERANCE * 60

# 程序开始
logs (f"INFO: \nBK_CI_BUILD_NUM = {BK_CI_BUILD_NUM}\nBK_CI_PIPELINE_ID = {BK_CI_PIPELINE_ID}\nNODES = {NODES}\nDELAY_START = {DELAY_START}\nMUTEX_OP = {MUTEX_OP}\nNODES_NUM = {NODES_NUM}\nLOG_NAME = {LOG_NAME}\nPREFER_LIST = {PREFER_MASK_LIST}\n ")

# Python脚本互斥锁 的初始化
if not os.path.exists(PY_MUTEX_FILE):
    write_file(PY_MUTEX_FILE, "0")

# 延迟启动
if DELAY_START > 0:
    logs("DelayStart: " + str(DELAY_START))
    sleeps(DELAY_START)

PY_START_TIME = time.time()

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
    # 如果是-1则还有一个操作需要处理，那就是将PyRun锁给删除掉。
    counter = 1
    while True:
        if os.path.exists(PY_MUTEX_FILE):
            logs(f"cause Build_Num is -1, remove PyMutexFile (No. {counter}) and sleep 10s ")
            counter += 1
            reset_file()
            os.remove(PY_MUTEX_FILE)
            sleeps(10)
            continue
        
        exit_script()

if MUTEX_OP == _LOCK:
    while True:
        if time_to_up():
            print ("time is up!")
            unlock_it_and_exit(with_exception=True)

        if not writable():
            sleeps(5) #只是争取写的权限，可以短暂睡眠
            continue
        
        # 重新读一次
        OBJ = json.loads(read_file_plain(FILE_NAME))
        print ("@ " + str(OBJ))

        # 如果曾经使用过，那么直接放行
        usingHandle = get_using_handle(BK_CI_BUILD_NUM)
        if usingHandle != -1:
            logs("using before, so go ahead!")
            logs ("setEnv NODE_INDEX {}".format(usingHandle))
            logs("setEnv NODE_NAME {}".format(NODES_LIST[usingHandle]))
            release_writable()
            exit_script()

        # 加入等待
        if not is_waiting(BK_CI_BUILD_NUM):
            wait(BK_CI_BUILD_NUM)
            if not try_to_write(FILE_NAME): # 没写成功将会重新考虑lock流程
                release_writable()
                continue 
        
        # 排队（waiting）：筛选出waiting最小者考虑占位。排队后立即短休眠，能有效避免冲突，只有最小者才考虑占位，否则都睡觉就好
        if minimal_waiting() != BK_CI_BUILD_NUM:
            release_writable()
            sleeps_with_changing_time()
            continue
        
        # 占位（using）：看using有没有空缺，有空缺就上，否则睡觉
        handle = occupy(BK_CI_BUILD_NUM)
        if handle == -1: # 占位失败，因为没有using空闲
            release_writable()
            sleeps_with_changing_time()
            continue
        # else: # 占位成功
        remove_waiting(BK_CI_BUILD_NUM)
        if not try_to_write(FILE_NAME): # “功亏一篑”，没写成功将会重新考虑lock流程
            continue
        logs (f"handle: {handle}")
        logs (NODES_LIST)
        logs (f"occupy successfully {handle} {NODES_LIST[handle]}")
        logs ("setEnv NODE_INDEX {}".format(handle))
        logs("setEnv NODE_NAME {}".format(NODES_LIST[handle]))
        release_writable()
        exit_script() #【退出】

elif MUTEX_OP == _UNLOCK:
    unlock_it_and_exit()
else:
    raise "invalid"

