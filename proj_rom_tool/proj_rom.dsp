# Microsoft Developer Studio Project File - Name="proj_rom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=proj_rom - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "proj_rom.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "proj_rom.mak" CFG="proj_rom - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "proj_rom - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "proj_rom - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "proj_rom - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d8.lib d3dx8.lib dxguid.lib dsound.lib vorbis_static.lib ogg_static.lib vorbisfile_static.lib winmm.lib vfw32.lib /nologo /subsystem:windows /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\game\proj_rom_tool.exe"

!ELSEIF  "$(CFG)" == "proj_rom - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib dxguid.lib dsound.lib vorbis_static.lib ogg_static.lib vorbisfile_static.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\game\proj_rom_tool.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "proj_rom - Win32 Release"
# Name "proj_rom - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\chacha_sdk8_audio.cpp
# End Source File
# Begin Source File

SOURCE=.\chacha_sdk8_graphic.cpp
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MENU.cpp
# End Source File
# Begin Source File

SOURCE=.\proj_rom.cpp
# End Source File
# Begin Source File

SOURCE=.\proj_rom.rc
# End Source File
# Begin Source File

SOURCE=.\proj_romDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\proj_romView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TekiConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\TekiSelect.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\chacha_sdk8_audio.h
# End Source File
# Begin Source File

SOURCE=.\chacha_sdk8_graphic.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MENU.h
# End Source File
# Begin Source File

SOURCE=.\proj_rom.h
# End Source File
# Begin Source File

SOURCE=.\proj_romDoc.h
# End Source File
# Begin Source File

SOURCE=.\proj_romView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TekiConfig.h
# End Source File
# Begin Source File

SOURCE=.\tekipart.h
# End Source File
# Begin Source File

SOURCE=.\TekiSelect.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\proj_rom.ico
# End Source File
# Begin Source File

SOURCE=.\res\proj_rom.rc2
# End Source File
# Begin Source File

SOURCE=.\res\proj_romDoc.ico
# End Source File
# End Group
# Begin Group "libra"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\COMPRESS.CPP
# End Source File
# Begin Source File

SOURCE=.\COMPRESS.H
# End Source File
# Begin Source File

SOURCE=.\Libarc.cpp
# End Source File
# Begin Source File

SOURCE=.\Libarc.h
# End Source File
# Begin Source File

SOURCE=.\MMIO.CPP
# End Source File
# Begin Source File

SOURCE=.\MMIO.H
# End Source File
# Begin Source File

SOURCE=.\Uncompress.cpp
# End Source File
# Begin Source File

SOURCE=.\UNCOMPRESS.H
# End Source File
# Begin Source File

SOURCE=.\WAVE.CPP
# End Source File
# Begin Source File

SOURCE=.\WAVE.H
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
