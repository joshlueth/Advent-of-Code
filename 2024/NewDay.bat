@echo off
set "nxtDay_strsp="
setlocal enableDelayedExpansion

REM Check if argument is provided
if "%~1"=="" GOTO help
if "%~1"=="help" GOTO help
if "%~1"=="-h" GOTO help
if "%~1"=="--help" GOTO help

GOTO afterhelp
:help
  echo Please provide an integer argument to copy from that day to the next day,
  echo or provide "git" to add and commit the current directory.
  echo Preappend "git" to commit the specific day directory.
  exit /b

:afterhelp
set "gitparam="
if "%~1"=="git" (
  set "gitparam=1"
  shift
  if not "%~1"=="" (
    GOTO git
  ) else (
    set /a nxtDay=%1
    set nxtDay_strsp=Day !nxtDay!
    cd "%~dp0!nxtDay_strsp!"
    setlocal
    GOTO git
  )
)
if "%~2"=="git" (
  set "gitparam=1"
)
REM Set the input argument to a variable

REM Add 1 to the number
set /a nxtDay=%1

set strAdd=0_template

set temp_str=Day!strAdd!
set temp_str_h=!temp_str!.h
set temp_str_cpp=!temp_str!.cpp

set nxtDay_strsp=Day !nxtDay!
set nxtDay_str=Day!nxtDay!
set nxtDay_str_h=!nxtDay_str!.h
set nxtDay_str_cpp=!nxtDay_str!.cpp

REM create filelist and copy directory
set fileList=!temp_str_cpp! !temp_str_h! Makefile
REM check that the respective directories and files exist and are empty/contain the necessary files
if not exist ".\!nxtDay_strsp!\" (
  choice /C YN /M Directory does not exist: Create directory?
  set choice_level=!errorlevel!
  if !choice+level!==2 (
    echo Canceled batch file execution
    exit /b REM chose n
  )
  mkdir ".\!nxDay_strsp!\"
)
dir /b /s /a ".\!nxtDay_strsp!\" | findstr .>nul && (
  echo Folder "!nxtDay_strsp!" is not empty
  choice /C YN /M Continue?
  set choice_level=!errorlevel!
  if !choice_level!==2 (
    echo Canceled batch file execution
    exit /b REM chose n
  )
)

for %%f in (!filelist!) do xcopy .\%%f .\"!nxtDay_strsp!" >> NUL

REM cd to new directory, rename files

cd "%~dp0!nxtDay_strsp!"

ren !temp_str!.* !nxtDay_str!.*

REM file modification - specifically change the MAIN variable in Makefile, header in .cpp
call powershell -ExecutionPolicy Bypass -File "%~dp0find_replace_aocTmplt.ps1" -file_mod "%~dp0!nxtDay_strsp!\Makefile" -replace_txt Day0_template -with_txt !nxtDay_str!
call powershell -ExecutionPolicy Bypass -File "%~dp0find_replace_aocTmplt.ps1" -file_mod "%~dp0!nxtDay_strsp!\!nxtDay_str_cpp!" -replace_txt Day0_template -with_txt !nxtDay_str!

REM create sample.txt
copy NUL sample.txt >> NUL

REM only run the following code if I am in a VSCode terminal
REM close all vscode files (Ctrl+K, Ctrl+W), open all non-directory files in vscode

REM Check if the TERM_PROGRAM environment variable is set to 'vscode'
if "%TERM_PROGRAM%"=="vscode" (
  call "%~dp0\closeVSCgroup.ahk"
  for %%f in (*) do (
    if not "%%~af"=="d" (
      start "" /B cmd /C code %%f
    )
  )
) else (
  echo This terminal is not running in Visual Studio Code.
)

REM add git commit, depending on command line input
:git
if not defined gitparam (
  goto end_git
)
git diff --cached --quiet --exit-code
if %ERRORLEVEL%==0 (
  git add "%CD%" >> "%~dp0temp.log"
  git commit -m "!nxtDay_strsp! Scaffolding" >> "%~dp0temp.log"
) else (
  echo Did not add or commit to git: There are currently staged changes.
)
:end_git

REM change directories to the desired directory if variables are defined
if not defined nxtDay_strsp (
  goto :end
)

:end_cd
endlocal & ( set nxtDay_strsp=%nxtDay_strsp% )
cd "%~dp0%nxtDay_strsp%"
REM Exit the script
exit /b

:end
endlocal
exit /b

