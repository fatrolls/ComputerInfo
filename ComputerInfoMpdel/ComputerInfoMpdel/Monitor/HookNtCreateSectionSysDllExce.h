///*
//windows2003 x86/x64 window7 x86 windows2008 R2 x64≤‚ ‘Õ®π˝
//*/
//
//#include <ntddk.h>  
//#include <ntddkbd.h>
//#include <nt_help.h>  
//
//DRIVER_INITIALIZE DriverEntry;
//
//typedef struct _OBJECT_TYPE_INITIALIZER {
//	USHORT Length;
//	BOOLEAN UseDefaultObject;
//	BOOLEAN CaseInsensitive;
//#if WINVER>=0x0600  
//	ULONG ObjectTypeCode;
//#endif  
//	ULONG InvalidAttributes;
//	GENERIC_MAPPING GenericMapping;
//	ULONG ValidAccessMask;
//	BOOLEAN SecurityRequired;
//	BOOLEAN MaintainHandleCount;
//	BOOLEAN MaintainTypeList;
//	POOL_TYPE PoolType;
//	ULONG DefaultPagedPoolCharge;
//	ULONG DefaultNonPagedPoolCharge;
//	PVOID DumpProcedure;
//	PVOID OpenProcedure;
//	PVOID CloseProcedure;
//	PVOID DeleteProcedure;
//	PVOID ParseProcedure;
//	PVOID SecurityProcedure;
//	PVOID QueryNameProcedure;
//	PVOID OkayToCloseProcedure;
//} OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;
//
//typedef struct _OBJECT_TYPE {
//#if WINVER<0x0600  
//	ERESOURCE Mutex;
//#endif  
//	LIST_ENTRY TypeList;
//	UNICODE_STRING Name;            // Copy from object header for convenience  
//	PVOID DefaultObject;
//	ULONG Index;
//	ULONG TotalNumberOfObjects;
//	ULONG TotalNumberOfHandles;
//	ULONG HighWaterNumberOfObjects;
//	ULONG HighWaterNumberOfHandles;
//	OBJECT_TYPE_INITIALIZER TypeInfo;
//} OBJECT_TYPE, *POBJECT_TYPE;
//
//extern POBJECT_TYPE* MmSectionObjectType;
//PVOID pNtCreateSection = NULL;
//SYSTEM_MODULE_INFORMATION ntModInfo = { 0 };
//
//#pragma alloc_text(INIT, DriverEntry)  
//
//NTSTATUS DevicePassthrough(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
//{
//	NTSTATUS status = STATUS_SUCCESS;
//	PIO_STACK_LOCATION  irpSp;
//
//	irpSp = IoGetCurrentIrpStackLocation(Irp);
//	Irp->IoStatus.Status = status;
//	IoCompleteRequest(Irp, IO_NO_INCREMENT);
//	return status;
//}
//
//VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
//{
//	(*MmSectionObjectType)->TypeInfo.OpenProcedure = NULL;
//	KdPrint(("DriverUnload Done!\n"));
//}
//
//#if WINVER>=0x0600  
//NTSTATUS HookSectionOpen(
//	IN ULONG OpenReason,
//	IN ULONG AccessMode,
//	IN PEPROCESS Process OPTIONAL,
//	IN PVOID Object,
//	IN ACCESS_MASK* GrantedAccess,
//	IN ULONG HandleCount
//	)
//#else  
//NTSTATUS HookSectionOpen(
//	IN ULONG OpenReason,
//	IN PEPROCESS Process OPTIONAL,
//	IN PVOID Object,
//	IN ACCESS_MASK GrantedAccess,
//	IN ULONG HandleCount
//	)
//#endif  
//{
//	PVOID* esp = (PVOID*)&esp;
//	PVOID* esp_end = (PVOID*)((((DWORD64)esp >> 12) + 1) << 12);        //4k round up  
//	PVOID* p = esp;
//	ULONG SectionPageProtection, AllocationAttributes;
//	HANDLE FileHandle;
//	NTSTATUS Status;
//
//	/*
//	* do stack walk back to NtCreateSection function
//	*/
//	while (p < esp_end &&
//		(*p < pNtCreateSection ||
//		*p >(PVOID)((PBYTE)pNtCreateSection + 0x300)))
//		p++;
//
//	if (p >= esp_end){
//		//KdPrint(("no found NtCreateSection %p -> %p\n", esp, esp_end));  
//		return STATUS_SUCCESS;
//	}
//
//	//KdPrint(("%p HookSectionOpen-Object:%p esp:%p %p\n", pNtCreateSection, Object, esp, *p));  
//#ifdef _WIN64  
//	/*
//	* esp layout look likes[2003 X64 DUMP]:
//	fffff800`0104113d nt!KiSystemServiceCopyEnd+0x3 retaddr <-------call nt!NtCreateSection
//	fffffadf`f662ec00  00000000`00000000 param1
//	fffffadf`f662ec08  00000000`000f001f param2 DesiredAccess
//	fffffadf`f662ec10  00000000`00000000
//	fffffadf`f662ec18  00000000`00000000
//	fffffadf`f662ec20  00000100`00000010 SectionPageProtection
//	fffffadf`f662ec28  00000000`01000000 AllocationAttributes
//	fffffadf`f662ec30  00000000`0000054c FileHandle
//	* - ...
//	*/
//	p++;
//	/*
//	* search retaddr -> nt!KiSystemServiceCopyEnd
//	*/
//	while (p < esp_end &&
//		(*p < ntModInfo.ImageBase ||
//		*p >(PVOID)((PBYTE)ntModInfo.ImageBase + ntModInfo.ImageSize)))
//		p++;
//
//	if (p >= esp_end){
//		//KdPrint(("no found nt!KiSystemxxxx %p -> %p\n", esp, esp_end));  
//		return STATUS_SUCCESS;
//	}
//#else  
//	/* stack DUMP from 2003/x86
//	* ebp = p - 1
//	fa06f4d8  fa06f540
//	fa06f4dc  80908715 nt!NtCreateSection+0x15c
//	...
//	fa06f540  fa06f564
//	fa06f544  808234cb nt!KiFastCallEntry+0xf8
//	fa06f548  fa06f668 param1
//	*/
//	p = (PVOID*)*(p - 1);
//	p++;
//#endif  
//
//	SectionPageProtection = (ULONG)*(p + 5);
//	AllocationAttributes = (ULONG)*(p + 6);
//	FileHandle = *(p + 7);
//
//	//KdPrint(("%x %x %p\n", SectionPageProtection, AllocationAttributes, FileHandle));  
//
//	if (FileHandle
//		&& SectionPageProtection == PAGE_EXECUTE
//		&& (AllocationAttributes == SEC_IMAGE || AllocationAttributes == 0x100000)){
//		/* windows7 AllocationAttributes = 0x100000 to LoadDriver */
//		PFILE_OBJECT File;
//
//		Status = ObReferenceObjectByHandle(FileHandle,
//			0,
//			NULL,
//			KernelMode,
//			(PVOID *)&File,
//			NULL);
//
//		if (!NT_SUCCESS(Status)) {
//			return STATUS_SUCCESS;
//		}
//		KdPrint(("FileName:%wZ\n", &File->FileName));
//		ObDereferenceObject(File);
//	}
//
//	return STATUS_SUCCESS;
//}
//
//BOOL GetNtImgBase(PSYSTEM_MODULE_INFORMATION modInfo)
//{
//	PSYSMODULELIST sysModuleList = NULL;
//	ULONG size, i;
//
//	NtQuerySystemInformation(SystemModuleInformation, &size, 0, &size);
//	sysModuleList = ExAllocatePoolWithTag(PagedPool, size, 'hlpm');
//
//	if (sysModuleList){
//		NtQuerySystemInformation(SystemModuleInformation, sysModuleList, size, NULL);
//		/* nt module should be the first one */
//		*modInfo = *sysModuleList->Modules;
//		ExFreePool(sysModuleList);
//		return TRUE;
//	}
//	return FALSE;
//}
//
//NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
//{
//	DWORD i;
//	UNICODE_STRING sFuncName;
//
//	RtlInitUnicodeString(&sFuncName, L"NtCreateSection");
//	pNtCreateSection = MmGetSystemRoutineAddress(&sFuncName);
//
//	if (!GetNtImgBase(&ntModInfo)){
//		KdPrint(("EnumSysModule nt base failed!\n"));
//		return STATUS_UNSUCCESSFUL;
//	}
//
//	KdPrint(("nt:%p pNtCreateSection:%p\nMmSectionObjectType:%p %p %p\n",
//		ntModInfo.ImageBase,
//		pNtCreateSection,
//		*MmSectionObjectType,
//		(*MmSectionObjectType)->TypeInfo.OpenProcedure,
//		(*MmSectionObjectType)->TypeInfo.DeleteProcedure));
//
//	(*MmSectionObjectType)->TypeInfo.OpenProcedure = HookSectionOpen;
//
//	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
//		DriverObject->MajorFunction[i] = DevicePassthrough;
//
//	DriverObject->DriverUnload = DriverUnload;
//
//	return STATUS_SUCCESS;
//}