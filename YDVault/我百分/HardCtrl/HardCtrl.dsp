# Microsoft Developer Studio Project File - Name="HardCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HardCtrl - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HardCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HardCtrl.mak" CFG="HardCtrl - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HardCtrl - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /stack:0x989680 /subsystem:windows /incremental:yes /debug /machine:I386 /out:"..\Release\HardCtrl.exe"
# Begin Target

# Name "HardCtrl - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Ado.cpp
# End Source File
# Begin Source File

SOURCE=.\AdoCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\AdoRecordSet.cpp
# End Source File
# Begin Source File

SOURCE=.\BackUp.cpp
# End Source File
# Begin Source File

SOURCE=.\Base.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpDat.cpp
# End Source File
# Begin Source File

SOURCE=.\cdxCSizingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Control.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportToExcel.cpp
# End Source File
# Begin Source File

SOURCE=.\font1.cpp
# End Source File
# Begin Source File

SOURCE=.\HardCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HardCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\ListCtrlPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuEx.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart1.cpp
# End Source File
# Begin Source File

SOURCE=.\PreGoto.cpp
# End Source File
# Begin Source File

SOURCE=.\PreParent.cpp
# End Source File
# Begin Source File

SOURCE=.\PreView.cpp
# End Source File
# Begin Source File

SOURCE=.\PrinterJob.cpp
# End Source File
# Begin Source File

SOURCE=.\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\shockwaveflash.cpp
# End Source File
# Begin Source File

SOURCE=.\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SoftStat.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XPButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Ado.h
# End Source File
# Begin Source File

SOURCE=.\AdoCommand.h
# End Source File
# Begin Source File

SOURCE=.\AdoRecordSet.h
# End Source File
# Begin Source File

SOURCE=.\BackUp.h
# End Source File
# Begin Source File

SOURCE=.\Base.h
# End Source File
# Begin Source File

SOURCE=.\BmpDat.h
# End Source File
# Begin Source File

SOURCE=.\cdxCSizingDialog.h
# End Source File
# Begin Source File

SOURCE=.\Control.h
# End Source File
# Begin Source File

SOURCE=.\CoolTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ExportToExcel.h
# End Source File
# Begin Source File

SOURCE=.\font1.h
# End Source File
# Begin Source File

SOURCE=.\HardCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlPrint.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MenuEx.h
# End Source File
# Begin Source File

SOURCE=.\mschart1.h
# End Source File
# Begin Source File

SOURCE=.\PreGoto.h
# End Source File
# Begin Source File

SOURCE=.\PreParent.h
# End Source File
# Begin Source File

SOURCE=.\PreView.h
# End Source File
# Begin Source File

SOURCE=.\PrinterJob.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\shockwaveflash.h
# End Source File
# Begin Source File

SOURCE=.\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\SkinMagicLib.h
# End Source File
# Begin Source File

SOURCE=.\SoftStat.h
# End Source File
# Begin Source File

SOURCE=.\SstUser.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XPButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ICO\ico\01.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\10.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\1005.ICO
# End Source File
# Begin Source File

SOURCE=.\res\11.bmp
# End Source File
# Begin Source File

SOURCE=".\res\1280-800.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\1280-800¸±±¾1.bmp"
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\16link.ico
# End Source File
# Begin Source File

SOURCE=.\res\17.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\281.ICO
# End Source File
# Begin Source File

SOURCE=".\ICO\3.5 floppy drive.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\327.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\40.ico
# End Source File
# Begin Source File

SOURCE=.\res\add_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\add_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addanswer_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addanswer_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addclass_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addclass_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\adddepart_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\adddepart_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addpaper_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\addpaper_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=".\ICO\Application _ Sherlock.ico"
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\arrow first.ico"
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\arrow last.ico"
# End Source File
# Begin Source File

SOURCE=.\res\back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\back1.bmp
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\bar chart-2.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\Bathhaus.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_l.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bn_back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bn_mask.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\BOOK06.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\BRIEF3D_.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\cancel.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\Cancel1.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\CARACOL.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\CD3D.ICO
# End Source File
# Begin Source File

SOURCE=.\res\changeexamtime_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\changeexamtime_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\changesystime_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\changesystime_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\choose_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\choose_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\Church.ico
# End Source File
# Begin Source File

SOURCE=".\ICO\close folder.ico"
# End Source File
# Begin Source File

SOURCE=.\res\closefm_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\closefm_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\cocodrilo.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\CONPANE_.ICO
# End Source File
# Begin Source File

SOURCE=".\ICO\control panel.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\COPY.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\Covered.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\CROSS.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\ctrl.ico
# End Source File
# Begin Source File

SOURCE=.\res\date_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\date_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\DELETE1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\delete_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\delete_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\deleteclass_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\deleteclass_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\deletedepart_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\deletedepart_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\DESKTOP3.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\dialup.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\DISKFILE.ICO
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\doc excel.ico"
# End Source File
# Begin Source File

SOURCE=.\res\downfm_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\downfm_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dragging.cur
# End Source File
# Begin Source File

SOURCE=.\ICO\DSCF0706.ico
# End Source File
# Begin Source File

SOURCE=".\ICO\Electric Co..ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\ELEFANTE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\exam_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exam_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit_bn1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit_bn_down1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit_log_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit_log_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exportchart_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exportchart_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=".\ICO\Family Prac..ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\favicon.ico
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\fill down.ico"
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\fill up.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\formback.bmp
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\full screen.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\Futurehaus1.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\Futurehaus2.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\gaoji.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\go.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\Greenhaus.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\Greenroundhaus.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\hammer.ico
# End Source File
# Begin Source File

SOURCE=.\res\handcur.cur
# End Source File
# Begin Source File

SOURCE=".\ICO\hard drive.ico"
# End Source File
# Begin Source File

SOURCE=.\res\HardCtrl.ico
# End Source File
# Begin Source File

SOURCE=.\res\HardCtrl.rc2
# End Source File
# Begin Source File

SOURCE=.\ICO\help.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\HIPO.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\Historichaus.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\history.ico
# End Source File
# Begin Source File

SOURCE=".\ICO\Ico02 040.ico"
# End Source File
# Begin Source File

SOURCE=".\ICO\Ico02 041.ico"
# End Source File
# Begin Source File

SOURCE=".\ICO\Ico02 066.ico"
# End Source File
# Begin Source File

SOURCE=".\ICO\Icon Lab.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\icon11.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\IE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\inputfm_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inputfm_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\KEY.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\link.ICO
# End Source File
# Begin Source File

SOURCE=.\res\login_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_bn1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_bn_down1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_btn_exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_btn_exit_over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_btn_login.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_btn_login_over.bmp
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\mag glass.ico"
# End Source File
# Begin Source File

SOURCE=.\res\main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menu1.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\Merganser.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\MISC1102.ICO
# End Source File
# Begin Source File

SOURCE=.\res\moon_folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\moon_folder1.ico
# End Source File
# Begin Source File

SOURCE=.\res\moon_gomi.ico
# End Source File
# Begin Source File

SOURCE=.\res\moon_gomi1.ico
# End Source File
# Begin Source File

SOURCE=.\res\moon_ie.ico
# End Source File
# Begin Source File

SOURCE=".\ICO\my computer.ico"
# End Source File
# Begin Source File

SOURCE=.\res\next_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\next_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nodraggi.cur
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\Note_Pad.ico
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\office building.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\ok.ico
# End Source File
# Begin Source File

SOURCE=.\res\ok_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ok_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\opnefm_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\opnefm_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\paperquery_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\paperquery_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\Parrot.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\Pelican.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\plus.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\preview.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\print.ico
# End Source File
# Begin Source File

SOURCE=.\res\print_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\print_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\printchart_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\printchart_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\PRINTER_.ICO
# End Source File
# Begin Source File

SOURCE=.\res\query_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\query_bn1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\query_bn1_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\query_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\radoi.ico
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\repeat.ico
# End Source File
# Begin Source File

SOURCE=.\res\report_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\report_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\roll_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\roll_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\roll_gray.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\rollall.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\rolling.ico
# End Source File
# Begin Source File

SOURCE=.\res\rolling_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rolling_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\rollsingle.ICO
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\save.ico
# End Source File
# Begin Source File

SOURCE=.\res\save_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\save_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\search_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\search_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\search_bn_gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\searchfm_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\searchfm_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\set_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\set_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\showall_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\showall_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\specsh.ICO
# End Source File
# Begin Source File

SOURCE=.\res\static_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\static_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\TICK.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\TRFFC20.ICO
# End Source File
# Begin Source File

SOURCE=.\res\up_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\up_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\up_gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\update_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\update_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\updateclass_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\updateclass_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\updatedepart_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\updatedepart_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\upfm_bn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\upfm_bn_down.bmp
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\users.ico
# End Source File
# Begin Source File

SOURCE=".\ICO\ico\view user.ico"
# End Source File
# Begin Source File

SOURCE=.\ICO\ico\VIEWER1.ICO
# End Source File
# Begin Source File

SOURCE=".\ICO\Yttrium Icon 20.ico"
# End Source File
# Begin Source File

SOURCE=".\res\µÇÂ¼_Down.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\µÇÂ¼_MASK.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\µÇÂ¼_Off.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\µÇÂ¼_On.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ËÑË÷_Down.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ËÑË÷_MASH.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ËÑË÷_Off.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ËÑË÷_On.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ÍË³ö_Down.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ÍË³ö_Off.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\ÍË³ö_On.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ñ¡Ôñ_Down.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ñ¡Ôñ_Off.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Ñ¡Ôñ_On.bmp"
# End Source File
# End Group
# Begin Group "Share"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CeXDib.cpp
# End Source File
# Begin Source File

SOURCE=.\CeXDib.h
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\ControlButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlButton.h
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Picture\JahCoverCapture.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\JahCoverColorListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\JahCoverHyperlink.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\JahCoverListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Frame\JahCoverSystemTray.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\JahCoverToolTipXye.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\JahCoverXPButton.cpp
# End Source File
# Begin Source File

SOURCE=.\JahFile.cpp
# End Source File
# Begin Source File

SOURCE=.\JahFile.h
# End Source File
# Begin Source File

SOURCE=.\jahobject.cpp
# End Source File
# Begin Source File

SOURCE=.\jahobject.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEdit.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEditBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEditBase.h
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlMap.h
# End Source File
# Begin Source File

SOURCE=.\MyList.cpp
# End Source File
# Begin Source File

SOURCE=.\MyList.h
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.h
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\SortHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Ctrl\SortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\videocapx.cpp
# End Source File
# Begin Source File

SOURCE=.\videocapx.h
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Frame\XTMemDC.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\Cover\Frame\XTOutBarCtrl.cpp
# End Source File
# End Group
# Begin Group "Left"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LeftAudit.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftAudit.h
# End Source File
# Begin Source File

SOURCE=.\LeftManage.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftManage.h
# End Source File
# Begin Source File

SOURCE=.\LeftReport.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftReport.h
# End Source File
# End Group
# Begin Group "Form"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FormAnswer.cpp
# End Source File
# Begin Source File

SOURCE=.\FormAnswer.h
# End Source File
# Begin Source File

SOURCE=.\FormClassResult.cpp
# End Source File
# Begin Source File

SOURCE=.\FormClassResult.h
# End Source File
# Begin Source File

SOURCE=.\FormClassResultQuey.cpp
# End Source File
# Begin Source File

SOURCE=.\FormClassResultQuey.h
# End Source File
# Begin Source File

SOURCE=.\FormCreatFile.cpp
# End Source File
# Begin Source File

SOURCE=.\FormCreatFile.h
# End Source File
# Begin Source File

SOURCE=.\FormDepartment.cpp
# End Source File
# Begin Source File

SOURCE=.\FormDepartment.h
# End Source File
# Begin Source File

SOURCE=.\FormExamaddr.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamaddr.h
# End Source File
# Begin Source File

SOURCE=.\FormExamBlue.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamBlue.h
# End Source File
# Begin Source File

SOURCE=.\FormExamDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamDetail.h
# End Source File
# Begin Source File

SOURCE=.\FormExamImport.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamImport.h
# End Source File
# Begin Source File

SOURCE=.\FormExamInfoBack.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamInfoBack.h
# End Source File
# Begin Source File

SOURCE=.\FormExamInfrar.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamInfrar.h
# End Source File
# Begin Source File

SOURCE=.\FormExamInrarOff.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamInrarOff.h
# End Source File
# Begin Source File

SOURCE=.\FormExamMode.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamMode.h
# End Source File
# Begin Source File

SOURCE=.\FormExamRollingAnswer.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamRollingAnswer.h
# End Source File
# Begin Source File

SOURCE=.\FormExamSet.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamSet.h
# End Source File
# Begin Source File

SOURCE=.\FormExamTime.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamTime.h
# End Source File
# Begin Source File

SOURCE=.\FormExamUSB.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExamUSB.h
# End Source File
# Begin Source File

SOURCE=.\FormExm.cpp
# End Source File
# Begin Source File

SOURCE=.\FormExm.h
# End Source File
# Begin Source File

SOURCE=.\FormFM.cpp
# End Source File
# Begin Source File

SOURCE=.\FormFM.h
# End Source File
# Begin Source File

SOURCE=.\FormImage.cpp
# End Source File
# Begin Source File

SOURCE=.\FormImage.h
# End Source File
# Begin Source File

SOURCE=.\FormImport.cpp
# End Source File
# Begin Source File

SOURCE=.\FormImport.h
# End Source File
# Begin Source File

SOURCE=.\FormListen.cpp
# End Source File
# Begin Source File

SOURCE=.\FormListen.h
# End Source File
# Begin Source File

SOURCE=.\FormlLink.cpp
# End Source File
# Begin Source File

SOURCE=.\FormlLink.h
# End Source File
# Begin Source File

SOURCE=.\FormMain.cpp
# End Source File
# Begin Source File

SOURCE=.\FormMain.h
# End Source File
# Begin Source File

SOURCE=.\FormMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\FormMenu.h
# End Source File
# Begin Source File

SOURCE=.\FormPaper.cpp
# End Source File
# Begin Source File

SOURCE=.\FormPaper.h
# End Source File
# Begin Source File

SOURCE=.\FormPaperAnaly.cpp
# End Source File
# Begin Source File

SOURCE=.\FormPaperAnaly.h
# End Source File
# Begin Source File

SOURCE=.\FormPCoffRoll.cpp
# End Source File
# Begin Source File

SOURCE=.\FormPCoffRoll.h
# End Source File
# Begin Source File

SOURCE=.\FormPCoffRolling.cpp
# End Source File
# Begin Source File

SOURCE=.\FormPCoffRolling.h
# End Source File
# Begin Source File

SOURCE=.\FormResult.cpp
# End Source File
# Begin Source File

SOURCE=.\FormResult.h
# End Source File
# Begin Source File

SOURCE=.\FormStuinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FormStuinfo.h
# End Source File
# Begin Source File

SOURCE=.\FormStuinfoSet.cpp
# End Source File
# Begin Source File

SOURCE=.\FormStuinfoSet.h
# End Source File
# Begin Source File

SOURCE=.\FormStuResultQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\FormStuResultQuery.h
# End Source File
# Begin Source File

SOURCE=.\FormSubject.cpp
# End Source File
# Begin Source File

SOURCE=.\FormSubject.h
# End Source File
# Begin Source File

SOURCE=.\FormTeacherDev.cpp
# End Source File
# Begin Source File

SOURCE=.\FormTeacherDev.h
# End Source File
# Begin Source File

SOURCE=.\FromImageread.cpp
# End Source File
# Begin Source File

SOURCE=.\FromImageread.h
# End Source File
# End Group
# Begin Group "dlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgAnswer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAnswer.h
# End Source File
# Begin Source File

SOURCE=.\DlgAnswerset.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAnswerset.h
# End Source File
# Begin Source File

SOURCE=.\DlgChangeExmaTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChangeExmaTime.h
# End Source File
# Begin Source File

SOURCE=.\DlgChangesystime.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChangesystime.h
# End Source File
# Begin Source File

SOURCE=.\DlgClassQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgClassQuery.h
# End Source File
# Begin Source File

SOURCE=.\DlgClassResult.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgClassResult.h
# End Source File
# Begin Source File

SOURCE=.\DlgClassSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgClassSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgDpartment.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDpartment.h
# End Source File
# Begin Source File

SOURCE=.\DlgExamaddr.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExamaddr.h
# End Source File
# Begin Source File

SOURCE=.\DlgExamEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExamEdit.h
# End Source File
# Begin Source File

SOURCE=.\DlgExamset.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExamset.h
# End Source File
# Begin Source File

SOURCE=.\DlgExamstat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExamstat.h
# End Source File
# Begin Source File

SOURCE=.\DlgExmOption.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExmOption.h
# End Source File
# Begin Source File

SOURCE=.\DlgFM.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFM.h
# End Source File
# Begin Source File

SOURCE=.\DlgLate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLate.h
# End Source File
# Begin Source File

SOURCE=.\DlgLinkroll.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLinkroll.h
# End Source File
# Begin Source File

SOURCE=.\DlgLinkrolling.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLinkrolling.h
# End Source File
# Begin Source File

SOURCE=.\DlgLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLogin.h
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPaperAnaly.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPaperAnaly.h
# End Source File
# Begin Source File

SOURCE=.\DlgPaperQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPaperQuery.h
# End Source File
# Begin Source File

SOURCE=.\DlgPaperset.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPaperset.h
# End Source File
# Begin Source File

SOURCE=.\DlgPerolling.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPerolling.h
# End Source File
# Begin Source File

SOURCE=.\DlgPhoto.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPhoto.h
# End Source File
# Begin Source File

SOURCE=.\DlgPreRolling.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPreRolling.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuerystudentresult.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuerystudentresult.h
# End Source File
# Begin Source File

SOURCE=.\DlgResDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgResDetail.h
# End Source File
# Begin Source File

SOURCE=.\DlgRollingChoose.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRollingChoose.h
# End Source File
# Begin Source File

SOURCE=.\DlgSingleroll.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSingleroll.h
# End Source File
# Begin Source File

SOURCE=.\Dlgsperoll.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlgsperoll.h
# End Source File
# Begin Source File

SOURCE=.\DlgStunfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStunfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgStuQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStuQuery.h
# End Source File
# Begin Source File

SOURCE=.\DlgSubject.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSubject.h
# End Source File
# Begin Source File

SOURCE=.\DlgTeacherdev.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTeacherdev.h
# End Source File
# Begin Source File

SOURCE=.\DlgTeacherSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTeacherSet.h
# End Source File
# End Group
# Begin Group "db"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DBClass.cpp
# End Source File
# Begin Source File

SOURCE=.\DBClass.h
# End Source File
# Begin Source File

SOURCE=.\DBDepartment.cpp
# End Source File
# Begin Source File

SOURCE=.\DBDepartment.h
# End Source File
# Begin Source File

SOURCE=.\DBExamaddr.cpp
# End Source File
# Begin Source File

SOURCE=.\DBExamaddr.h
# End Source File
# Begin Source File

SOURCE=.\DBLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\DBLogin.h
# End Source File
# Begin Source File

SOURCE=.\DBPaper.cpp
# End Source File
# Begin Source File

SOURCE=.\DBPaper.h
# End Source File
# Begin Source File

SOURCE=.\DBResult.cpp
# End Source File
# Begin Source File

SOURCE=.\DBResult.h
# End Source File
# Begin Source File

SOURCE=.\DBStdanswer.cpp
# End Source File
# Begin Source File

SOURCE=.\DBStdanswer.h
# End Source File
# Begin Source File

SOURCE=.\DBStuInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DBStuInfo.h
# End Source File
# Begin Source File

SOURCE=.\DBSubject.cpp
# End Source File
# Begin Source File

SOURCE=.\DBSubject.h
# End Source File
# Begin Source File

SOURCE=.\DBTeacherDev.cpp
# End Source File
# Begin Source File

SOURCE=.\DBTeacherDev.h
# End Source File
# Begin Source File

SOURCE=.\RQDb.cpp
# End Source File
# Begin Source File

SOURCE=.\RQDb.h
# End Source File
# Begin Source File

SOURCE=.\RQWhere.cpp
# End Source File
# Begin Source File

SOURCE=.\RQWhere.h
# End Source File
# End Group
# Begin Group "com"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExamApp.cpp
# End Source File
# Begin Source File

SOURCE=.\ExamApp.h
# End Source File
# Begin Source File

SOURCE=.\ExamDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ExamDetail.h
# End Source File
# Begin Source File

SOURCE=.\Stuanswer.cpp
# End Source File
# Begin Source File

SOURCE=.\Stuanswer.h
# End Source File
# Begin Source File

SOURCE=.\Studev.cpp
# End Source File
# Begin Source File

SOURCE=.\Studev.h
# End Source File
# Begin Source File

SOURCE=.\StuInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StuInfo.h
# End Source File
# Begin Source File

SOURCE=.\Stusearch.cpp
# End Source File
# Begin Source File

SOURCE=.\Stusearch.h
# End Source File
# Begin Source File

SOURCE=.\TeacherCom.cpp
# End Source File
# Begin Source File

SOURCE=.\TeacherCom.h
# End Source File
# Begin Source File

SOURCE=.\TeacherDev.cpp
# End Source File
# Begin Source File

SOURCE=.\TeacherDev.h
# End Source File
# Begin Source File

SOURCE=.\YDAppCom.cpp
# End Source File
# Begin Source File

SOURCE=.\YDAppCom.h
# End Source File
# Begin Source File

SOURCE=.\YDCom.cpp
# End Source File
# Begin Source File

SOURCE=.\YDCom.h
# End Source File
# End Group
# Begin Group "garaph"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Answer.cpp
# End Source File
# Begin Source File

SOURCE=.\Answer.h
# End Source File
# Begin Source File

SOURCE=.\APPGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\APPGraph.h
# End Source File
# Begin Source File

SOURCE=.\DlgGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGraph.h
# End Source File
# Begin Source File

SOURCE=.\FileGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\FileGraph.h
# End Source File
# Begin Source File

SOURCE=.\FormGraphData.cpp
# End Source File
# Begin Source File

SOURCE=.\FormGraphData.h
# End Source File
# Begin Source File

SOURCE=.\Graph.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph.h
# End Source File
# Begin Source File

SOURCE=.\GraphDataColor.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphDataColor.h
# End Source File
# Begin Source File

SOURCE=.\GraphDataSet.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphDataSet.h
# End Source File
# Begin Source File

SOURCE=.\GraphLegend.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLegend.h
# End Source File
# Begin Source File

SOURCE=.\GraphLegendSet.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLegendSet.h
# End Source File
# Begin Source File

SOURCE=.\GraphPieLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphPieLabel.h
# End Source File
# Begin Source File

SOURCE=.\GraphSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphSeries.h
# End Source File
# End Group
# Begin Group "mschart"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vcaxis.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxis.h
# End Source File
# Begin Source File

SOURCE=.\vcaxisgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxisgrid.h
# End Source File
# Begin Source File

SOURCE=.\vcaxisscale.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxisscale.h
# End Source File
# Begin Source File

SOURCE=.\vcaxistitle.cpp
# End Source File
# Begin Source File

SOURCE=.\vcaxistitle.h
# End Source File
# Begin Source File

SOURCE=.\vcbackdrop.cpp
# End Source File
# Begin Source File

SOURCE=.\vcbackdrop.h
# End Source File
# Begin Source File

SOURCE=.\vcbrush.cpp
# End Source File
# Begin Source File

SOURCE=.\vcbrush.h
# End Source File
# Begin Source File

SOURCE=.\vccategoryscale.cpp
# End Source File
# Begin Source File

SOURCE=.\vccategoryscale.h
# End Source File
# Begin Source File

SOURCE=.\vccolor.cpp
# End Source File
# Begin Source File

SOURCE=.\vccolor.h
# End Source File
# Begin Source File

SOURCE=.\vccoor.cpp
# End Source File
# Begin Source File

SOURCE=.\vccoor.h
# End Source File
# Begin Source File

SOURCE=.\vcdatagrid.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatagrid.h
# End Source File
# Begin Source File

SOURCE=.\vcdatapoint.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatapoint.h
# End Source File
# Begin Source File

SOURCE=.\vcdatapointlabel.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatapointlabel.h
# End Source File
# Begin Source File

SOURCE=.\vcdatapoints.cpp
# End Source File
# Begin Source File

SOURCE=.\vcdatapoints.h
# End Source File
# Begin Source File

SOURCE=.\vcfill.cpp
# End Source File
# Begin Source File

SOURCE=.\vcfill.h
# End Source File
# Begin Source File

SOURCE=.\vcfont.cpp
# End Source File
# Begin Source File

SOURCE=.\vcfont.h
# End Source File
# Begin Source File

SOURCE=.\vcfootnote.cpp
# End Source File
# Begin Source File

SOURCE=.\vcfootnote.h
# End Source File
# Begin Source File

SOURCE=.\vcframe.cpp
# End Source File
# Begin Source File

SOURCE=.\vcframe.h
# End Source File
# Begin Source File

SOURCE=.\vcintersection.cpp
# End Source File
# Begin Source File

SOURCE=.\vcintersection.h
# End Source File
# Begin Source File

SOURCE=.\vclabel.cpp
# End Source File
# Begin Source File

SOURCE=.\vclabel.h
# End Source File
# Begin Source File

SOURCE=.\vclabels.cpp
# End Source File
# Begin Source File

SOURCE=.\vclabels.h
# End Source File
# Begin Source File

SOURCE=.\vclcoor.cpp
# End Source File
# Begin Source File

SOURCE=.\vclcoor.h
# End Source File
# Begin Source File

SOURCE=.\vclegend.cpp
# End Source File
# Begin Source File

SOURCE=.\vclegend.h
# End Source File
# Begin Source File

SOURCE=.\vclight.cpp
# End Source File
# Begin Source File

SOURCE=.\vclight.h
# End Source File
# Begin Source File

SOURCE=.\vclightsource.cpp
# End Source File
# Begin Source File

SOURCE=.\vclightsource.h
# End Source File
# Begin Source File

SOURCE=.\vclightsources.cpp
# End Source File
# Begin Source File

SOURCE=.\vclightsources.h
# End Source File
# Begin Source File

SOURCE=.\vclocation.cpp
# End Source File
# Begin Source File

SOURCE=.\vclocation.h
# End Source File
# Begin Source File

SOURCE=.\vcmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\vcmarker.h
# End Source File
# Begin Source File

SOURCE=.\vcpen.cpp
# End Source File
# Begin Source File

SOURCE=.\vcpen.h
# End Source File
# Begin Source File

SOURCE=.\vcplot.cpp
# End Source File
# Begin Source File

SOURCE=.\vcplot.h
# End Source File
# Begin Source File

SOURCE=.\vcplotbase.cpp
# End Source File
# Begin Source File

SOURCE=.\vcplotbase.h
# End Source File
# Begin Source File

SOURCE=.\vcrect.cpp
# End Source File
# Begin Source File

SOURCE=.\vcrect.h
# End Source File
# Begin Source File

SOURCE=.\vcseries.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseries.h
# End Source File
# Begin Source File

SOURCE=.\vcseriescollection.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseriescollection.h
# End Source File
# Begin Source File

SOURCE=.\vcseriesmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseriesmarker.h
# End Source File
# Begin Source File

SOURCE=.\vcseriesposition.cpp
# End Source File
# Begin Source File

SOURCE=.\vcseriesposition.h
# End Source File
# Begin Source File

SOURCE=.\vcshadow.cpp
# End Source File
# Begin Source File

SOURCE=.\vcshadow.h
# End Source File
# Begin Source File

SOURCE=.\vcstatline.cpp
# End Source File
# Begin Source File

SOURCE=.\vcstatline.h
# End Source File
# Begin Source File

SOURCE=.\vctextlayout.cpp
# End Source File
# Begin Source File

SOURCE=.\vctextlayout.h
# End Source File
# Begin Source File

SOURCE=.\vctick.cpp
# End Source File
# Begin Source File

SOURCE=.\vctick.h
# End Source File
# Begin Source File

SOURCE=.\vctitle.cpp
# End Source File
# Begin Source File

SOURCE=.\vctitle.h
# End Source File
# Begin Source File

SOURCE=.\vcvaluescale.cpp
# End Source File
# Begin Source File

SOURCE=.\vcvaluescale.h
# End Source File
# Begin Source File

SOURCE=.\vcview3d.cpp
# End Source File
# Begin Source File

SOURCE=.\vcview3d.h
# End Source File
# Begin Source File

SOURCE=.\vcwall.cpp
# End Source File
# Begin Source File

SOURCE=.\vcwall.h
# End Source File
# Begin Source File

SOURCE=.\vcweighting.cpp
# End Source File
# Begin Source File

SOURCE=.\vcweighting.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\SkinMagicTrial.dll
# End Source File
# Begin Source File

SOURCE=.\SkinMagicTrial.lib
# End Source File
# End Target
# End Project
# Section HardCtrl : {E9E07517-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcCategoryScale
# 	2:10:HeaderFile:vccategoryscale1.h
# 	2:8:ImplFile:vccategoryscale1.cpp
# End Section
# Section HardCtrl : {E9E0750E-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataPoint
# 	2:10:HeaderFile:vcdatapoint1.h
# 	2:8:ImplFile:vcdatapoint1.cpp
# End Section
# Section HardCtrl : {E9E07527-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcPlot
# 	2:10:HeaderFile:vcplot1.h
# 	2:8:ImplFile:vcplot1.cpp
# End Section
# Section HardCtrl : {E9E074E6-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFootnote
# 	2:10:HeaderFile:vcfootnote1.h
# 	2:8:ImplFile:vcfootnote1.cpp
# End Section
# Section HardCtrl : {E9E074D6-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLocation
# 	2:10:HeaderFile:vclocation1.h
# 	2:8:ImplFile:vclocation1.cpp
# End Section
# Section HardCtrl : {E9E07502-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeriesPosition
# 	2:10:HeaderFile:vcseriesposition1.h
# 	2:8:ImplFile:vcseriesposition1.cpp
# End Section
# Section HardCtrl : {E9E074CD-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcRect
# 	2:10:HeaderFile:vcrect1.h
# 	2:8:ImplFile:vcrect1.cpp
# End Section
# Section HardCtrl : {3A2B370A-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CMSChart
# 	2:10:HeaderFile:mschart1.h
# 	2:8:ImplFile:mschart1.cpp
# End Section
# Section HardCtrl : {E9E074D1-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcPen
# 	2:10:HeaderFile:vcpen1.h
# 	2:8:ImplFile:vcpen1.cpp
# End Section
# Section HardCtrl : {27F983A6-6963-4024-B18E-5F54628C4D66}
# 	2:21:DefaultSinkHeaderFile:trocxnat.h
# 	2:16:DefaultSinkClass:CTrOcxNat
# End Section
# Section HardCtrl : {E9E07506-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcStatLine
# 	2:10:HeaderFile:vcstatline1.h
# 	2:8:ImplFile:vcstatline1.cpp
# End Section
# Section HardCtrl : {E9E074DC-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcShadow
# 	2:10:HeaderFile:vcshadow1.h
# 	2:8:ImplFile:vcshadow1.cpp
# End Section
# Section HardCtrl : {9790BF9A-C8BE-4A03-B293-FB1CE134D95F}
# 	2:21:DefaultSinkHeaderFile:ctrocxnat.h
# 	2:16:DefaultSinkClass:CCTrOcxNat
# End Section
# Section HardCtrl : {E9E07511-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeries
# 	2:10:HeaderFile:vcseries1.h
# 	2:8:ImplFile:vcseries1.cpp
# End Section
# Section HardCtrl : {E9E07521-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxisScale
# 	2:10:HeaderFile:vcaxisscale1.h
# 	2:8:ImplFile:vcaxisscale1.cpp
# End Section
# Section HardCtrl : {E9E074D5-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcTextLayout
# 	2:10:HeaderFile:vctextlayout1.h
# 	2:8:ImplFile:vctextlayout1.cpp
# End Section
# Section HardCtrl : {41A3E5E4-EEDC-4F8F-89CA-3590E54ECA29}
# 	2:5:Class:CSkyGraphCtrl
# 	2:10:HeaderFile:skygraphctrl.h
# 	2:8:ImplFile:skygraphctrl.cpp
# End Section
# Section HardCtrl : {E9E0751D-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcIntersection
# 	2:10:HeaderFile:vcintersection1.h
# 	2:8:ImplFile:vcintersection1.cpp
# End Section
# Section HardCtrl : {E9E074FC-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcWall
# 	2:10:HeaderFile:vcwall1.h
# 	2:8:ImplFile:vcwall1.cpp
# End Section
# Section HardCtrl : {E9E074EC-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLightSource
# 	2:10:HeaderFile:vclightsource1.h
# 	2:8:ImplFile:vclightsource1.cpp
# End Section
# Section HardCtrl : {E9E074F0-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLight
# 	2:10:HeaderFile:vclight1.h
# 	2:8:ImplFile:vclight1.cpp
# End Section
# Section HardCtrl : {E9E074E0-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFrame
# 	2:10:HeaderFile:vcframe1.h
# 	2:8:ImplFile:vcframe1.cpp
# End Section
# Section HardCtrl : {B95FCCC6-F8C2-46BD-9576-5D1350291E4F}
# 	2:5:Class:CCTrOcxNat
# 	2:10:HeaderFile:ctrocxnat.h
# 	2:8:ImplFile:ctrocxnat.cpp
# End Section
# Section HardCtrl : {C8873EA2-31A5-48D9-9A6C-3AD077DF592E}
# 	2:21:DefaultSinkHeaderFile:skygraphctrl.h
# 	2:16:DefaultSinkClass:CSkyGraphCtrl
# End Section
# Section HardCtrl : {E9E074C9-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcCoor
# 	2:10:HeaderFile:vccoor1.h
# 	2:8:ImplFile:vccoor1.cpp
# End Section
# Section HardCtrl : {E9E07525-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxis
# 	2:10:HeaderFile:vcaxis1.h
# 	2:8:ImplFile:vcaxis1.cpp
# End Section
# Section HardCtrl : {E9E07515-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxisTitle
# 	2:10:HeaderFile:vcaxistitle1.h
# 	2:8:ImplFile:vcaxistitle1.cpp
# End Section
# Section HardCtrl : {D27CDB6E-AE6D-11CF-96B8-444553540000}
# 	2:21:DefaultSinkHeaderFile:shockwaveflash.h
# 	2:16:DefaultSinkClass:CShockwaveFlash
# End Section
# Section HardCtrl : {2777BFAD-4C0D-4C9C-9046-EB1936630E92}
# 	2:5:Class:CTrOcxNat
# 	2:10:HeaderFile:trocxnat.h
# 	2:8:ImplFile:trocxnat.cpp
# End Section
# Section HardCtrl : {E9E0750C-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataPointLabel
# 	2:10:HeaderFile:vcdatapointlabel1.h
# 	2:8:ImplFile:vcdatapointlabel1.cpp
# End Section
# Section HardCtrl : {E9E074F4-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcPlotBase
# 	2:10:HeaderFile:vcplotbase1.h
# 	2:8:ImplFile:vcplotbase1.cpp
# End Section
# Section HardCtrl : {E9E074CB-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLCoor
# 	2:10:HeaderFile:vclcoor1.h
# 	2:8:ImplFile:vclcoor1.cpp
# End Section
# Section HardCtrl : {E9E074E4-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcTitle
# 	2:10:HeaderFile:vctitle1.h
# 	2:8:ImplFile:vctitle1.cpp
# End Section
# Section HardCtrl : {7F75C1E7-3E98-4298-8767-227D8BAB46EB}
# 	2:21:DefaultSinkHeaderFile:trocxtc.h
# 	2:16:DefaultSinkClass:CTrOcxTc
# End Section
# Section HardCtrl : {E9E074CF-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcColor
# 	2:10:HeaderFile:vccolor1.h
# 	2:8:ImplFile:vccolor1.cpp
# End Section
# Section HardCtrl : {E9E074D8-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFont
# 	2:10:HeaderFile:vcfont1.h
# 	2:8:ImplFile:vcfont1.cpp
# End Section
# Section HardCtrl : {E9E07504-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeriesMarker
# 	2:10:HeaderFile:vcseriesmarker1.h
# 	2:8:ImplFile:vcseriesmarker1.cpp
# End Section
# Section HardCtrl : {E9E074E8-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLegend
# 	2:10:HeaderFile:vclegend1.h
# 	2:8:ImplFile:vclegend1.cpp
# End Section
# Section HardCtrl : {D0FD360C-A5FE-48B0-AA97-91EF96AA6D12}
# 	2:5:Class:CTrOcxTc
# 	2:10:HeaderFile:trocxtc.h
# 	2:8:ImplFile:trocxtc.cpp
# End Section
# Section HardCtrl : {E9E0751B-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcAxisGrid
# 	2:10:HeaderFile:vcaxisgrid1.h
# 	2:8:ImplFile:vcaxisgrid1.cpp
# End Section
# Section HardCtrl : {3A2B370C-BA0A-11D1-B137-0000F8753F5D}
# 	2:21:DefaultSinkHeaderFile:mschart1.h
# 	2:16:DefaultSinkClass:CMSChart
# End Section
# Section HardCtrl : {E9E074D3-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcMarker
# 	2:10:HeaderFile:vcmarker1.h
# 	2:8:ImplFile:vcmarker1.cpp
# End Section
# Section HardCtrl : {E9E074EA-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataGrid
# 	2:10:HeaderFile:vcdatagrid1.h
# 	2:8:ImplFile:vcdatagrid1.cpp
# End Section
# Section HardCtrl : {E9E074DA-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcBrush
# 	2:10:HeaderFile:vcbrush1.h
# 	2:8:ImplFile:vcbrush1.cpp
# End Section
# Section HardCtrl : {E9E074FA-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcWeighting
# 	2:10:HeaderFile:vcweighting1.h
# 	2:8:ImplFile:vcweighting1.cpp
# End Section
# Section HardCtrl : {E9E07508-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLabel
# 	2:10:HeaderFile:vclabel1.h
# 	2:8:ImplFile:vclabel1.cpp
# End Section
# Section HardCtrl : {E9E0751F-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcTick
# 	2:10:HeaderFile:vctick1.h
# 	2:8:ImplFile:vctick1.cpp
# End Section
# Section HardCtrl : {E9E07513-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcSeriesCollection
# 	2:10:HeaderFile:vcseriescollection1.h
# 	2:8:ImplFile:vcseriescollection1.cpp
# End Section
# Section HardCtrl : {E9E074EE-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLightSources
# 	2:10:HeaderFile:vclightsources1.h
# 	2:8:ImplFile:vclightsources1.cpp
# End Section
# Section HardCtrl : {D27CDB6C-AE6D-11CF-96B8-444553540000}
# 	2:5:Class:CShockwaveFlash
# 	2:10:HeaderFile:shockwaveflash.h
# 	2:8:ImplFile:shockwaveflash.cpp
# End Section
# Section HardCtrl : {E9E07523-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcValueScale
# 	2:10:HeaderFile:vcvaluescale1.h
# 	2:8:ImplFile:vcvaluescale1.cpp
# End Section
# Section HardCtrl : {E9E074DE-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcFill
# 	2:10:HeaderFile:vcfill1.h
# 	2:8:ImplFile:vcfill1.cpp
# End Section
# Section HardCtrl : {E9E0750A-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcLabels
# 	2:10:HeaderFile:vclabels1.h
# 	2:8:ImplFile:vclabels1.cpp
# End Section
# Section HardCtrl : {E9E074E2-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcBackdrop
# 	2:10:HeaderFile:vcbackdrop1.h
# 	2:8:ImplFile:vcbackdrop1.cpp
# End Section
# Section HardCtrl : {E9E0752A-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcDataPoints
# 	2:10:HeaderFile:vcdatapoints1.h
# 	2:8:ImplFile:vcdatapoints1.cpp
# End Section
# Section HardCtrl : {E9E074F2-BA0A-11D1-B137-0000F8753F5D}
# 	2:5:Class:CVcView3d
# 	2:10:HeaderFile:vcview3d1.h
# 	2:8:ImplFile:vcview3d1.cpp
# End Section
# Section HardCtrl : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font1.h
# 	2:8:ImplFile:font1.cpp
# End Section
