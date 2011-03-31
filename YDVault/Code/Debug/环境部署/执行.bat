@echo off
start /wait vcredist_x86.exe
start /wait Y-105Driver.exe
start /wait MSCHART.exe 
start /wait ../bin/YDVaultMain.exe&exit