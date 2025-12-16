@echo off
echo Building WordCloud Generator

if exist build rmdir /S /Q build
mkdir build
cd build

REM ЗАМЕНИТЬ ПУТЬ ПРИ НЕОБХОДИМОСТИ
set QT=E:\Qt\6.5.3\mingw_64

cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT%/lib/cmake"
mingw32-make

echo Copying Qt libs
copy "%QT%\bin\Qt6Core.dll" .
copy "%QT%\bin\Qt6Gui.dll" .
copy "%QT%\bin\libgcc_s_seh-1.dll" .
copy "%QT%\bin\libstdc++-6.dll" .
copy "%QT%\bin\libwinpthread-1.dll" .

mkdir platforms
mkdir imageformats
copy "%QT%\plugins\platforms\qwindows.dll" platforms\
copy "%QT%\plugins\imageformats\qjpeg.dll" imageformats\

echo Build successful
pause