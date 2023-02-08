import os


# 从指定path下递归获取所有文件
def getAllFile(path, fileList):
    dirList = []  # 存放文件夹路径的列表
    for ff in os.listdir(path):
        wholepath = os.path.join(path, ff)
        # wholepath = path+'/'+ff
        if os.path.isdir(wholepath):
            dirList.append(wholepath)  # 如果是文件添加到结果文件列表中

        if os.path.isfile(wholepath):
            fileList.append(wholepath)  # 如果是文件夹，存到文件夹列表中

    for dir in dirList:
        getAllFile(dir, fileList)  # 对于dirList列表中的文件夹，递归提取其中的文件，fileList一直在往下传，所有的文件路径都会被保存在这个列表中

def get_FileSize(filePath):
    #filePath = unicode(filePath,'utf8')
    fsize = os.path.getsize(filePath)
    fsize = fsize/float(1024)
    return round(fsize,3)

# 从文件路径列表中筛选出指定后缀的文件
# 这里可以从源列表中删除 非后缀 项，也可以新建一个 后缀列表，遍历源列表向后缀列表中.append()后缀项
# 后者更容易一些，我这里选择了前者，还费劲解决了一波list循环remove的问题。。。
# list循环remove http://itdarcy.wang/index.php/20200109/381
def getSufFilePath(fileList, oldDict, suffix):
    # print(len(fileList))
    for ff in fileList[:]:
        if not ff.endswith(suffix):
            fileList.remove(ff)
        else:
            oldDict[os.path.basename(ff)] = get_FileSize((ff))

if __name__ == '__main__':
    flist = []
    oldDict = {}
    newDict= {}
    findpath = r"D:\devops_workspace\CompareAB\CompareAB\Download\DolphinOld\dolphin"
    getAllFile(findpath, flist)
    print('allfile:', len(flist))  # filepath下的文件总数
    getSufFilePath(flist,oldDict, '.unity3d')

    flist1 = []
    findpath1 = r"D:\devops_workspace\CompareAB\CompareAB\Download\DolphinNew\dolphin"
    getAllFile(findpath1, flist1)

    sameCnt = 0
    changeCnt = 0
    addCnt = 0
    deleteCnt = 0
    sameSize = 0
    oldSize = 0
    newSize = 0
    for abNew in flist1:
        abNewBase = os.path.basename(abNew)
        newDict[abNewBase] = get_FileSize((abNew))
        newSize = newDict[abNewBase] + newSize
        if(abNewBase in oldDict.keys()) and get_FileSize(abNew) == oldDict[abNewBase]:
            print(abNewBase+"[Same]")
            sameCnt = sameCnt + 1
            sameSize = sameSize + oldDict[abNewBase]
        elif(abNewBase in oldDict.keys() and get_FileSize(abNew) != oldDict[abNewBase]):
            print(abNewBase + "[Changed]")
            changeCnt = changeCnt + 1
        elif(abNewBase not in oldDict.keys()):
            print(abNewBase + "[Added]")
            addCnt = addCnt + 1
    for abOld in oldDict:
        oldSize = oldDict[abOld] + oldSize
        if(abOld not in newDict):
            print(abOld + "[Deleted]")
            deleteCnt = deleteCnt + 1

    print('oldAB:', len(flist),'  oldSize:',round(oldSize/float(1024),2),'MB')
    print('newAB:', len(flist1),'  newSize:',round(newSize/float(1024),2),'MB')
    print('sameAB:',sameCnt,'  sameSize:',round(sameSize/float(1024),2),'MB')
    print('changeAB:',changeCnt)
    print('addAB:',addCnt)
    print('deleteAB:',deleteCnt)