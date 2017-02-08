# Microsoft Developer Studio Project File - Name="VAC2 Proof Base" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VAC2 Proof Base - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VAC2 Proof Base.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VAC2 Proof Base.mak" CFG="VAC2 Proof Base - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VAC2 Proof Base - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VAC2 Proof Base - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VAC2 Proof Base - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VAC2PROOFBASE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VAC2PROOFBASE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "VAC2 Proof Base - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VAC2PROOFBASE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VAC2PROOFBASE_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VAC2 Proof Base - Win32 Release"
# Name "VAC2 Proof Base - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Added Headers"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\client_headers.h
# End Source File
# Begin Source File

SOURCE=.\mempatcher.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\DLLMain.cpp
# End Source File
# Begin Source File

SOURCE=.\engine.cpp
# End Source File
# Begin Source File

SOURCE=.\mempatcher.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ValveSDK\engine\anorms.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\beamdef.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\beamdef.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\cdll_dll.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\cdll_engine_dll.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\Cdll_int.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\cl_dll.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\cl_entity.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\cl_entity.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\com_model.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\con_nprint.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\const.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\const.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\crc.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\crc.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\custom.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\customentity.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\cvardef.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\cvardef.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\demo_api.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\director_cmds.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\dlight.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\dlight.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\dll_state.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\edict.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\eiface.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\engine_launcher_api.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\entity_state.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\entity_state.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\entity_types.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\entity_types.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\event_api.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\event_args.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\event_args.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\event_flags.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\event_flags.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\exefuncs.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\hltv.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\in_buttons.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\in_buttons.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\interface.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\itrackeruser.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\ivoicetweak.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\keydefs.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\net_api.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\netadr.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\nowin.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\misc\parsemsg.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\particledef.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\particledef.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\pm_defs.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\pm_info.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\pmtrace.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\pmtrace.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\progdefs.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\progs.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\qfont.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\r_efx.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\r_efx.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\r_studioint.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\ref_params.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\screenfade.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\shake.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\studio.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\studio_event.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\studio_event.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\triangleapi.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\triangleapi.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\usercmd.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\usercmd.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\util_vector.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\weaponinfo.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\weaponinfo.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\engine\wrect.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
