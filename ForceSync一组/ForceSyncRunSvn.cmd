@echo off

if "%1" == "RevertFile" (
    echo Revert_ %2
    svn revert %2
) else if "%1" == "RevertFolder" (
    echo Revert_folder %2
    svn revert "%2" --depth infinity
) else if "%1" == "Stat" (
    svn stat "%2"
) else if "%1" == "Update" (
    svn up "%2" --accept theirs-full
) else if "%1" == "CleanUnversioned" (
    svn cleanup "%2" --remove-unversioned
) else if "%1" == "JustClean" (
    svn cleanup "%2"
) else if "%1" == "Resolve" (
    svn resolve "%2" --accept theirs-full
) else if "%1" == "Resolved" (
    svn resolved "%2"
)
