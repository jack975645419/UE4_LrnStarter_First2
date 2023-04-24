
echo begin force sync
if exist "output.log" (
    del output.log
)

python %~dp0\ForceSync.py "%CD%\DFM\Content"
python %~dp0\ForceSync.py "%CD%\DFM\Plugins"

echo end force sync
pause