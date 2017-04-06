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
#include"dxd.h"
#include<IOKit/IOLib.h>
#include<IOKit/IOService.h>
// for Availability.h to work with the 10.5 SDK
// cd [XcodeRoot]/SDKs/MacOSX10.5.sdk/System/Library/Frameworks/Kernel.framework/Versions/A/Headers
// sudo ln -s ../../../../../../../usr/include/Availability.h Availability.h
// sudo ln -s ../../../../../../../usr/include/AvailabilityInternal.h AvailabilityInternal.h
#include<Availability.h>


///\cond
///////////////////////////////////////////////////////////////////////////////
#ifdef DX_DEBUG
#define trace(...) IOLog("dx|" __VA_ARGS__)
class at_dynax_dxd_trace{
	const char*exit;
public:
	at_dynax_dxd_trace(const char*entry,const char*exit):exit(exit){
		trace("\033[1m""%s\033[0m [%x] {\n",entry,
			static_cast<unsigned int>(reinterpret_cast<uint64_t>(current_thread)));
	}
	~at_dynax_dxd_trace(){
		trace("  } %s\n",exit);
	}
};
#define $ ::at_dynax_dxd_trace at_dynax_dxd_trace(__PRETTY_FUNCTION__,__FUNCTION__);
#define dxd_assert try
#else
#define trace(...) {}
#define $
#define dxd_assert
#endif//DEBUG
#define log IOLog
#define warnlog IOLog
#define errorlog IOLog

// this is a tribute to the kernel restriction of not providing run time type 
// information and contemporary exception handling, nor enough stack for 
// implementing such. BTW, this is 2009.
#define try(...) at_dynax_dxd_try((__VA_ARGS__))
#define at_dynax_dxd_try(test){\
	::IOReturn at_dynax_dxd_status=test;\
	if(at_dynax_dxd_status){\
		trace("\n\033[31m%s(%d):\033[0m\n  \033[31m%s:\033[0m\n  \033[31m%s failed with \"%s\"[%x]\033[0m\n",\
			__FILE__,__LINE__,__PRETTY_FUNCTION__,#test,\
			::IOService::getResourceService()->stringFromReturn(at_dynax_dxd_status),at_dynax_dxd_status);\
		goto catch;}}
#define catch at_dynax_dxd_catch


// undefine dxd:: namespace - OSX dynamic kernel linker doesn't support 
// OSObject derived classes to be in namespaces
// http://developer.apple.com/mac/library/documentation/DeviceDrivers/Conceptual/IOKitFundamentals/Features/Features.html#//apple_ref/doc/uid/TP0000012-DontLinkElementID_4
#define dxd
///\endcond

#ifdef __doxygen__
/// dxd - dynax driver framework
namespace dxd{
#endif//__doxygen__
///////////////////////////////////////////////////////////////////////////////
typedef long long int64;
typedef unsigned long long uint64;


///////////////////////////////////////////////////////////////////////////////
typedef::IOReturn os_result; ///< defining platform abstract type os_result for IOKit driver
enum{
	ok=kIOReturnSuccess,
	not_implemented=kIOReturnUnsupported,
	invalid=kIOReturnBadArgument,
	device_error=kIOReturnDeviceError,
	no_resources=kIOReturnNoResources,
	not_initialized=kIOReturnNotReady,
	timeout=kIOReturnTimeout,
	invalid_size=kIOReturnNoSpace,
	aborted=kIOReturnAborted,
	exclusive=kIOReturnExclusiveAccess,
	device_removed=kIOReturnNotAttached,
	closed=kIOReturnNotOpen,
};

static inline const char*describe(os_result status){
	return::IOService::getResourceService()->stringFromReturn(status);
}

union fourchar_string{
	char c_str[];
	uint64 cast;
};
static inline fourchar_string to(unsigned int fourchar){
	fourchar_string result;
	unsigned int target_idx=0;
	for(unsigned int source_idx=sizeof(fourchar);source_idx--;){
		const char nib=reinterpret_cast<const char*>(&fourchar)[source_idx];
		if(nib>0x1f)
			result.c_str[target_idx++]=nib;
		else if(nib>0){
			result.c_str[target_idx++]=nib/10;
			result.c_str[target_idx++]=nib%10;
		}
	}
	result.c_str[target_idx++]=0;
	return result;
}


/// kernel IOCTLs
namespace ioctl{
static inline fourchar_string to(unsigned int ioctl){return dxd::to(ioctl);}
///////////////////////////////////////////////////////////////////////////////
/**
\brief kernel-only streaming IOCTLs

see also dx::ioctls::stream for shared kernel/user space IOCTLs
*/
namespace stream{
enum ioctl{
	open='OPEN',	///<open generic streaming pin from kernel
	//close='CLSE',	///<close generic streaming pin from kernel
	start='STRT',	///<start generic streaming pin from kernel
	stop='STOP',	///<stop generic streaming pin from kernel
};
}//namespace stream
}//namespace ioctl


///////////////////////////////////////////////////////////////////////////////
typedef::IOLock*os_event;///<platform abstract type os_event for IOKit driver


///////////////////////////////////////////////////////////////////////////////
#ifdef __ppc__
#define sync() asm("eieio")
#else
#define sync() {}
#endif//__ppc__

/*
///////////////////////////////////////////////////////////////////////////////
template<class object_t>
class prepare{
	object_t*object;
public:
	///////////////////////////////////////////////////////////////////////////////
	prepare(object_t*object):object(0),status(not_initilialized){
		try(status=object->prepare())
		this->object=object;
	catch:;
	}
	///////////////////////////////////////////////////////////////////////////////
	~prepare(){
		if(object)
			object->complete();
	}
	///////////////////////////////////////////////////////////////////////////////
	os_result status;
};
*/
///////////////////////////////////////////////////////////////////////////////
template<class object_t,class service_t>
class open{
	object_t*object;
	service_t*service;
public:
	///////////////////////////////////////////////////////////////////////////////
	open(object_t*object,service_t*service):object(0),service(service),status(not_initialized){
		if(object)
			try(status=object->open(service)?ok:device_error);
		this->object=object;
		catch:;
	}
	///////////////////////////////////////////////////////////////////////////////
	~open(){
		if(object)
			object->close(service);
	}
	///////////////////////////////////////////////////////////////////////////////
	os_result status;
};
	

///////////////////////////////////////////////////////////////////////////////
template<typename vm_t>class array{
	vm_t*_vm;
public:
	array(int elements=1):_vm(0),status(not_initialized){
		try(status=(_vm=new vm_t[elements])?ok:no_resources);
	catch:;
	}
	operator vm_t*()const{return _vm;}
	~array(){
		delete[]_vm;
	}
	os_result status;
};
#ifdef __doxygen__
}//namespace dxd
#endif//__doxygen__
