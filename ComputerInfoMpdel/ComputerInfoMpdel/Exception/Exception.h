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

	////得到错误码的文本信息
	static std::string GetLastErrorToString(DWORD errorCode)
	{
		//因为FORMAT_MESSAGE_ALLOCATE_BUFFER标志，这个函数帮你分配内存，所以需要LocalFree来释放

		char *text;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&text, 0, NULL);
		string result(text);    //结果
		LocalFree(text);
		return result;
	}

};