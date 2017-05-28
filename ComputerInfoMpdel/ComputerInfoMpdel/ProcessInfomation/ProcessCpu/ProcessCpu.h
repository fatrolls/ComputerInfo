#pragma once
#ifndef PROCESS_CPU_H
#define	PROCESS_CPU_H

#include <Psapi.h>
#pragma  comment (lib, "Psapi.Lib")

#ifndef __cplusplus
extern "C"{
#endif

	typedef long long           int64_t;
	typedef unsigned long long  uint64_t;

	class CProcessCpuUse{
	public:

		CProcessCpuUse(){};
		~CProcessCpuUse(){};

	public:

		/// ��ȡ��ǰ���̵�cpuʹ���ʣ�����-1ʧ��
		int get_cpu_usage();

		/// ��ȡ��ǰ�����ڴ�������ڴ�ʹ����������-1ʧ�ܣ�0�ɹ�
		int get_memory_usage(uint64_t* mem, uint64_t* vmem);

		/// ��ȡ��ǰ�����ܹ�����д��IO�ֽ���������-1ʧ�ܣ�0�ɹ�
		int get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes);

		// ʱ��ת��
		static uint64_t file_time_2_utc(const FILETIME* ftime);

		/// ���CPU�ĺ���
		static int get_processor_number();

		int processcpuget_memory_usage(uint64_t* mem, uint64_t* vmem);
	};

#ifndef __cplusplus
}
#endif;
#endif // PROCESS_CPU_H