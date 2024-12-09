@echo off
setlocal enableDelayedExpansion

REM Check if argument is provided
if "%~1"=="" GOTO help
if "%~1"=="help" GOTO help
if "%~1"=="-h" GOTO help
if "%~1"=="--help" GOTO help

GOTO afterhelp
help:
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
    GOTO git_specific
  )
)
if "%~2"=="git" (
  set "gitparam=1"
)
REM Set the input argument to a variable

REM Add 1 to the number
set /a nxtDay=%1

set strAdd=0_template

set temp_str=Day%strAdd%
set temp_str_h=%temp_str%.h
set temp_str_cpp=%temp_str%.cpp

set nxtDay_strsp=Day %nxtDay%
set nxtDay_str=Day%nxtDay%
set nxtDay_str_h=%nxtDay_str%.h
set nxtDay_str_cpp=%nxtDay_str%.cpp

REM create filelist and copy directory
set fileList=%temp_str_cpp% %temp_str_h% Makefile
REM check that the respective directories and files exist and are empty/contain the necessary files
if exist ".\%nxtDay_strsp%\" (
  dir /b /s /a ".\%nxtDay_strsp%\" | findstr .>nul && (
    echo Folder "%nxtDay_strsp%" is not empty
    choice /C YN /M Continue?
    set choice_level=!errorlevel!
    if !choice_level!==2 (
      echo Canceled batch file execution
      exit /b REM chose n
    )
  )
) else (
  echo "%nxtDay_strsp%" does not exist
  exit /b
)

for %%f in (%filelist%) do xcopy .\%%f .\"%nxtDay_strsp%" >> temp.log

REM cd to new directory, rename files
echo "%~dp0"
echo "%nxtDay_strsp%"
echo "%~dp0""%nxtDay_strsp%"
echo %~dp0 %nxtDay_strsp%
echo %~dp0%nxtDay_strsp%

cd %~dp0%nxtDay_strsp%
exit /b
ren %temp_str%.* %nxtDay_str%.*

REM REM file modification - specifically change the MAIN variable in Makefile, header in .cpp

REM create sample.txt
copy NUL sample.txt

REM only run the following code if I am in a VSCode terminal
REM close all vscode files (Ctrl+K, Ctrl+W), open all non-directory files in vscode
setlocal
REM Check if the TERM_PROGRAM environment variable is set to 'vscode'
if "%TERM_PROGRAM%"=="vscode" (
  "%~dp0\closeVSCgroup.ahk"
  for %%f in (*) do (
    if not "%%~af"=="d" (
      code %%f
    ) >> temp.log
  )
  code %nxtDay_str_cpp%
) else (
  echo This terminal is not running in Visual Studio Code.
)
endlocal

REM add git commit, depending on command line input
:git
if defined gitparam (
  git diff --cached --quiet
  if !ERRORLEVEL! NEQ 0 (
    git add .
    git commit -m "%nxtDay_strsp% Scaffolding"
  ) else (
    echo Did not add or commit to git: There are currently staged changes.
  )
)

REM Exit the script
exit /b
  
:git_specific
set /a nxtDay=%1
set nxtDay_strsp=Day %nxtDay%
@cd "%~dp0%nxtDay_strsp%"

GOTO git
