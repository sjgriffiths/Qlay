@echo off

REM Windows batch file to automatically zip files into release packages

set "rar="C:\Program Files\WinRAR\winrar.exe""

%rar% a -afzip x86-Qlay.zip Qlay\Qlay.h
%rar% a -afzip x86-Qlay.zip Release\Qlay.dll
%rar% a -afzip x86-Qlay.zip Release\QlayCLI.dll

%rar% a -afzip x64-Qlay.zip Qlay\Qlay.h
%rar% a -afzip x64-Qlay.zip x64\Release\Qlay.dll
%rar% a -afzip x64-Qlay.zip x64\Release\QlayCLI.dll

%rar% a -afzip x86-QlayVisual.zip QlayVisual\bin\x86\Release\QlayVisual.exe
%rar% a -afzip x86-QlayVisual.zip QlayVisual\bin\x86\Release\Qlay.dll
%rar% a -afzip x86-QlayVisual.zip QlayVisual\bin\x86\Release\QlayCLI.dll

%rar% a -afzip x64-QlayVisual.zip QlayVisual\bin\x64\Release\QlayVisual.exe
%rar% a -afzip x64-QlayVisual.zip QlayVisual\bin\x64\Release\Qlay.dll
%rar% a -afzip x64-QlayVisual.zip QlayVisual\bin\x64\Release\QlayCLI.dll
