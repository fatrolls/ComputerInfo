; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInjectDllDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "InjectDll.h"

ClassCount=3
Class1=CInjectDllApp
Class2=CInjectDllDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_INJECTDLL_DIALOG

[CLS:CInjectDllApp]
Type=0
HeaderFile=InjectDll.h
ImplementationFile=InjectDll.cpp
Filter=N

[CLS:CInjectDllDlg]
Type=0
HeaderFile=InjectDllDlg.h
ImplementationFile=InjectDllDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDITPID

[CLS:CAboutDlg]
Type=0
HeaderFile=InjectDllDlg.h
ImplementationFile=InjectDllDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_INJECTDLL_DIALOG]
Type=1
Class=CInjectDllDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_UNLOAD,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDITDLL,edit,1350631552
Control6=IDC_EDITPID,edit,1350631552

