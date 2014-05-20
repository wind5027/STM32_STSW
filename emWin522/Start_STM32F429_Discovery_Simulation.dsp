# Microsoft Developer Studio Project File - Name="Start_STM32F429_Discovery" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Start_STM32F429_Discovery - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Start_STM32F429_Discovery_Simulation.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Start_STM32F429_Discovery_Simulation.mak" CFG="Start_STM32F429_Discovery - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Start_STM32F429_Discovery - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Start_STM32F429_Discovery - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Start_STM32F429_Discovery - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O1 /I "Setup" /I "FS" /I "GUI" /I "USBH" /I "Inc" /I "Config" /I "Windows\GUI" /I "Application\SEGGERDEMO" /I "Application\SEGGERDEMO\Src" /D "NDEBUG" /D "TARGET_1375_C8_137X" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fo"Output/STM32F429_Discovery/emWinSimulation_Release/" /Fd"Output/STM32F429_Discovery/emWinSimulation_Release/" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /fo"Output/STM32F429_Discovery/emWinSimulation_Release/STM32F429_Discovery_Simulation.res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Output/STM32F429_Discovery/emWinSimulator_Release/Start_STM32F429_Discovery_Simulation.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib winmm.lib libcmt.lib /nologo /subsystem:windows /pdb:none /map:"Output/STM32F429_Discovery/emWinSimulation_Release/Start_STM32F429_Discovery_Simulation.map" /machine:I386 /nodefaultlib /out:"Windows/GUI/Start_STM32F429_Discovery_Simulation.exe"

!ELSEIF  "$(CFG)" == "Start_STM32F429_Discovery - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /I "Setup" /I "GUI" /I "USBH" /I "Inc" /I "Config" /I "Windows\GUI" /I "Application\SEGGERDEMO" /I "Application\SEGGERDEMO\Src" /D "_DEBUG" /D "TARGET_1375_C8_137X" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fo"Output/STM32F429_Discovery/emWinSimulation_Debug/" /Fd"Output/STM32F429_Discovery/emWinSimulation_Debug/" /FD /GZ "FS" /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"Output/STM32F429_Discovery/emWinSimulation_Debug/STM32F429_Discovery_Simulation.res"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Output/STM32F429_Discovery/emWinSIM_Debug/Start_STM32F429_Discovery_Simulation.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib shell32.lib winmm.lib libcmt.lib /nologo /subsystem:windows /pdb:"Output/STM32F429_Discovery/emWinSimulation_Debug/Start_STM32F429_Discovery_Simulation.pdb" /debug /machine:I386 /nodefaultlib /out:"Windows/GUI/Start_STM32F429_Discovery_Simulation_DEBUG.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Start_STM32F429_Discovery - Win32 Release"
# Name "Start_STM32F429_Discovery - Win32 Debug"
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Group "SEGGERDEMO"

# PROP Default_Filter ""
# Begin Group "Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO.h
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_AntialiasedText.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Automotive.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_BarGraph.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Bitmap.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_ColorBar.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Cursor.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Fading.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Graph.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_IconView.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_ImageFlow.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Intro.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Listview.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_RadialMenu.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Resource.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Skinning.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Speed.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Speedometer.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_TransparentDialog.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_Treeview.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_VScreen.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_WashingMachine.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\GUIDEMO_ZoomAndRotate.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\Src\IMAGE_SeggerLogo_300x150_565c.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\SEGGERDEMO.c
# End Source File
# Begin Source File

SOURCE=.\Application\SEGGERDEMO\SysConfDefaults.h
# End Source File
# End Group
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Windows\GUI\GUISIMx86.res
# End Source File
# Begin Source File

SOURCE=.\Windows\GUI\SIM_X.c
# End Source File
# Begin Source File

SOURCE=.\Windows\GUI\GUISIMx86.lib
# End Source File
# Begin Source File

SOURCE=.\Windows\GUI\GUIv5x86_trial.lib
# End Source File
# End Group
# End Group
# Begin Group "Setup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Setup\GUIConf.c
# End Source File
# End Group
# End Target
# End Project
