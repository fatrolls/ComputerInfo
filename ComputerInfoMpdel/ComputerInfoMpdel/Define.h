#pragma  once

#ifndef DEFINE_H
#define  DEFINE_H

//-----------------include------------------------
#include <Windows.h>


#ifdef DEFINE_DLL
#define DEFINE_API _declspec (dllexport)
#else
#define DEFINE_API _declspec(dllimport)
#endif

#ifdef  __cplusplus
extern  "C"{
#endif

#define CPU_ERROR_FAILED		0xFFFF

#define BUF_SIZE 256

	typedef struct WHPOINT     // ©М╦ъ
	{
		int width;
		int height;
		WHPOINT() :width(0), height(0){}
	}WhPoint;



#define  MU_MOVE  MOUSEEVENTF_MOVE
#define  MU_LDOWN MOUSEEVENTF_LEFTDOWN
#define  MU_LUP   MOUSEEVENTF_LEFTUP
#define  MU_RDOWN MOUSEEVENTF_RIGHTDOWN
#define  MU_RUP   MOUSEEVENTF_RIGHTUP
#define  MU_MDOWN MOUSEEVENTF_MIDDLEDOWN
#define  MU_MUP	  MOUSEEVENTF_MIDDLEUP
#define	 MU_ALE	  MOUSEEVENTF_ABSOLUTE

#define  MU_LCLK  MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP
#define  MU_RCLK  MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP

	/*
	* Standard Cursor IDs
	*/
#define IDC_ARROW           MAKEINTRESOURCE(32512)
#define IDC_IBEAM           MAKEINTRESOURCE(32513)
#define IDC_WAIT            MAKEINTRESOURCE(32514)
#define IDC_CROSS           MAKEINTRESOURCE(32515)
#define IDC_UPARROW         MAKEINTRESOURCE(32516)
#define IDC_SIZE            MAKEINTRESOURCE(32640)  /* OBSOLETE: use IDC_SIZEALL */
#define IDC_ICON            MAKEINTRESOURCE(32641)  /* OBSOLETE: use IDC_ARROW */
#define IDC_SIZENWSE        MAKEINTRESOURCE(32642)
#define IDC_SIZENESW        MAKEINTRESOURCE(32643)
#define IDC_SIZEWE          MAKEINTRESOURCE(32644)
#define IDC_SIZENS          MAKEINTRESOURCE(32645)
#define IDC_SIZEALL         MAKEINTRESOURCE(32646)
#define IDC_NO              MAKEINTRESOURCE(32648) /*not in win3.1 */
#if(WINVER >= 0x0500)
#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif /* WINVER >= 0x0500 */
#define IDC_APPSTARTING     MAKEINTRESOURCE(32650) /*not in win3.1 */
#if(WINVER >= 0x0400)
#define IDC_HELP            MAKEINTRESOURCE(32651)
#endif /* WINVER >= 0x0400 */

	//--------KyBd----------------------------------------------

#define  IBMPCXT		"IBM PC - XT or compatible(83 - key) keyboard"
#define  OlivettiICO	"Olivetti ICO(102 - key) keyboard"
#define  IBMPCAT		"IBM PC/AT (84-key) or similar keyboard"
#define	 IBMEnhanced	"IBM enhanced (101- or 102-key) keyboard"
#define	 Nokia1050similar 	"Nokia 1050 and similar keyboards"
#define	 Nokia9140similar	"Nokia 9140 and similar keyboards"
#define	 Japanese			"Japanese keyboard"
#define  NOTFIND				"No Find Keyboard Type"

	//void WriteLog(const char* filepath, std::string strError)
	//{
	//	CLog* m_log = new CLog(filepath);
	//	m_log->OpenLogFile(filepath);
	//	m_log->WriteLog(strError);
	//	m_log->CloseLogFile();
	//}


	// ╪сть


	




#ifdef  __cplusplus
}
#endif
#endif  // DEFINE_H