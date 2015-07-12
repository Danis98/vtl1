@echo off
SET force_recompile=false

:PARSE
SET var=%1

IF "%var%"=="" GOTO END_PARSE
IF "%var%"=="-debug" SET compile_args=%compile_args% %var%
IF "%var%"=="-print-ast" SET compile_args=%compile_args% %var%
IF "%var%"=="-f" SET force_recompile=true
IF "%var%"=="--force" SET force_recompile=true

SHIFT

GOTO PARSE
:END_PARSE

IF NOT EXIST vtl.exe CALL build.cmd
IF %force_recompile%==true CALL build.cmd

FOR %%F IN (examples\*.vtl) DO (
	vtl.exe %%F %compile_args%
)

SET compile_args=
SET force_recompile=