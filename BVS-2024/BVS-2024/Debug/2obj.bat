call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x86
ml /c /Fomain.obj out.asm
link main.obj /SUBSYSTEM:CONSOLE
pause

