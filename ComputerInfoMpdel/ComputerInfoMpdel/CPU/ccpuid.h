////////////////////////////////////////////////////////////
// ccpuid.h: CPUID��Ϣ.
// Author: zyl910
// Blog: http://www.cnblogs.com/zyl910
// URL: http://www.cnblogs.com/zyl910/archive/2012/07/11/ccpuid.html
// Version: V1.0
// Updata: 2012-07-11
////////////////////////////////////////////////////////////

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef __CCPUID_H_INCLUDED
#define __CCPUID_H_INCLUDED

#include <stddef.h>	// NULL�ȱ�׼�������.
#include <basetsd.h>	// INT32��UINT32�ȹ淶������.

#if _MSC_VER >=1400	// VC2005��֧��intrin.h
#include <intrin.h>	// ����Intrinsics����.
#else
#include <emmintrin.h>	// MMX, SSE, SSE2
#endif

#if defined __cplusplus
extern "C" {
#endif

// __cpuid,__cpuidex
#if defined(_WIN64)
// 64λ�²�֧���������. Ӧʹ��__cpuid��__cpuidex��Intrinsics����.
#else
#if _MSC_VER < 1600	// VS2010. ��˵VC2008 SP1֮���֧��__cpuidex
inline void __cpuidex(INT32 CPUInfo[4], INT32 InfoType, INT32 ECXValue)
{
	if (0==CPUInfo)	return;
	_asm{
		// load. ��ȡ�������Ĵ���.
		mov edi, CPUInfo;	// ׼����ediѰַCPUInfo
		mov eax, InfoType;
		mov ecx, ECXValue;
		// CPUID
		cpuid;
		// save. ���Ĵ������浽CPUInfo
		mov	[edi], eax;
		mov	[edi+4], ebx;
		mov	[edi+8], ecx;
		mov	[edi+12], edx;
	}
}
#endif	// #if _MSC_VER < 1600	// VS2010. ��˵VC2008 SP1֮���֧��__cpuidex

#if _MSC_VER < 1400	// VC2005��֧��__cpuid
inline void __cpuid(INT32 CPUInfo[4], INT32 InfoType)
{
	__cpuidex(CPUInfo, InfoType, 0);
}
#endif	// #if _MSC_VER < 1400	// VC2005��֧��__cpuid

#endif	// #if defined(_WIN64)


// CPUIDFIELD
typedef INT32 CPUIDFIELD;

#define  CPUIDFIELD_MASK_POS	0x0000001F	// λƫ��. 0~31.
#define  CPUIDFIELD_MASK_LEN	0x000003E0	// λ��. 1~32
#define  CPUIDFIELD_MASK_REG	0x00000C00	// �Ĵ���. 0=EAX, 1=EBX, 2=ECX, 3=EDX.
#define  CPUIDFIELD_MASK_FIDSUB	0x000FF000	// �ӹ��ܺ�(��8λ).
#define  CPUIDFIELD_MASK_FID	0xFFF00000	// ���ܺ�(���4λ �� ��8λ).

#define CPUIDFIELD_SHIFT_POS	0
#define CPUIDFIELD_SHIFT_LEN	5
#define CPUIDFIELD_SHIFT_REG	10
#define CPUIDFIELD_SHIFT_FIDSUB	12
#define CPUIDFIELD_SHIFT_FID	20

#define CPUIDFIELD_MAKE(fid,fidsub,reg,pos,len)	(((fid)&0xF0000000) \
	| ((fid)<<CPUIDFIELD_SHIFT_FID & 0x0FF00000) \
	| ((fidsub)<<CPUIDFIELD_SHIFT_FIDSUB & CPUIDFIELD_MASK_FIDSUB) \
	| ((reg)<<CPUIDFIELD_SHIFT_REG & CPUIDFIELD_MASK_REG) \
	| ((pos)<<CPUIDFIELD_SHIFT_POS & CPUIDFIELD_MASK_POS) \
	| (((len)-1)<<CPUIDFIELD_SHIFT_LEN & CPUIDFIELD_MASK_LEN) \
	)
#define CPUIDFIELD_FID(cpuidfield)	( ((cpuidfield)&0xF0000000) | (((cpuidfield) & 0x0FF00000)>>CPUIDFIELD_SHIFT_FID) )
#define CPUIDFIELD_FIDSUB(cpuidfield)	( ((cpuidfield) & CPUIDFIELD_MASK_FIDSUB)>>CPUIDFIELD_SHIFT_FIDSUB )
#define CPUIDFIELD_REG(cpuidfield)	( ((cpuidfield) & CPUIDFIELD_MASK_REG)>>CPUIDFIELD_SHIFT_REG )
#define CPUIDFIELD_POS(cpuidfield)	( ((cpuidfield) & CPUIDFIELD_MASK_POS)>>CPUIDFIELD_SHIFT_POS )
#define CPUIDFIELD_LEN(cpuidfield)	( (((cpuidfield) & CPUIDFIELD_MASK_LEN)>>CPUIDFIELD_SHIFT_LEN) + 1 )

typedef struct tagCPUIDFIELDDESC{
	CPUIDFIELD	cpuf;
	INT32	reserved;
	const char* szName;
	const char* szDesc;
}CPUIDFIELDDESC;

#define CPUF_LFuncStd	CPUIDFIELD_MAKE(0,0,0,0,32)
#define CPUF_Stepping	CPUIDFIELD_MAKE(1,0,0,0,4)
#define CPUF_BaseModel	CPUIDFIELD_MAKE(1,0,0,4,4)
#define CPUF_BaseFamily	CPUIDFIELD_MAKE(1,0,0,8,4)
#define CPUF_ProcessorType	CPUIDFIELD_MAKE(1,0,0,12,2)
#define CPUF_ExtModel	CPUIDFIELD_MAKE(1,0,0,16,4)
#define CPUF_ExtFamily	CPUIDFIELD_MAKE(1,0,0,20,8)
#define CPUF_BrandId8	CPUIDFIELD_MAKE(1,0,1,0,8)
#define CPUF_CLFlush	CPUIDFIELD_MAKE(1,0,1,8,8)
#define CPUF_MaxApicId	CPUIDFIELD_MAKE(1,0,1,16,8)
#define CPUF_ApicId	CPUIDFIELD_MAKE(1,0,1,24,8)
#define CPUF_SSE3	CPUIDFIELD_MAKE(1,0,2,0,1)
#define CPUF_PCLMULQDQ	CPUIDFIELD_MAKE(1,0,2,1,1)
#define CPUF_DTES64	CPUIDFIELD_MAKE(1,0,2,2,1)
#define CPUF_MONITOR	CPUIDFIELD_MAKE(1,0,2,3,1)
#define CPUF_DS_CPL	CPUIDFIELD_MAKE(1,0,2,4,1)
#define CPUF_VMX	CPUIDFIELD_MAKE(1,0,2,5,1)
#define CPUF_SMX	CPUIDFIELD_MAKE(1,0,2,6,1)
#define CPUF_EIST	CPUIDFIELD_MAKE(1,0,2,7,1)
#define CPUF_TM2	CPUIDFIELD_MAKE(1,0,2,8,1)
#define CPUF_SSSE3	CPUIDFIELD_MAKE(1,0,2,9,1)
#define CPUF_CNXT_ID	CPUIDFIELD_MAKE(1,0,2,10,1)
#define CPUF_FMA	CPUIDFIELD_MAKE(1,0,2,12,1)
#define CPUF_CMPXCHG16B	CPUIDFIELD_MAKE(1,0,2,13,1)
#define CPUF_xTPR	CPUIDFIELD_MAKE(1,0,2,14,1)
#define CPUF_PDCM	CPUIDFIELD_MAKE(1,0,2,15,1)
#define CPUF_PCID	CPUIDFIELD_MAKE(1,0,2,17,1)
#define CPUF_DCA	CPUIDFIELD_MAKE(1,0,2,18,1)
#define CPUF_SSE41	CPUIDFIELD_MAKE(1,0,2,19,1)
#define CPUF_SSE42	CPUIDFIELD_MAKE(1,0,2,20,1)
#define CPUF_x2APIC	CPUIDFIELD_MAKE(1,0,2,21,1)
#define CPUF_MOVBE	CPUIDFIELD_MAKE(1,0,2,22,1)
#define CPUF_POPCNT	CPUIDFIELD_MAKE(1,0,2,23,1)
#define CPUF_TSC_DEADLINE	CPUIDFIELD_MAKE(1,0,2,24,1)
#define CPUF_AES	CPUIDFIELD_MAKE(1,0,2,25,1)
#define CPUF_XSAVE	CPUIDFIELD_MAKE(1,0,2,26,1)
#define CPUF_OSXSAVE	CPUIDFIELD_MAKE(1,0,2,27,1)
#define CPUF_AVX	CPUIDFIELD_MAKE(1,0,2,28,1)
#define CPUF_F16C	CPUIDFIELD_MAKE(1,0,2,29,1)
#define CPUF_RDRAND	CPUIDFIELD_MAKE(1,0,2,30,1)
#define CPUF_FPU	CPUIDFIELD_MAKE(1,0,3,0,1)
#define CPUF_VME	CPUIDFIELD_MAKE(1,0,3,1,1)
#define CPUF_DE	CPUIDFIELD_MAKE(1,0,3,2,1)
#define CPUF_PSE	CPUIDFIELD_MAKE(1,0,3,3,1)
#define CPUF_TSC	CPUIDFIELD_MAKE(1,0,3,4,1)
#define CPUF_MSR	CPUIDFIELD_MAKE(1,0,3,5,1)
#define CPUF_PAE	CPUIDFIELD_MAKE(1,0,3,6,1)
#define CPUF_MCE	CPUIDFIELD_MAKE(1,0,3,7,1)
#define CPUF_CX8	CPUIDFIELD_MAKE(1,0,3,8,1)
#define CPUF_APIC	CPUIDFIELD_MAKE(1,0,3,9,1)
#define CPUF_SEP	CPUIDFIELD_MAKE(1,0,3,11,1)
#define CPUF_MTRR	CPUIDFIELD_MAKE(1,0,3,12,1)
#define CPUF_PGE	CPUIDFIELD_MAKE(1,0,3,13,1)
#define CPUF_MCA	CPUIDFIELD_MAKE(1,0,3,14,1)
#define CPUF_CMOV	CPUIDFIELD_MAKE(1,0,3,15,1)
#define CPUF_PAT	CPUIDFIELD_MAKE(1,0,3,16,1)
#define CPUF_PSE36	CPUIDFIELD_MAKE(1,0,3,17,1)
#define CPUF_PSN	CPUIDFIELD_MAKE(1,0,3,18,1)
#define CPUF_CLFSH	CPUIDFIELD_MAKE(1,0,3,19,1)
#define CPUF_DS	CPUIDFIELD_MAKE(1,0,3,21,1)
#define CPUF_ACPI	CPUIDFIELD_MAKE(1,0,3,22,1)
#define CPUF_MMX	CPUIDFIELD_MAKE(1,0,3,23,1)
#define CPUF_FXSR	CPUIDFIELD_MAKE(1,0,3,24,1)
#define CPUF_SSE	CPUIDFIELD_MAKE(1,0,3,25,1)
#define CPUF_SSE2	CPUIDFIELD_MAKE(1,0,3,26,1)
#define CPUF_SS	CPUIDFIELD_MAKE(1,0,3,27,1)
#define CPUF_HTT	CPUIDFIELD_MAKE(1,0,3,28,1)
#define CPUF_TM	CPUIDFIELD_MAKE(1,0,3,29,1)
#define CPUF_PBE	CPUIDFIELD_MAKE(1,0,3,31,1)
#define CPUF_Cache_Type	CPUIDFIELD_MAKE(4,0,0,0,5)
#define CPUF_Cache_Level	CPUIDFIELD_MAKE(4,0,0,5,3)
#define CPUF_CACHE_SI	CPUIDFIELD_MAKE(4,0,0,8,1)
#define CPUF_CACHE_FA	CPUIDFIELD_MAKE(4,0,0,9,1)
#define CPUF_MaxApicIdShare	CPUIDFIELD_MAKE(4,0,0,14,12)
#define CPUF_MaxApicIdCore	CPUIDFIELD_MAKE(4,0,0,26,6)
#define CPUF_Cache_LineSize	CPUIDFIELD_MAKE(4,0,1,0,12)
#define CPUF_Cache_Partitions	CPUIDFIELD_MAKE(4,0,1,12,10)
#define CPUF_Cache_Ways	CPUIDFIELD_MAKE(4,0,1,22,10)
#define CPUF_Cache_Sets	CPUIDFIELD_MAKE(4,0,2,0,32)
#define CPUF_CACHE_INVD	CPUIDFIELD_MAKE(4,0,3,0,1)
#define CPUF_CACHE_INCLUSIVENESS	CPUIDFIELD_MAKE(4,0,3,1,1)
#define CPUF_CACHE_COMPLEXINDEX	CPUIDFIELD_MAKE(4,0,3,2,1)
#define CPUF_MonLineSizeMin	CPUIDFIELD_MAKE(5,0,0,0,16)
#define CPUF_MonLineSizeMax	CPUIDFIELD_MAKE(5,0,1,0,16)
#define CPUF_EMX	CPUIDFIELD_MAKE(5,0,2,0,1)
#define CPUF_IBE	CPUIDFIELD_MAKE(5,0,2,1,1)
#define CPUF_MWAIT_Number_C0	CPUIDFIELD_MAKE(5,0,3,0,4)
#define CPUF_MWAIT_Number_C1	CPUIDFIELD_MAKE(5,0,3,4,4)
#define CPUF_MWAIT_Number_C2	CPUIDFIELD_MAKE(5,0,3,8,4)
#define CPUF_MWAIT_Number_C3	CPUIDFIELD_MAKE(5,0,3,12,4)
#define CPUF_MWAIT_Number_C4	CPUIDFIELD_MAKE(5,0,3,16,4)
#define CPUF_DTS	CPUIDFIELD_MAKE(6,0,0,0,1)
#define CPUF_TURBO_BOOST	CPUIDFIELD_MAKE(6,0,0,1,1)
#define CPUF_ARAT	CPUIDFIELD_MAKE(6,0,0,2,1)
#define CPUF_PLN	CPUIDFIELD_MAKE(6,0,0,4,1)
#define CPUF_ECMD	CPUIDFIELD_MAKE(6,0,0,5,1)
#define CPUF_PTM	CPUIDFIELD_MAKE(6,0,0,6,1)
#define CPUF_DTS_ITs	CPUIDFIELD_MAKE(6,0,1,0,4)
#define CPUF_PERF	CPUIDFIELD_MAKE(6,0,2,0,1)
#define CPUF_ACNT2	CPUIDFIELD_MAKE(6,0,2,1,1)
#define CPUF_ENERGY_PERF_BIAS	CPUIDFIELD_MAKE(6,0,2,3,1)
#define CPUF_Max07Subleaf	CPUIDFIELD_MAKE(7,0,0,0,32)
#define CPUF_FSGSBASE	CPUIDFIELD_MAKE(7,0,1,0,1)
#define CPUF_BMI1	CPUIDFIELD_MAKE(7,0,1,3,1)
#define CPUF_HLE	CPUIDFIELD_MAKE(7,0,1,4,1)
#define CPUF_AVX2	CPUIDFIELD_MAKE(7,0,1,5,1)
#define CPUF_SMEP	CPUIDFIELD_MAKE(7,0,1,7,1)
#define CPUF_BMI2	CPUIDFIELD_MAKE(7,0,1,8,1)
#define CPUF_ERMS	CPUIDFIELD_MAKE(7,0,1,9,1)
#define CPUF_INVPCID	CPUIDFIELD_MAKE(7,0,1,10,1)
#define CPUF_RTM	CPUIDFIELD_MAKE(7,0,1,11,1)
#define CPUF_PLATFORM_DCA_CAP	CPUIDFIELD_MAKE(9,0,0,0,32)
#define CPUF_APM_Version	CPUIDFIELD_MAKE(0xA,0,0,0,8)
#define CPUF_APM_Counters	CPUIDFIELD_MAKE(0xA,0,0,8,8)
#define CPUF_APM_Bits	CPUIDFIELD_MAKE(0xA,0,0,16,8)
#define CPUF_APM_Length	CPUIDFIELD_MAKE(0xA,0,0,24,8)
#define CPUF_APM_CC	CPUIDFIELD_MAKE(0xA,0,1,0,1)
#define CPUF_APM_IR	CPUIDFIELD_MAKE(0xA,0,1,1,1)
#define CPUF_APM_RC	CPUIDFIELD_MAKE(0xA,0,1,2,1)
#define CPUF_APM_LLCR	CPUIDFIELD_MAKE(0xA,0,1,3,1)
#define CPUF_APM_LLCM	CPUIDFIELD_MAKE(0xA,0,1,4,1)
#define CPUF_APM_BIR	CPUIDFIELD_MAKE(0xA,0,1,5,1)
#define CPUF_APM_BMR	CPUIDFIELD_MAKE(0xA,0,1,6,1)
#define CPUF_APM_FC_Number	CPUIDFIELD_MAKE(0xA,0,3,0,5)
#define CPUF_APM_FC_Bits	CPUIDFIELD_MAKE(0xA,0,3,5,8)
#define CPUF_Topology_Bits	CPUIDFIELD_MAKE(0xB,0,0,0,5)
#define CPUF_Topology_Number	CPUIDFIELD_MAKE(0xB,0,1,0,16)
#define CPUF_Topology_Level	CPUIDFIELD_MAKE(0xB,0,2,0,8)
#define CPUF_Topology_Type	CPUIDFIELD_MAKE(0xB,0,2,8,8)
#define CPUF_X2APICID	CPUIDFIELD_MAKE(0xB,0,3,0,32)
#define CPUF_XFeatureSupportedMaskLo	CPUIDFIELD_MAKE(0xD,0,0,0,32)
#define CPUF_XFeatureEnabledSizeMax	CPUIDFIELD_MAKE(0xD,0,1,0,32)
#define CPUF_XFeatureSupportedSizeMax	CPUIDFIELD_MAKE(0xD,0,2,0,32)
#define CPUF_XFeatureSupportedMaskHi	CPUIDFIELD_MAKE(0xD,0,3,0,32)
#define CPUF_XSAVEOPT	CPUIDFIELD_MAKE(0xD,1,0,0,1)
#define CPUF_YmmSaveStateSize	CPUIDFIELD_MAKE(0xD,2,0,0,32)
#define CPUF_YmmSaveStateOffset	CPUIDFIELD_MAKE(0xD,2,1,0,32)
#define CPUF_LwpSaveStateSize	CPUIDFIELD_MAKE(0xD,62,0,0,32)
#define CPUF_LwpSaveStateOffset	CPUIDFIELD_MAKE(0xD,62,1,0,32)
#define CPUF_LFuncExt	CPUIDFIELD_MAKE(0x80000000,0,0,0,32)
#define CPUF_BrandId16	CPUIDFIELD_MAKE(0x80000001,0,1,0,16)
#define CPUF_PkgType	CPUIDFIELD_MAKE(0x80000001,0,1,28,4)
#define CPUF_LahfSahf	CPUIDFIELD_MAKE(0x80000001,0,2,0,1)
#define CPUF_CmpLegacy	CPUIDFIELD_MAKE(0x80000001,0,2,1,1)
#define CPUF_SVM	CPUIDFIELD_MAKE(0x80000001,0,2,2,1)
#define CPUF_ExtApicSpace	CPUIDFIELD_MAKE(0x80000001,0,2,3,1)
#define CPUF_AltMovCr8	CPUIDFIELD_MAKE(0x80000001,0,2,4,1)
#define CPUF_ABM	CPUIDFIELD_MAKE(0x80000001,0,2,5,1)
#define CPUF_SSE4A	CPUIDFIELD_MAKE(0x80000001,0,2,6,1)
#define CPUF_MisAlignSse	CPUIDFIELD_MAKE(0x80000001,0,2,7,1)
#define CPUF_3DNowPrefetch	CPUIDFIELD_MAKE(0x80000001,0,2,8,1)
#define CPUF_OSVW	CPUIDFIELD_MAKE(0x80000001,0,2,9,1)
#define CPUF_IBS	CPUIDFIELD_MAKE(0x80000001,0,2,10,1)
#define CPUF_XOP	CPUIDFIELD_MAKE(0x80000001,0,2,11,1)
#define CPUF_SKINIT	CPUIDFIELD_MAKE(0x80000001,0,2,12,1)
#define CPUF_WDT	CPUIDFIELD_MAKE(0x80000001,0,2,13,1)
#define CPUF_LWP	CPUIDFIELD_MAKE(0x80000001,0,2,15,1)
#define CPUF_FMA4	CPUIDFIELD_MAKE(0x80000001,0,2,16,1)
#define CPUF_BIT_NODEID	CPUIDFIELD_MAKE(0x80000001,0,2,19,1)
#define CPUF_TBM	CPUIDFIELD_MAKE(0x80000001,0,2,21,1)
#define CPUF_TopologyExtensions	CPUIDFIELD_MAKE(0x80000001,0,2,22,1)
#define CPUF_SYSCALL	CPUIDFIELD_MAKE(0x80000001,0,3,11,1)
#define CPUF_XD	CPUIDFIELD_MAKE(0x80000001,0,3,20,1)
#define CPUF_MmxExt	CPUIDFIELD_MAKE(0x80000001,0,3,22,1)
#define CPUF_FFXSR	CPUIDFIELD_MAKE(0x80000001,0,3,25,1)
#define CPUF_Page1GB	CPUIDFIELD_MAKE(0x80000001,0,3,26,1)
#define CPUF_RDTSCP	CPUIDFIELD_MAKE(0x80000001,0,3,27,1)
#define CPUF_LM	CPUIDFIELD_MAKE(0x80000001,0,3,29,1)
#define CPUF_3DNowExt	CPUIDFIELD_MAKE(0x80000001,0,3,30,1)
#define CPUF_3DNow	CPUIDFIELD_MAKE(0x80000001,0,3,31,1)
#define CPUF_L1ITlb2and4MSize	CPUIDFIELD_MAKE(0x80000005,0,0,0,8)
#define CPUF_L1ITlb2and4MAssoc	CPUIDFIELD_MAKE(0x80000005,0,0,8,8)
#define CPUF_L1DTlb2and4MSize	CPUIDFIELD_MAKE(0x80000005,0,0,16,8)
#define CPUF_L1DTlb2and4MAssoc	CPUIDFIELD_MAKE(0x80000005,0,0,24,8)
#define CPUF_L1ITlb4KSize	CPUIDFIELD_MAKE(0x80000005,0,1,0,8)
#define CPUF_L1ITlb4KAssoc	CPUIDFIELD_MAKE(0x80000005,0,1,8,8)
#define CPUF_L1DTlb4KSize	CPUIDFIELD_MAKE(0x80000005,0,1,16,8)
#define CPUF_L1DTlb4KAssoc	CPUIDFIELD_MAKE(0x80000005,0,1,24,8)
#define CPUF_L1DcLineSize	CPUIDFIELD_MAKE(0x80000005,0,2,0,8)
#define CPUF_L1DcLinesPerTag	CPUIDFIELD_MAKE(0x80000005,0,2,8,8)
#define CPUF_L1DcAssoc	CPUIDFIELD_MAKE(0x80000005,0,2,16,8)
#define CPUF_L1DcSize	CPUIDFIELD_MAKE(0x80000005,0,2,24,8)
#define CPUF_L1IcLineSize	CPUIDFIELD_MAKE(0x80000005,0,3,0,8)
#define CPUF_L1IcLinesPerTag	CPUIDFIELD_MAKE(0x80000005,0,3,8,8)
#define CPUF_L1IcAssoc	CPUIDFIELD_MAKE(0x80000005,0,3,16,8)
#define CPUF_L1IcSize	CPUIDFIELD_MAKE(0x80000005,0,3,24,8)
#define CPUF_L2ITlb2and4MSize	CPUIDFIELD_MAKE(0x80000006,0,0,0,12)
#define CPUF_L2ITlb2and4MAssoc	CPUIDFIELD_MAKE(0x80000006,0,0,12,4)
#define CPUF_L2DTlb2and4MSize	CPUIDFIELD_MAKE(0x80000006,0,0,16,12)
#define CPUF_L2DTlb2and4MAssoc	CPUIDFIELD_MAKE(0x80000006,0,0,28,4)
#define CPUF_L2ITlb4KSize	CPUIDFIELD_MAKE(0x80000006,0,1,0,12)
#define CPUF_L2ITlb4KAssoc	CPUIDFIELD_MAKE(0x80000006,0,1,12,4)
#define CPUF_L2DTlb4KSize	CPUIDFIELD_MAKE(0x80000006,0,1,16,12)
#define CPUF_L2DTlb4KAssoc	CPUIDFIELD_MAKE(0x80000006,0,1,28,4)
#define CPUF_L2LineSize	CPUIDFIELD_MAKE(0x80000006,0,2,0,8)
#define CPUF_L2LinesPerTag	CPUIDFIELD_MAKE(0x80000006,0,2,8,4)
#define CPUF_L2Assoc	CPUIDFIELD_MAKE(0x80000006,0,2,12,4)
#define CPUF_L2Size	CPUIDFIELD_MAKE(0x80000006,0,2,16,16)
#define CPUF_L3LineSize	CPUIDFIELD_MAKE(0x80000006,0,3,0,8)
#define CPUF_L3LinesPerTag	CPUIDFIELD_MAKE(0x80000006,0,3,8,4)
#define CPUF_L3Assoc	CPUIDFIELD_MAKE(0x80000006,0,3,12,4)
#define CPUF_L3Size	CPUIDFIELD_MAKE(0x80000006,0,3,18,14)
#define CPUF_TS	CPUIDFIELD_MAKE(0x80000007,0,3,0,1)
#define CPUF_FID	CPUIDFIELD_MAKE(0x80000007,0,3,1,1)
#define CPUF_VID	CPUIDFIELD_MAKE(0x80000007,0,3,2,1)
#define CPUF_TTP	CPUIDFIELD_MAKE(0x80000007,0,3,3,1)
#define CPUF_HTC	CPUIDFIELD_MAKE(0x80000007,0,3,4,1)
#define CPUF_100MHzSteps	CPUIDFIELD_MAKE(0x80000007,0,3,6,1)
#define CPUF_HwPstate	CPUIDFIELD_MAKE(0x80000007,0,3,7,1)
#define CPUF_TscInvariant	CPUIDFIELD_MAKE(0x80000007,0,3,8,1)
#define CPUF_CPB	CPUIDFIELD_MAKE(0x80000007,0,3,9,1)
#define CPUF_EffFreqRO	CPUIDFIELD_MAKE(0x80000007,0,3,10,1)
#define CPUF_PhysAddrSize	CPUIDFIELD_MAKE(0x80000008,0,0,0,8)
#define CPUF_LinAddrSize	CPUIDFIELD_MAKE(0x80000008,0,0,8,8)
#define CPUF_GuestPhysAddrSize	CPUIDFIELD_MAKE(0x80000008,0,0,16,8)
#define CPUF_NC	CPUIDFIELD_MAKE(0x80000008,0,2,0,8)
#define CPUF_ApicIdCoreIdSize	CPUIDFIELD_MAKE(0x80000008,0,2,12,4)
#define CPUF_SvmRev	CPUIDFIELD_MAKE(0x8000000A,0,0,0,8)
#define CPUF_NASID	CPUIDFIELD_MAKE(0x8000000A,0,1,0,32)
#define CPUF_NP	CPUIDFIELD_MAKE(0x8000000A,0,3,0,1)
#define CPUF_LbrVirt	CPUIDFIELD_MAKE(0x8000000A,0,3,1,1)
#define CPUF_SVML	CPUIDFIELD_MAKE(0x8000000A,0,3,2,1)
#define CPUF_NRIPS	CPUIDFIELD_MAKE(0x8000000A,0,3,3,1)
#define CPUF_TscRateMsr	CPUIDFIELD_MAKE(0x8000000A,0,3,4,1)
#define CPUF_VmcbClean	CPUIDFIELD_MAKE(0x8000000A,0,3,5,1)
#define CPUF_FlushByAsid	CPUIDFIELD_MAKE(0x8000000A,0,3,6,1)
#define CPUF_DecodeAssists	CPUIDFIELD_MAKE(0x8000000A,0,3,7,1)
#define CPUF_PauseFilter	CPUIDFIELD_MAKE(0x8000000A,0,3,10,1)
#define CPUF_PauseFilterThreshold	CPUIDFIELD_MAKE(0x8000000A,0,3,12,1)
#define CPUF_L1ITlb1GSize	CPUIDFIELD_MAKE(0x80000019,0,0,0,12)
#define CPUF_L1ITlb1GAssoc	CPUIDFIELD_MAKE(0x80000019,0,0,12,4)
#define CPUF_L1DTlb1GSize	CPUIDFIELD_MAKE(0x80000019,0,0,16,12)
#define CPUF_L1DTlb1GAssoc	CPUIDFIELD_MAKE(0x80000019,0,0,28,4)
#define CPUF_L2ITlb1GSize	CPUIDFIELD_MAKE(0x80000019,0,1,0,12)
#define CPUF_L2ITlb1GAssoc	CPUIDFIELD_MAKE(0x80000019,0,1,12,4)
#define CPUF_L2DTlb1GSize	CPUIDFIELD_MAKE(0x80000019,0,1,16,12)
#define CPUF_L2DTlb1GAssoc	CPUIDFIELD_MAKE(0x80000019,0,1,28,4)
#define CPUF_FP128	CPUIDFIELD_MAKE(0x8000001A,0,0,0,1)
#define CPUF_MOVU	CPUIDFIELD_MAKE(0x8000001A,0,0,1,1)
#define CPUF_IBSFFV	CPUIDFIELD_MAKE(0x8000001B,0,0,0,1)
#define CPUF_FetchSam	CPUIDFIELD_MAKE(0x8000001B,0,0,1,1)
#define CPUF_OpSam	CPUIDFIELD_MAKE(0x8000001B,0,0,2,1)
#define CPUF_RdWrOpCnt	CPUIDFIELD_MAKE(0x8000001B,0,0,3,1)
#define CPUF_OpCnt	CPUIDFIELD_MAKE(0x8000001B,0,0,4,1)
#define CPUF_BrnTrgt	CPUIDFIELD_MAKE(0x8000001B,0,0,5,1)
#define CPUF_OpCntExt	CPUIDFIELD_MAKE(0x8000001B,0,0,6,1)
#define CPUF_RipInvalidChk	CPUIDFIELD_MAKE(0x8000001B,0,0,7,1)
#define CPUF_LwpAvail	CPUIDFIELD_MAKE(0x8000001C,0,0,0,1)
#define CPUF_LwpVAL	CPUIDFIELD_MAKE(0x8000001C,0,0,1,1)
#define CPUF_LwpIRE	CPUIDFIELD_MAKE(0x8000001C,0,0,2,1)
#define CPUF_LwpBRE	CPUIDFIELD_MAKE(0x8000001C,0,0,3,1)
#define CPUF_LwpDME	CPUIDFIELD_MAKE(0x8000001C,0,0,4,1)
#define CPUF_LwpCNH	CPUIDFIELD_MAKE(0x8000001C,0,0,5,1)
#define CPUF_LwpRNH	CPUIDFIELD_MAKE(0x8000001C,0,0,6,1)
#define CPUF_LwpInt	CPUIDFIELD_MAKE(0x8000001C,0,0,31,1)
#define CPUF_LwpCbSize	CPUIDFIELD_MAKE(0x8000001C,0,1,0,8)
#define CPUF_LwpEventSize	CPUIDFIELD_MAKE(0x8000001C,0,1,8,8)
#define CPUF_LwpMaxEvents	CPUIDFIELD_MAKE(0x8000001C,0,1,16,8)
#define CPUF_LwpEventOffset	CPUIDFIELD_MAKE(0x8000001C,0,1,24,8)
#define CPUF_LwpLatencyMax	CPUIDFIELD_MAKE(0x8000001C,0,2,0,5)
#define CPUF_LwpDataAddress	CPUIDFIELD_MAKE(0x8000001C,0,2,5,1)
#define CPUF_LwpLatencyRnd	CPUIDFIELD_MAKE(0x8000001C,0,2,6,3)
#define CPUF_LwpVersion	CPUIDFIELD_MAKE(0x8000001C,0,2,9,7)
#define CPUF_LwpMinBufferSize	CPUIDFIELD_MAKE(0x8000001C,0,2,16,8)
#define CPUF_LwpBranchPrediction	CPUIDFIELD_MAKE(0x8000001C,0,2,28,1)
#define CPUF_LwpIpFiltering	CPUIDFIELD_MAKE(0x8000001C,0,2,29,1)
#define CPUF_LwpCacheLevels	CPUIDFIELD_MAKE(0x8000001C,0,2,30,1)
#define CPUF_LwpCacheLatency	CPUIDFIELD_MAKE(0x8000001C,0,2,31,1)
#define CPUF_D_LwpAvail	CPUIDFIELD_MAKE(0x8000001C,0,3,0,1)
#define CPUF_D_LwpVAL	CPUIDFIELD_MAKE(0x8000001C,0,3,1,1)
#define CPUF_D_LwpIRE	CPUIDFIELD_MAKE(0x8000001C,0,3,2,1)
#define CPUF_D_LwpBRE	CPUIDFIELD_MAKE(0x8000001C,0,3,3,1)
#define CPUF_D_LwpDME	CPUIDFIELD_MAKE(0x8000001C,0,3,4,1)
#define CPUF_D_LwpCNH	CPUIDFIELD_MAKE(0x8000001C,0,3,5,1)
#define CPUF_D_LwpRNH	CPUIDFIELD_MAKE(0x8000001C,0,3,6,1)
#define CPUF_D_LwpInt	CPUIDFIELD_MAKE(0x8000001C,0,3,31,1)
#define CPUF_CacheType	CPUIDFIELD_MAKE(0x8000001D,0,0,0,5)
#define CPUF_CacheLevel	CPUIDFIELD_MAKE(0x8000001D,0,0,5,3)
#define CPUF_SelfInitialization	CPUIDFIELD_MAKE(0x8000001D,0,0,8,1)
#define CPUF_FullyAssociative	CPUIDFIELD_MAKE(0x8000001D,0,0,9,1)
#define CPUF_NumSharingCache	CPUIDFIELD_MAKE(0x8000001D,0,0,14,12)
#define CPUF_CacheLineSize	CPUIDFIELD_MAKE(0x8000001D,0,1,0,12)
#define CPUF_CachePhysPartitions	CPUIDFIELD_MAKE(0x8000001D,0,1,12,10)
#define CPUF_CacheNumWays	CPUIDFIELD_MAKE(0x8000001D,0,1,22,10)
#define CPUF_CacheNumSets	CPUIDFIELD_MAKE(0x8000001D,0,2,0,32)
#define CPUF_WBINVD	CPUIDFIELD_MAKE(0x8000001D,0,3,0,1)
#define CPUF_CacheInclusive	CPUIDFIELD_MAKE(0x8000001D,0,3,1,1)
#define CPUF_ExtendedApicId	CPUIDFIELD_MAKE(0x8000001E,0,0,0,32)
#define CPUF_ComputeUnitId	CPUIDFIELD_MAKE(0x8000001E,0,1,0,8)
#define CPUF_CoresPerComputeUnit	CPUIDFIELD_MAKE(0x8000001E,0,1,8,2)
#define CPUF_NodeId	CPUIDFIELD_MAKE(0x8000001E,0,2,0,8)
#define CPUF_NodesPerProcessor	CPUIDFIELD_MAKE(0x8000001E,0,2,8,3)


// ȡ��λ��
#ifndef __GETBITS32
#define __GETBITS32(src,pos,len)	( ((src)>>(pos)) & (((UINT32)-1)>>(32-len)) )
#endif

// ����CPUIDFIELD�ӻ������л�ȡ�ֶ�.
inline UINT32	getcpuidfield_buf(const INT32 dwBuf[4], CPUIDFIELD cpuf)
{
	return __GETBITS32(dwBuf[CPUIDFIELD_REG(cpuf)], CPUIDFIELD_POS(cpuf), CPUIDFIELD_LEN(cpuf));
}

// ����CPUIDFIELD��ȡCPUID�ֶ�.
inline UINT32	getcpuidfield(CPUIDFIELD cpuf)
{
	INT32 dwBuf[4];
	__cpuidex(dwBuf, CPUIDFIELD_FID(cpuf), CPUIDFIELD_FIDSUB(cpuf));
	return getcpuidfield_buf(dwBuf, cpuf);
}



// SSEϵ��ָ���֧�ּ���. simd_sse_level �����ķ���ֵ.
#define SIMD_SSE_NONE	0	// ��֧��.
#define SIMD_SSE_1	1	// SSE
#define SIMD_SSE_2	2	// SSE2
#define SIMD_SSE_3	3	// SSE3
#define SIMD_SSE_3S	4	// SSSE3
#define SIMD_SSE_41	5	// SSE4.1
#define SIMD_SSE_42	6	// SSE4.2


// AVXϵ��ָ���֧�ּ���. simd_avx_level �����ķ���ֵ��
#define SIMD_AVX_NONE	0	// ��֧��
#define SIMD_AVX_1	1	// AVX
#define SIMD_AVX_2	2	// AVX2







// functions
int cpu_getvendor(char* pvendor);
int cpu_getbrand(char* pbrand);
int	simd_mmx(int* phwmmx);
int	simd_sse_level(int* phwsse);
int	simd_avx_level(int* phwavx);




typedef struct tagCPUIDINFO{
	INT32	fid;
	INT32	fidsub;
	union{
		INT32	dw[4];
		struct{
			INT32	_eax;
			INT32	_ebx;
			INT32	_ecx;
			INT32	_edx;
		};
	};
}CPUIDINFO;
typedef CPUIDINFO* LPCPUIDINFO;
typedef const CPUIDINFO* LPCCPUIDINFO;

#define MAX_CPUIDINFO	0x100	// CCPUID������ౣ�������CPUIDINFO��Ϣ��

#if defined __cplusplus
};
#endif

class CCPUID{
public:
	enum {
		CPUFDescLen = 292	// CPUIDFIELD������Ϣ����ĳ���.
	};
	static const CPUIDFIELDDESC	CPUFDesc[CPUFDescLen];	// CPUIDFIELD������Ϣ����.
	static const char*	CacheDesc[0x100];	// ����������Ϣ����.
	static const char*	SseNames[7];	// SSE���������.
	static const char*	AvxNames[3];	// AVX���������.
	
	CPUIDINFO	Info[MAX_CPUIDINFO+1];	// CPUID��Ϣ����.

	CCPUID();
	static CCPUID& cur() { if(0==_cur._InfoCount){ _cur.RefreshAll(); } return _cur; }	// ��ǰ��������CCPUID.
	int InfoCount() const { return _InfoCount; }	// Info�������Ч��Ŀ��.
	void RefreshInfo();	// ˢ����Ϣ.
	void RefreshProperty();	// ˢ������.
	void RefreshAll();	// ˢ������.
	LPCCPUIDINFO GetInfo(INT32 InfoType, INT32 ECXValue=0) const;	// ȡ����Ϣ.
	void GetData(INT32 CPUInfo[4], INT32 InfoType, INT32 ECXValue=0) const;	// ȡ������.
	UINT32 GetField(CPUIDFIELD cpuf) const;	// ȡ��CPUID�ֶ�
	// Property
	int LFuncStd() const { return _LFuncStd; }	// ���������ܺ�.
	int LFuncExt() const { return _LFuncExt; }	// ������չ���ܺ�.
	const char* Vendor() const { return _Vendor; }	// ����.
	const char* Brand() const { return _Brand; }	// �̱�.
	const char* BrandTrim() const { return _BrandTrim; }	// ȥ���׶��ո����̱�.
	int mmx() const { return _mmx; }	// ϵͳ֧��MMX.
	int hwmmx() const { return _hwmmx; }	// Ӳ��֧��MMX.
	int sse() const { return _sse; }	// ϵͳ֧��SSE.
	int hwsse() const { return _hwsse; }	// Ӳ��֧��SSE.
	int avx() const { return _avx; }	// ϵͳ֧��AVX.
	int hwavx() const { return _hwavx; }	// Ӳ��֧��AVX.

private:
	static CCPUID _cur;	// ��ǰ��������CCPUID. Ϊ�˷����ճ�ʹ��.

	int _InfoCount;	// Info�������Ч��Ŀ��.

	// Property
	int _LFuncStd;	// ���������ܺ�.
	int _LFuncExt;	// ������չ���ܺ�.
	char _Vendor[13];	// ����.
	char _Brand[49];	// �̱�.
	const char* _BrandTrim;	// ȥ���׶��ո����̱�.
	int _mmx;	// ϵͳ֧��MMX.
	int _hwmmx;	// Ӳ��֧��MMX.
	int _sse;	// ϵͳ֧��SSE.
	int _hwsse;	// Ӳ��֧��SSE.
	int _avx;	// ϵͳ֧��AVX.
	int _hwavx;	// Ӳ��֧��AVX.

	void RefreshInfo_Put(INT32 fid, INT32 fidsub, INT32 CPUInfo[4]);
	int	simd_mmx(int* phwmmx) const;
	int	simd_sse_level(int* phwsse) const;
	int	simd_avx_level(int* phwavx) const;

};


#endif	// #ifndef __CCPUID_H_INCLUDED
