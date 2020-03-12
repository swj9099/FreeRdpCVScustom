 echo 'begin end' > %~dp0/begin-flag
 cscript.exe %~dp0/vm1/windows-check.vbs
 echo 'check end' > %~dp0/end-flag
