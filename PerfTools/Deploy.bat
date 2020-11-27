@SETLOCAL
@SET LIB_NAME=PerfTools
@SET DEST=%OneDrive%\Documents\Arduino\libraries\%LIB_NAME%\
@MKDIR %DEST%
COPY %LIB_NAME%.* %DEST%
COPY keywords.txt %DEST%
COPY readme.md %DEST%
@ENDLOCAL