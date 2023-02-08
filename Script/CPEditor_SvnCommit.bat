@echo off

rem 前面加 %WORKSPACE%\Editor\DFM 或者是 K:\CPEd\DFM 或者是 J:\TrunkEd\DFM
rem 全文本将 http://tc-svn.tencent.com/TIMIJ3/ProjectD_proj/trunk/DFMEditorExt/DFM / 删除掉
rem 见 CPEditor_SvnCommit.bat 脚本

if "%BK_CI_PIPELINE_ID%" equ "" (
	set FORDEBUGGING=True
) else (
	set FORDEBUGGING=False
)
echo now is debugging: %FORDEBUGGING%

if %FORDEBUGGING% equ False (
	set PREFIX=%WORKSPACE%\Editor\DFM
) else (
	set PREFIX=J:\TrunkEd\DFM
	rem 我的调试目录
)

rem 往这里面添加同步路径，分批次提交避免提交失败【同时维护parents和list和WHITE_PATH_BISYNC和LARGER_FOLDERS】
set list=Config ^
Content/GenTexDirHD/HDContent ^
Content/GenTexDirHD/Game/Battlefield ^
Content/GenTexDirHD/Game/Environment/Biome ^
Content/GenTexDirHD/Game/Environment/Gameplay ^
Content/GenTexDirHD/Game/Environment/Props ^
Content/GenTexDirHD/Game/Environment/Structures ^
Content/GenTexDirHD/Game/Environment ^
Content/GenTexDirHD/Game/Models ^
Content/GenTexDirHD/Game/Maps/BC_HD ^
Content/GenTexDirHD ^
Content/Environment/Biome ^
Content/Battlefield ^
Content/Environment ^
Content/GenTexDir ^
Content/Maps/BC_HD ^
Content/Maps/BuildMaps ^
Content/Maps ^
Content

rem 父目录强制Add --- 

set parents=Content/GenTexDirHD ^
Content/GenTexDirHD/HDContent ^
Content/GenTexDirHD/Game ^
Content/GenTexDirHD/Game/Battlefield ^
Content/GenTexDirHD/Game/Models ^
Content/Battlefield


for %%a in ( %parents% ) do (
	if exist %PREFIX%\%%a (
		svn add %PREFIX%\%%a --force
	) else (
		echo not existing %PREFIX%\%%a
	)
)

rem 添加并提交各个目录里的内容

if %FORDEBUGGING% equ False (
	cd %WORKSPACE%\DFMEditor\DFM\Content
	for /f %%i in ('svn info --show-item revision') do set REV=%%i

	echo going to update with using theirs
	svn up --accept theirs-full
)

for %%a in ( %list% ) do ( 
	if exist %PREFIX%\%%a (
		echo commiting %PREFIX%\%%a 
		rem cd /d %PREFIX%\%%a rem for循环里无法cd，所以可以放弃
		rem echo %CD%
		
		if %FORDEBUGGING% equ False (
			svn add %PREFIX%\%%a --force
			svn ci %PREFIX%\%%a -m "同步主干：r%REV%, %CD%"
		)
		
	) else (
		echo not existing %PREFIX%\%%a
	)
)
