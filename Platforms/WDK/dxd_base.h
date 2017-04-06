///////////////////////////////////////////////////////////////////////////////
//
//	(c) dynamic acoustics e.U. 2009
//	dxd - dynax driver framework
//
///////////////////////////////////////////////////////////////////////////////
/*
The License Notice below must appear in each file of the Source Code of any
copy You distribute of the Licensed Software or any Extensions thereto:

Unless explicitly acquired and licensed from dynamic acoustics e.U. under another
license, the contents of this file are subject to the Reciprocal Public
License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
and You may not copy or use this file in either source code or executable
form, except in compliance with the terms and conditions of the RPL.

All software distributed under the RPL is provided strictly on an "AS
IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND
LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific
language governing rights and limitations under the RPL.
*/
#pragma once
#pragma warning(disable:4100) // unreferenced formal parameter
#pragma warning(disable:4512) // assignment operator could not be generated
#pragma warning(disable:4595) // 'operator new': non-member operator new or delete functions may not be declared inline
#pragma warning(disable:4706) // assignment within conditional expression

#include"dxd.h"
#include<ntddk.h>
#include"dx_types.h"

///////////////////////////////////////////////////////////////////////////////
//
///\cond
#ifdef DX_DEBUG
#define trace(...) DbgPrint("dx|" __VA_ARGS__)

class at_dynax_dxd_trace{
	const char*msg;
public:
	at_dynax_dxd_trace(const char*msg):msg(msg){
		trace("%s [irql:%d] {\n",msg,::KeGetCurrentIrql());
	}
	~at_dynax_dxd_trace(){
		trace("  } %s\n",msg);
	}
};
#define $ at_dynax_dxd_trace __at_dynax_dxd_trace(__FUNCTION__);
#define dxd_assert try
#else
#define trace(...) {}
#define $
#define dxd_assert
#endif//DX_DEBUG

///////////////////////////////////////////////////////////////////////////////
inline void*__cdecl operator new(size_t size){return((::KeGetCurrentIrql()<=DISPATCH_LEVEL)?::ExAllocatePoolWithTag(NonPagedPool,size,'dxd'):nullptr);}
inline void*__cdecl operator new[](size_t size){ return((::KeGetCurrentIrql()<=DISPATCH_LEVEL)?::ExAllocatePoolWithTag(NonPagedPool,size,'dxd'):nullptr);}
inline void __cdecl operator delete(void*object){if(object)::ExFreePool(object);}
inline void __cdecl operator delete[](void*object){if(object)::ExFreePool(object);}
inline void __cdecl operator delete(void*object,size_t){if(object)::ExFreePool(object);}
inline void __cdecl operator delete[](void*object,size_t){if(object)::ExFreePool(object);}
///\endcond


///////////////////////////////////////////////////////////////////////////////
#define sync() {}
#define strcasecmp _stricmp

#ifndef offsetof
#define offsetof(type,member)((size_t)(&((type*)0)->member))
#endif//offsetof

/// dxd - dynax driver framework
namespace dxd{
///////////////////////////////////////////////////////////////////////////////
/**
\brief windows kernel logging
*/
void log(const char*fmt...);
void warnlog(const char*fmt...);
void errorlog(const char*fmt...);
}//namespace dxd


// this is a tribute to the kernel restriction of not providing run time type 
// information and contemporary exception handling, nor enough stack for 
// implementing such. BTW, this was 2009.
#define try(...) at_dynax_dxd_try((__VA_ARGS__))
#define at_dynax_dxd_try(test){\
	::NTSTATUS at_dynax_dxd_status=(test);\
	if NT_ERROR(at_dynax_dxd_status){\
		dxd::errorlog("%s(%d) %s[%x,irql:%d]:\n  %s\n",\
			__FILE__,__LINE__,__FUNCTION__,at_dynax_dxd_status,::KeGetCurrentIrql(),#test);\
		goto catch;}}
#define catch _catch


/// dxd - dynax driver framework
namespace dxd{
///////////////////////////////////////////////////////////////////////////////
typedef __int64 int64;
typedef unsigned __int64 uint64;


///////////////////////////////////////////////////////////////////////////////
typedef::NTSTATUS os_result;///<platform abstract type os_result for WDK driver
enum{
//enum class os_result: ::NTSTATUS{
	ok=STATUS_SUCCESS,
	not_implemented=STATUS_NOT_IMPLEMENTED,
	invalid=STATUS_INVALID_PARAMETER,
	device_error=STATUS_IO_DEVICE_ERROR,
	no_resources=STATUS_INSUFFICIENT_RESOURCES,
	not_initialized=STATUS_REINITIALIZATION_NEEDED,
	interrupt_level_mismatch=STATUS_INVALID_LEVEL,
	timeout=STATUS_IO_TIMEOUT,
//	aborted=STATUS_REQUEST_ABORTED,
	invalid_size=STATUS_INVALID_BUFFER_SIZE,
	not_aligned=STATUS_DATATYPE_MISALIGNMENT_ERROR,
	aborted=STATUS_CANCELLED,
	failed=STATUS_UNSUCCESSFUL,
};

static inline const char*describe(os_result status){
	return"n/a";
}

/// kernel IOCTLs
namespace ioctl{
enum ioctl{
};
///////////////////////////////////////////////////////////////////////////////
/**
\brief kernel-only streaming IOCTLs

see also dx::ioctls::stream for shared kernel/user space IOCTLs
*/
namespace stream{
enum ioctl{
	open=	DX_IOCTL('S',0),///<open generic streaming pin from kernel
//	close=	DX_IOCTL('S',1),///<close generic streaming pin from kernel
	start=	DX_IOCTL('S',2),///<start generic streaming pin from kernel
	stop=	DX_IOCTL('S',3),///<stop generic streaming pin from kernel
};
}//namespace stream
}//namespace ioctl


///////////////////////////////////////////////////////////////////////////////
typedef::KEVENT*os_event;///<platform abstract type os_event for WDK driver


///////////////////////////////////////////////////////////////////////////////
///\todo guid: find appropriate place
class guid:public::GUID{
public:
	guid(const::GUID&guid): ::GUID(guid),status(ok){}
	guid(
		unsigned long data1,
		unsigned short data2,
		unsigned short data3,
		uint64 data4):status(ok){
		Data1=data1;
		Data2=data2;
		Data3=data3;
		*(uint64*)Data4=data4;
	}
	guid(const char fill):status(ok){
		::memset(static_cast<::GUID*>(this),fill,sizeof(::GUID));}
	guid(){
		::memset(static_cast<::GUID*>(this),0,sizeof(::GUID));
		while((status=::ExUuidCreate(this))==STATUS_RETRY);
//			if(::KeGetCurrentIrql()<=PASSIVE_LEVEL)
//				delay(msec);///\todo: requires include<dxd_sync.h>
		try(status);
		return;
	catch:
		trace("%s: "DX_STRINGIFY(DX_GUID_FORMAT)" status:%x\n",__FUNCTION__,DX_GUID_SPLIT(*this),status);
	}
	///////////////////////////////////////////////////////////////////////////////
	/**
	\brief constructs guid from ::UNICODE_STRING

	format is: {12345678-1234-1234-1234-123456789012}
	*/
	guid(const::UNICODE_STRING&guid){
		::memset(static_cast<::GUID*>(this),0,sizeof(::GUID));
		try(status=(::KeGetCurrentIrql()<=PASSIVE_LEVEL)?ok:interrupt_level_mismatch);
		try(status=::RtlGUIDFromString(&guid,this));
		return;
	catch:
		trace("%s: %S->"DX_STRINGIFY(DX_GUID_FORMAT)" status:%x\n",__FUNCTION__,guid.Buffer,DX_GUID_SPLIT(*this),status);
	}
/*	operator wchar_t*()const{
		wchar_t*result;
		try((result=new wchar_t[40])?ok:no_resources);
		try(::RtlStringCbPrintfW(result,sizeof(wchar_t[40]),DX_LSTRINGIFY(DX_GUID_FORMAT),DX_GUID_SPLIT(*this)));
		return result;
	catch:;
		return L"";
	}
*/	///////////////////////////////////////////////////////////////////////////////
	os_result status;
};


///////////////////////////////////////////////////////////////////////////////
enum class synchrony{
	sync,
	async
};
enum direction{
	none,
	in,
	out,
	both,
};


///////////////////////////////////////////////////////////////////////////////
/**
\brief Main driver entry

DriverEntry is the first driver-supplied routine that is called after a 
driver is loaded. It is responsible for initializing the driver.

This is the main WDF managed driver entry; the first call to the driver will
end up here. from here it is the responisibilty of the driver to register at
least a callback for the case the I/O manager matches a device to this driver
and adds the FDO to the driver stack.

The WDF framework is called with an initialized WDF_DRIVER_CONFIG containing
the EvtDeviceAdd callback.

for further reference see:
http://msdn.microsoft.com/en-us/library/aa491316.aspx
*/
extern::DRIVER_OBJECT*wdm_driver;
template<class driver_t>::NTSTATUS create(
	::DRIVER_OBJECT*wdm_driver,	///< A pointer to a DRIVER_OBJECT structure that represents the driver's WDM driver object.
    ::UNICODE_STRING*registry,	///< A pointer to a UNICODE_STRING structure that specifies the path to the driver's Parameters key in the registry.
	wchar_t*name=L"",			///< An internal unique name
	dxd::guid guid=0			///< GUID identifying the kernel driver interface
	){
//	trace("DBGVIEWCLEAR\n");
	dxd::wdm_driver=wdm_driver;
	driver_t*driver=new driver_t(wdm_driver,registry,name,guid);
	if(driver){
		::NTSTATUS status=driver->status;
		if NT_ERROR(status)
			delete driver;
//		trace(__FUNCTION__" status:%x\n",status);
		return status;
	}
	else
		return STATUS_INSUFFICIENT_RESOURCES;
}
}//namespace dxd
