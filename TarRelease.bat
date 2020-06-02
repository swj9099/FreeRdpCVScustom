::自动整理文件，免去每次出包拷贝libssl相关库以及删除静态库操作
@echo off
set target_path=D:\FreerdpPackage\Release
set source_path=D:\Company\Freerdp\FreeRDP-2.0.0-rc4\Release
set libssl_path=D:\Company\Freerdp\FreeRDP-2.0.0-rc4\libssl
echo 'begin tar'
mkdir %target_path%
xcopy %source_path% %target_path% /S /Y
xcopy %libssl_path% %target_path% /S /Y
cd /d %target_path%
del *.lib
echo 'tar end'
