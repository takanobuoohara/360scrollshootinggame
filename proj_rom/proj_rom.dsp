# Microsoft Developer Studio Project File - Name="proj_rom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=proj_rom - Win32 Debug2
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "proj_rom.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "proj_rom.mak" CFG="proj_rom - Win32 Debug2"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "proj_rom - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "proj_rom - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "proj_rom - Win32 beta" ("Win32 (x86) Application" 用)
!MESSAGE "proj_rom - Win32 Debug2" ("Win32 (x86) Application" 用)
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
# ADD CPP /nologo /MT /W3 /GX /Ot /Og /Oi /Op /Oy /Ob1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 vorbis_static.lib ogg_static.lib vorbisfile_static.lib winmm.lib dxguid.lib dxerr8.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib /nologo /stack:0xf4240,0x7a120 /subsystem:windows /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\game\sep_dim.exe"
# SUBTRACT LINK32 /incremental:yes

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vorbis_static_d.lib ogg_static_d.lib vorbisfile_static_d.lib winmm.lib dxguid.lib dxerr8.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\sep_dim.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "proj_rom - Win32 beta"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "proj_rom___Win32_beta"
# PROP BASE Intermediate_Dir "proj_rom___Win32_beta"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "proj_rom___Win32_beta"
# PROP Intermediate_Dir "proj_rom___Win32_beta"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /W3 /Gi /GX /Ot /Og /Oi /Op /Oy /Ob1 /Gf /Gy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MT /W3 /Gi /GX /Ot /Og /Oi /Op /Oy /Ob1 /Gf /Gy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "SD_BETA" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 vorbis_static.lib ogg_static.lib vorbisfile_static.lib winmm.lib dxguid.lib dxerr8.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib /nologo /subsystem:windows /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\game\sep_dim.exe"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 vorbis_static.lib ogg_static.lib vorbisfile_static.lib winmm.lib dxguid.lib dxerr8.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib /nologo /subsystem:windows /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\game\sep_dim_trial.exe"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "proj_rom - Win32 Debug2"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "proj_rom___Win32_Debug2"
# PROP BASE Intermediate_Dir "proj_rom___Win32_Debug2"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "proj_rom___Win32_Debug2"
# PROP Intermediate_Dir "proj_rom___Win32_Debug2"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 vorbis_static_d.lib ogg_static_d.lib vorbisfile_static_d.lib winmm.lib dxguid.lib dxerr8.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\sep_dim.exe" /pdbtype:sept
# ADD LINK32 vorbis_static_d.lib ogg_static_d.lib vorbisfile_static_d.lib winmm.lib dxguid.lib dxerr8.lib d3d8.lib d3dx8.lib dsound.lib dinput8.lib /nologo /subsystem:windows /machine:I386 /out:"C:\PROJECTS\APPLICATION\proj_rom\Release\game\sep_dim.exe"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "proj_rom - Win32 Release"
# Name "proj_rom - Win32 Debug"
# Name "proj_rom - Win32 beta"
# Name "proj_rom - Win32 Debug2"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\chacha_sdk8_audio.cpp
# End Source File
# Begin Source File

SOURCE=.\chacha_sdk8_graphic.cpp
# End Source File
# Begin Source File

SOURCE=.\chacha_sdk8_Input.cpp
# End Source File
# Begin Source File

SOURCE=.\File_Class.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrmMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\proj_rom.cpp
# End Source File
# Begin Source File

SOURCE=.\proj_rom.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
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

SOURCE=.\chacha_sdk8_Input.h
# End Source File
# Begin Source File

SOURCE=.\File_Class.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\proj_rom.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tekipart.h
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
# Begin Source File

SOURCE=.\res\SEP.ico
# End Source File
# End Group
# Begin Group "libra"

# PROP Default_Filter ""
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
