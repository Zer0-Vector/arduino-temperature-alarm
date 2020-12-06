@SETLOCAL
@SET LIB_NAME=SerialKeypad
@SET DEST=%OneDrive%\Documents\Arduino\libraries\%LIB_NAME%\
@MKDIR %DEST%
COPY %~dp0%LIB_NAME%.* %DEST%
@ENDLOCAL