/***************************************************************************************************/
/*                       clock 获取程序运行时间                                                         */
/*                     1、clock   该程序从启动到函数调用占用CPU的时间。这个函数返回从“开启这个程序进程”到“程序    */
/* 									中调用clock()函数”时之间的CPU时钟计时单元（clock tick）数，在MSDN中称之为 */
/* 									挂钟时间（wal-clock）；若挂钟时间不可取，则返回-1。其中clock_t是用来      */
/* 									保存时间的数据类型。                                                 */
/*                     2、自定义精确时间。boost库                                                        */
/*                     3、自定义 CMicroSecondClockTikt 微秒                                            */
/*                                                                                                   */
/*                                                                                                   */
/*                                                                                                   */
/***************************************************************************************************/

#pragma  once
#include <time.h>
#include <stdio.h>
#include "..\..\Time\MicrosecondTime.h"

#ifndef CLOCKTIKT_H
#define CLOCKTIKT_H

// 系统自带精确时间：秒级别
class CClockTikt{
public:

	CClockTikt(){};
	~CClockTikt(){};

public:

	virtual clock_t GetStartClock(){ return clock(); }
	virtual	clock_t GetEndClock(){ return clock(); }

	//常量CLOCKS_PER_SEC，它用来表示一秒钟会有多少个时钟计时单元，其定义如下：#define CLOCKS_PER_SEC ((clock_t)1000)
	double GetTakenTime(clock_t m_start, clock_t m_end){ return (double)((m_end - m_start) / CLOCKS_PER_SEC); }

	// 时间打印
	void PrintTakenClock(clock_t m_start, clock_t m_end){
		cout << "Spend time: " << GetTakenTime(m_start, m_end) << "  seconds" << endl;
	}
};

class CMicroSecondClockTikt :public TimeConversion
{
public:

	CMicroSecondClockTikt(){};
	~CMicroSecondClockTikt(){};

public:

	virtual time_t GetStartClock(){ return GetUtcCaressing(); }
	virtual	time_t GetEndClock(){ return GetUtcCaressing(); }

	//常量CLOCKS_PER_SEC，它用来表示一秒钟会有多少个时钟计时单元，其定义如下：#define CLOCKS_PER_SEC ((clock_t)1000)
	double GetTakenTime(time_t m_start, time_t m_end){ return (double)((m_end - m_start) / CLOCKS_PER_SEC); }

	// 时间打印
	void PrintTakenClock(time_t m_start, time_t m_end){
		cout << "Spend time: " << GetTakenTime(m_start, m_end) << "  Microseconds" << endl;
	}
};

#endif

// 测试代码
//CClockTikt * m_clocktikt = new CClockTikt();
//开始时间
//m_clocktikt->GetStartClock();
//结束时间
//m_clocktikt->GetEndClock();
//m_clocktikt->PrintTakenClock(m_clocktikt->GetTakenTime());
//delete m_clocktikt;