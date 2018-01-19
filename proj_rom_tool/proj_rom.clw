; CLW ファイルは MFC ClassWizard の情報を含んでいます。

[General Info]
Version=1
LastClass=CTekiSelect
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "proj_rom.h"
LastPage=0

ClassCount=8
Class1=CProj_romApp
Class2=CProj_romDoc
Class3=CProj_romView
Class4=CMainFrame

ResourceCount=6
Resource1=IDR_MENU1
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG1
Class6=CMENU
Resource4=IDD_DIALOG2
Class7=CTekiConfig
Resource5=IDD_ABOUTBOX
Class8=CTekiSelect
Resource6=IDD_DIALOG3

[CLS:CProj_romApp]
Type=0
HeaderFile=proj_rom.h
ImplementationFile=proj_rom.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CProj_romApp

[CLS:CProj_romDoc]
Type=0
HeaderFile=proj_romDoc.h
ImplementationFile=proj_romDoc.cpp
Filter=N

[CLS:CProj_romView]
Type=0
HeaderFile=proj_romView.h
ImplementationFile=proj_romView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=W
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=proj_rom.cpp
ImplementationFile=proj_rom.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_APP_ABOUT
CommandCount=11

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=IDM_LOAD
Command2=IDM_SAVE
Command3=IDM_END
Command4=IDM_1
Command5=IDM_2
Command6=IDM_4
Command7=IDM_8
Command8=IDM_16
Command9=IDM_syoki
Command10=IDM_MENU
Command11=IDM_TEKI1
CommandCount=11

[DLG:IDD_DIALOG1]
Type=1
Class=CMENU
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1073807360
Control3=IDC_EDIT1,edit,1350639744
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350639744
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT3,edit,1350639744
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT4,edit,1350639744
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[CLS:CMENU]
Type=0
HeaderFile=MENU.h
ImplementationFile=MENU.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CTekiConfig
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_LIST1,SysListView32,1350631425
Control6=IDC_LIST2,SysListView32,1350631425
Control7=IDC_STATIC,button,1342177287
Control8=IDC_COMBO1,combobox,1344339971
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT5,edit,1350631552
Control12=IDC_BUTTON1,button,1342242816
Control13=IDC_BUTTON2,button,1342242816
Control14=IDC_BUTTON3,button,1342242816
Control15=IDC_CHECK1,button,1342242819

[CLS:CTekiConfig]
Type=0
HeaderFile=TekiConfig.h
ImplementationFile=TekiConfig.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CHECK1
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=CTekiSelect
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425
Control4=IDC_BUTTON1,button,1342242816

[CLS:CTekiSelect]
Type=0
HeaderFile=TekiSelect.h
ImplementationFile=TekiSelect.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON1
VirtualFilter=dWC

