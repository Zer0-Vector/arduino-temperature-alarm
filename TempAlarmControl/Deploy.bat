@SETLOCAL
@SET LIB_NAME=TempAlarmControl
@SET DEST=%OneDrive%\Documents\Arduino\libraries\%LIB_NAME%\
@MKDIR %DEST%
COPY *.* %DEST%
@REM COPY keywords.txt %DEST%
@REM COPY readme.md %DEST%
@ENDLOCAL