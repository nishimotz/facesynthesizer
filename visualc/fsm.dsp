# Microsoft Developer Studio Project File - Name="fsm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=fsm - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "fsm.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "fsm.mak" CFG="fsm - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "fsm - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "fsm - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE "fsm - Win32 Release for Galatea MMI" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fsm - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib ws2_32.lib opengl32.lib glu32.lib glut32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libcd.lib" /out:"../bin/fsm.exe" /libpath:"opengl32.lib glu32.lib glut32.lib glui.lib" /libpath:"."

!ELSEIF  "$(CFG)" == "fsm - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib opengl32.lib glu32.lib glut32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"../bin/fsm_d.exe" /pdbtype:sept /libpath:"."

!ELSEIF  "$(CFG)" == "fsm - Win32 Release for Galatea MMI"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "fsm___Win32_Release_for_Galatea_MMI"
# PROP BASE Intermediate_Dir "fsm___Win32_Release_for_Galatea_MMI"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "fsm___Win32_Release_for_Galatea_MMI"
# PROP Intermediate_Dir "fsm___Win32_Release_for_Galatea_MMI"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "WIN32_GALATEA" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib opengl32.lib glu32.lib glut32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libcd.lib" /out:"../bin/fsm.exe" /libpath:"opengl32.lib glu32.lib glut32.lib glui.lib" /libpath:"."
# ADD LINK32 winmm.lib ws2_32.lib opengl32.lib glu32.lib glut32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libcd.lib" /out:"../bin/fsm_mmi.exe" /libpath:"opengl32.lib glu32.lib glut32.lib glui.lib" /libpath:"."

!ENDIF 

# Begin Target

# Name "fsm - Win32 Release"
# Name "fsm - Win32 Debug"
# Name "fsm - Win32 Release for Galatea MMI"
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Main.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Main.h
# End Source File
# End Group
# Begin Group "View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\FaceEditApp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FaceEditApp.h
# End Source File
# Begin Source File

SOURCE=..\src\FaceEditView.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FaceEditView.h
# End Source File
# Begin Source File

SOURCE=..\src\FaceModel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FaceModel.h
# End Source File
# Begin Source File

SOURCE=..\src\FaceView.cpp
# End Source File
# Begin Source File

SOURCE=..\src\FaceView.h
# End Source File
# Begin Source File

SOURCE=..\src\trace.h
# End Source File
# Begin Source File

SOURCE=..\src\UnitMove.cpp
# End Source File
# Begin Source File

SOURCE=..\src\UnitMove.h
# End Source File
# End Group
# Begin Group "Reousrces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Resources.h
# End Source File
# Begin Source File

SOURCE=..\src\Resources_AU.h
# End Source File
# Begin Source File

SOURCE=..\src\Resources_MU.h
# End Source File
# Begin Source File

SOURCE=..\src\Resources_VM.h
# End Source File
# End Group
# Begin Group "Command Read & Write"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\CommandSet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\CommandSet.h
# End Source File
# Begin Source File

SOURCE=..\src\Configuration.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Configuration.h
# End Source File
# Begin Source File

SOURCE=..\src\ReadCommand.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ReadCommand.h
# End Source File
# Begin Source File

SOURCE=..\src\SetViseme.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SetViseme.h
# End Source File
# End Group
# Begin Group "Lib"

# PROP Default_Filter ""
# Begin Group "Image"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\ImageData.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ImageData.h
# End Source File
# Begin Source File

SOURCE=..\src\SgiImageFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SgiImageFormat.h
# End Source File
# Begin Source File

SOURCE=..\src\Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Texture.h
# End Source File
# Begin Source File

SOURCE=..\src\WinBitmapFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\src\WinBitmapFormat.h
# End Source File
# End Group
# Begin Group "WFM and OBJ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\ObjectModel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjectModel.h
# End Source File
# Begin Source File

SOURCE=..\src\ObjModel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjModel.h
# End Source File
# Begin Source File

SOURCE=..\src\Wfm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Wfm.h
# End Source File
# End Group
# Begin Group "Trans"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Trans.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Trans.h
# End Source File
# End Group
# Begin Group "Time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\ControlTime.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ControlTime.h
# End Source File
# End Group
# Begin Group "For Galatea-MMI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Sock.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Sock.h
# End Source File
# End Group
# End Group
# End Target
# End Project
