@del temp.txt
@cls
for %%i in (*.txt) do cl /EHsc %%~ni.cpp ..\..\src\ymfm_adpcm.cpp ..\..\src\ymfm_opn.cpp ..\..\src\ymfm_ssg.cpp || goto :eof
del *.obj
@cls
echo Running tests:
for %%i in (*.txt) do @call :runone %%~ni.exe %%~ni.txt
@goto :eof

:runone
@echo.
@echo %1
@%1 > temp.txt
@fc /w /n temp.txt %2
@del temp.txt
@goto :eof
