#include "stdafx.h"
#include <Windows.h>

#include <ASSERT.h>
#include "ProcessCpu.h"

/// ʱ��ת��
uint64_t CProcessCpuUse::file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	ASSERT(ftime);
	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

/// ���CPU�ĺ���
int  CProcessCpuUse::get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}

int CProcessCpuUse::get_cpu_usage()
{
	//cpu����
	static int processor_count_ = -1;
	//��һ�ε�ʱ��
	static int64_t last_time_ = 0;
	static int64_t last_system_time_ = 0;

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;

	int cpu = -1;

	if (processor_count_ == -1)
	{
		processor_count_ = get_processor_number();
	}

	GetSystemTimeAsFileTime(&now);

	if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
		&kernel_time, &user_time))
	{
		// We don't ASSERT here because in some cases (such as in the TaskManager)
		// we may call this function on a process that has just exited but we have
		// not yet received the notification.
		return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))

		/
		processor_count_;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		// First call, just set the last values.
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	ASSERT(time_delta != 0);

	if (time_delta == 0)
		return -1;

	// We add time_delta / 2 so the result is rounded.
	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;
	return cpu;
}

int CProcessCpuUse::get_memory_usage(uint64_t* mem, uint64_t* vmem)
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		if (mem) *mem = pmc.WorkingSetSize;
		if (vmem) *vmem = pmc.PagefileUsage;
		return 0;
	}
	return -1;
}

int CProcessCpuUse::get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes)
{
	IO_COUNTERS io_counter;
	if (GetProcessIoCounters(GetCurrentProcess(), &io_counter))
	{
		if (read_bytes) *read_bytes = io_counter.ReadTransferCount;
		if (write_bytes) *write_bytes = io_counter.WriteTransferCount;
		return 0;
	}
	return -1;
}