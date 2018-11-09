@echo off

REM Windows batch file to automatically zip files into release packages

set "rar="C:\Program Files\WinRAR\winrar.exe" a -ibck -ep1 -afzip"

%rar% x86-Qlay.zip Qlay\Qlay.h
%rar% x86-Qlay.zip Release\Qlay.dll
%rar% x86-Qlay.zip Release\Qlay.lib
%rar% x86-Qlay.zip Release\QlayCLI.dll

%rar% x64-Qlay.zip Qlay\Qlay.h
%rar% x64-Qlay.zip x64\Release\Qlay.dll
%rar% x64-Qlay.zip x64\Release\Qlay.lib
%rar% x64-Qlay.zip x64\Release\QlayCLI.dll

%rar% x86-QlayVisual.zip QlayVisual\bin\x86\Release\QlayVisual.exe
%rar% x86-QlayVisual.zip QlayVisual\bin\x86\Release\Qlay.dll
%rar% x86-QlayVisual.zip QlayVisual\bin\x86\Release\QlayCLI.dll
%rar% x86-QlayVisual.zip QlayVisual\bin\x86\Release\QLAYVISUAL.chm

%rar% x64-QlayVisual.zip QlayVisual\bin\x64\Release\QlayVisual.exe
%rar% x64-QlayVisual.zip QlayVisual\bin\x64\Release\Qlay.dll
%rar% x64-QlayVisual.zip QlayVisual\bin\x64\Release\QlayCLI.dll
%rar% x64-QlayVisual.zip QlayVisual\bin\x64\Release\QLAYVISUAL.chm
