#pragma  once

#include <winbase.h>
#include <iostream>
using namespace std;

class ExceptionClass
{
public:
	ExceptionClass(const char* name = "Exception Default Class")
	{
		cout << "Exception Class Construct String" << endl;
	}
	~ExceptionClass()
	{
		cout << "Exception Class Destruct String" << endl;
	}
	void ReportError()
	{
		cout << "Exception Class:: This is Report Error Message" << endl;
	}

	////�õ���������ı���Ϣ
	static std::string GetLastErrorToString(DWORD errorCode)
	{
		//��ΪFORMAT_MESSAGE_ALLOCATE_BUFFER��־�����������������ڴ棬������ҪLocalFree���ͷ�

		char *text;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&text, 0, NULL);
		string result(text);    //���
		LocalFree(text);
		return result;
	}

};