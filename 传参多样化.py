# -*- coding: utf-8 -*-

'''脚本说明： 
这个脚本尝试进行参数多样化

1. 需要改写 PLATFORM 参数
2. CLUSTER_CONFIG 的格式是： [{"NAMES": ["集群机器名1", "集群机器名2"], "PATHS": ["路径名1","路径名2"]}, {同左 第一个是iOS第二个是安卓}]

'''

IOS = 0
AND = 1
PLATFORM = IOS

CLUSTER_CONFIG = ${{CLUSTER_CONFIG}}
# 例如： [ {"NAMES": ["MAC构建机2号","引擎组-CloudMac1"],"PATHS": ["/Users/devops/workspace/EngineDev","/Users/bkdevops/Documents/agent"]}, {"NAMES": ["云构建机1号","引擎组-CloudWin1"], "PATHS": ["D:/DFM/Engine_Build", "F:/DFM/Engine_Build"]}]


if PLATFORM == IOS:
    INDEX = ${{CTRLIOS_NODE_INDEX}}
    PREFIX = "CTRLIOS"
else:
    INDEX = ${{CTRLAND_NODE_INDEX}}
    PREFIX = "CTRLAND"


# 极端情况
if len(CLUSTER_CONFIG) <= PLATFORM:
    print ("CLUSTER_CONFIG invalid")
    raise "error"

name = CLUSTER_CONFIG[PLATFORM]["NAMES"][INDEX]
path = CLUSTER_CONFIG[PLATFORM]["PATHS"][INDEX]
print ("setEnv {}_NAME {}".format(PREFIX, name))
print ("setEnv {}_PATH {}".format(PREFIX, path))

