import shutil, os, subprocess
import time
print (time.localtime().tm_hour)

'''

_from = r"K:\CPEd\common_env"
_to = r"J:\trunked\common_env"


def exec_cmd(cmd,cwd = None):
    try:
        result = subprocess.check_output(cmd,cwd = cwd, shell= True, stderr = subprocess.STDOUT) # 不要忽略错误 , errors="ignore"

        if not isinstance(result, str):
            result = str(result)
        print (f"watching exec_cmd: {str(cmd)} result:\n{result}")

        # 这个result里的内容没有 红字error！这是非常棘手的
        # result = result.replace("\r\n","\n")
        # result = result.split("\n")
        return result
    except subprocess.CalledProcessError as e: 
        print(e)
    return "error"

output = exec_cmd(f"Robocopy /E /NP /NFL /NDL /MIR {_from} {_to}")
print (output)

print ('FINISHED')'''