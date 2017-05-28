/***************************************************************************************************/
/*                      simpleCSimpleTimer 简单计时器                                                        */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/*                                                                                                  */
/***************************************************************************************************/

#pragma  once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <windows.h>

using namespace std;
void gotoxy(int x, int y)//定位光标，x为行坐标,y为列坐标
{
	COORD pos = { x, y };//（坐标  位置）；
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  //得到标准处理（标准输出处理）；
	SetConsoleCursorPosition(hOut, pos);//设置控制台光标位置；
}

void hidden()//隐藏光标
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = 0;//赋1为显示，赋0为隐藏
	SetConsoleCursorInfo(hOut, &cci);
}

class CSimpleTimer
{
private:
	long start_time;
	long pause_time;
	//两个bool值标记四种状态
	bool is_pause; //记录计时器的状态 （是否处于暂停状态）
	bool is_stop;//是否处于停止状态
public:
	CSimpleTimer();
	bool isPause(); //返回计时器状态
	bool isStop();
	//计时器的三种动作（功能）
	void Start();
	void Pause();
	void Stop();
	inline long getStartTime() { return start_time; }
	void show();
	void TestCode();
};

CSimpleTimer::CSimpleTimer()
{
	is_pause = false; //初始化计时器状态
	is_stop = true;
}

bool CSimpleTimer::isPause()
{
	if (is_pause)
		return true;
	else
		return false;
}

bool CSimpleTimer::isStop()
{
	if (is_stop)
		return true;
	return false;
}

void CSimpleTimer::Start() //开始
{
	if (is_stop)
	{
		start_time = (long)time(0);
		is_stop = false;
	}
	else if (is_pause)
	{
		is_pause = false;
		start_time += (long)time(0) - pause_time; //更新开始时间：用此时的时间 - 暂停时所用的时间 + 上一次开始的时间 = 此时的开始时间
	}
}

void CSimpleTimer::Pause() //暂停
{
	if (is_stop || is_pause) //如果处于停止/暂停状态,此动作不做任何处理，直接返回
		return;
	else    //否则调制为暂停状态
	{
		is_pause = true;
		pause_time = (long)time(0); //获取暂停时间
	}
}
void CSimpleTimer::Stop()  //停止
{
	if (is_stop) //如果正处于停止状态（不是暂停状态），不做任何处理
		return;
	else if (is_pause) //改变计时器状态
	{
		is_pause = false;
		is_stop = true;
	}
	else if (!is_stop)
	{
		is_stop = true;
	}
}

void CSimpleTimer::show()
{
	long t = (long)time(0) - start_time;
	gotoxy(35, 12);
	cout << setw(2) << setfill('0') << t / 60 / 60 << ":"
		<< setw(2) << setfill('0') << t / 60 << ":"
		<< setw(2) << setfill('0') << t % 60 << endl;
}

// 测试代码
void TestCode()
{
	CSimpleTimer t;
	char ch;
	hidden();//隐藏光标
	system("color 02");
	gotoxy(35, 12);
	cout << "00:00:00";
	gotoxy(20, 18);
	cout << "按a开始，按空格暂停，按s停止";
	while (1)
	{
		if (kbhit())
		{
			ch = getch();
			switch (ch)
			{
			case 'a':t.Start(); break;
			case 's':t.Stop(); break;
			case ' ':t.Pause(); break;
			default:break;
			}
		}
		if (!t.isStop() && !t.isPause())
		{
			t.show();
		}
	}
}