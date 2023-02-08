import os, re 

#lines 版本：
def read_file_lines(filepath):
    outf = open(filepath, "r", encoding='utf-8', errors='ignore') 
    content = outf.read()
    content = content.split('\n')
    outf.close()
    return content
    
def write_file_lines(filepath, lines):
    outf = open(filepath, "w", encoding='utf-8', errors='ignore') 
    lines = '\n'.join(lines)
    outf.write(lines)
    outf.close()
 
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
    

print ("trying to change svn config ")
SVN_SERVER_FILE = r"C:\Users\Administrator\AppData\Roaming\Subversion\servers"
if os.path.exists(SVN_SERVER_FILE):
    content = read_file_plain(SVN_SERVER_FILE)
    if re.search("^http-timeout = ", content, re.RegexFlag.MULTILINE) == None:
        content = content.replace("[global]", "[global]\nhttp-timeout = 7200")
        print ("change to :\n\n")
        print (content)
        print ("svn server cfg changed as above")
else:
    print ("there is no such file: " + SVN_SERVER_FILE)