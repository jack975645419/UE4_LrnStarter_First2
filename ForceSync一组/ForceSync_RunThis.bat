
echo begin force sync
if exist "output.log" (
    del output.log
)

python ForceSync.py "%CD%\DFM\Content"
python ForceSync.py "%CD%\DFM\Plugins"

echo end force sync
pause