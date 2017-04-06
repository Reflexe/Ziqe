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
#include"dxd_base.h"
#include<wdf.h>


/// dynax driver framework kernel namespace
namespace dxd{
///////////////////////////////////////////////////////////////////////////////
/**
\brief indicates whether memory is mapped above 4GB boundary

- a serious OS should handle physical memory layout dependencies transparently.
Bills buggy experiment on the other hand requires the awareness of where memory
is physically mapped.
*/
bool below32(::MDL*mdl){
    unsigned int page=ADDRESS_AND_SIZE_TO_SPAN_PAGES(MmGetMdlVirtualAddress(mdl),MmGetMdlByteCount(mdl));
    ::PFN_NUMBER*pfn=MmGetMdlPfnArray(mdl);
    while(page--)
        if(pfn[page]>((1ull<<32)/PAGE_SIZE))
            return false;
    return true;
}

class KernelMemory {

};

class UserMemory {

};

///////////////////////////////////////////////////////////////////////////////
/**
\brief WDK: virtual kernel memory description

The dxd::memory is a kernel memory representation of the kernels virtual memory
description. It is an internally used wrapper to generate a virtual kernel memory
representation from a platform independent pointer:size description.
*/
template<scope scope_t=kernel>class memory{
    ///\cond
    ::MDL*mdl=nullptr;
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief private (disabled) copy constructor
    \todo check if copy constructor is needed.
    */
    memory(const memory&)=delete;
    ///\endcond
public:
    ///////////////////////////////////////////////////////////////////////////////
    void free();
    ///////////////////////////////////////////////////////////////////////////////
    memory()=default;
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief WDK memory constructor
    */
    memory(const void*data,///<source data pointer
        size_t size///<source data size
        ){
        initialize(data,size);
    }
    memory(const::MDL*mdl///<source MDL
        ){
        initialize(mdl);
    }
    ///////////////////////////////////////////////////////////////////////////////
    os_result initialize(const void*data,size_t size);
    /**
    in contrast to an IOKit MemoryDescriptor MDLs do not support reference counting.
    take care when creating a dxd::memory from a dxd::buffer: although the
    dxd::memory holds an MDL to the buffer the buffer itself is not retained after
    the dxd::buffer is released!
    */
    os_result initialize(const::MDL*mdl){
#pragma message(__FILE__" line " DX_STRINGIFY(__LINE__) ": bills MDLs do not support reference counting!")
        free();
        try(status=mdl?ok:not_initialized);
        return initialize(MmGetMdlVirtualAddress(mdl),MmGetMdlByteCount(mdl));
    catch:
        return status;
    }
    os_result initialize(const::MDL*mdl,size_t offset,size_t size){
        free();
        try(status=mdl?ok:not_initialized);
        return initialize(static_cast<char*>(MmGetMdlVirtualAddress(mdl))+offset,size);
    catch:
        return status;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief WDK: memory destructor
    */
    virtual~memory(){
        free();
    }
    ///////////////////////////////////////////////////////////////////////////////
    operator::MDL*()const{return mdl;}
    ///////////////////////////////////////////////////////////////////////////////
    os_result status=not_initialized;	///< status of memory object
};

///////////////////////////////////////////////////////////////////////////////
template<>os_result memory<kernel>::initialize(
    const void*data,///<virtual read only kernel base address of range to create memory descriptor from
    size_t size///<size of range to create memory descriptor from
    ){
    /// Allows re-initialization
    free();
    try(status=(::KeGetCurrentIrql()<=DISPATCH_LEVEL)?ok:interrupt_level_mismatch);
    try(status=(mdl=::IoAllocateMdl(
        const_cast<void*>(data),
        static_cast<::ULONG>(size),
        false,false,0))?ok:no_resources);
    ::MmBuildMdlForNonPagedPool(mdl);
    // flush for DMA
    KeFlushIoBuffers(mdl,true,true);
//	trace("  mdl:%p\n",__mdl);
catch:
    return status;
}
///////////////////////////////////////////////////////////////////////////////
template<>os_result memory<user>::initialize(
    const void*data,///<virtual read only user base address of range to create memory descriptor from
    size_t size///<size of range to create memory descriptor from
    ){
    /// Allows re-initialization
    free();
    try(status=(::KeGetCurrentIrql()<=APC_LEVEL)?ok:interrupt_level_mismatch);
    __try{
        // do not trust if originated in user space
        ::ProbeForWrite(const_cast<void*>(data),size,1);
    }__except(EXCEPTION_EXECUTE_HANDLER){
        try(status=::GetExceptionCode());
    }
    try(status=(mdl=::IoAllocateMdl(
        const_cast<void*>(data),
        static_cast<::ULONG>(size),
        false,false,nullptr))?ok:no_resources);
    // do not trust (especially if potentially originated in user space)
    __try{
        // wire down
        // Callers of MmProbeAndLockPages must be running at
        // IRQL <= APC_LEVEL for pageable addresses, or at
        // IRQL <= DISPATCH_LEVEL for nonpageable addresses.
        ::MmProbeAndLockPages(mdl,::KernelMode,::IoWriteAccess);
    }__except(EXCEPTION_EXECUTE_HANDLER){
        try(status=::GetExceptionCode());
    }
    // flush for DMA
    KeFlushIoBuffers(mdl,true,true);
//		trace("  mdl:%p\n",__mdl);
    return ok;
catch:
    // don't use free here: if there is an MDL lock page failed:
    if(mdl){
        // deletes MDL store
        ::IoFreeMdl(mdl);
        mdl=nullptr;
    }
    return status;
}
///////////////////////////////////////////////////////////////////////////////
template<>void memory<kernel>::free(){
    if(mdl){
        // flush for DMA
        KeFlushIoBuffers(mdl,false,true);
        // deletes MDL store
        ::IoFreeMdl(mdl);
        mdl=nullptr;
        status=not_initialized;
    }
}
///////////////////////////////////////////////////////////////////////////////
template<>void memory<user>::free(){
    if(mdl){
        if NT_SUCCESS(status){
            // flush for DMA
            KeFlushIoBuffers(mdl,false,true);
            // unwire
            ::MmUnlockPages(mdl);
        }
        // deletes MDL store
        ::IoFreeMdl(mdl);
        mdl=nullptr;
        status=not_initialized;
    }
}


#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4293) // '<<' : shift count negative or too big, undefined behavior
///////////////////////////////////////////////////////////////////////////////
/**
\brief generic WDF buffer class
*/
template<scope scope_t=kernel,unsigned int limit_t=64,unsigned int alignment_t=0>
class buffer:public memory<kernel>{
    typedef memory<kernel>super;
    ///////////////////////////////////////////////////////////////////////////////
    class physical_address{
        ::PHYSICAL_ADDRESS address;//union
    public:
        physical_address(int64 address64){address.QuadPart=address64;}
        ///////////////////////////////////////////////////////////////////////////////
        operator const::PHYSICAL_ADDRESS&()const{return address;}
    };
    void*vm=nullptr;///< kernel memory pointer to allocated space
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief private (disabled) copy constructor
    \todo check if copy constructor is needed.
    */
    buffer(const buffer&)=delete;
public:
    ///////////////////////////////////////////////////////////////////////////////
    void free(){
//		trace("  vm:%p\n",_vm);
        super::free();
        if(vm){
            if((limit_t==64)&&(alignment_t==0))
                ::ExFreePool(vm);
            else
                ::MmFreeContiguousMemory(vm);
            vm=nullptr;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief WDK buffer default constructor
    \todo Move public definition to a template interface.
    */
    buffer()=default;
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief WDK buffer constructor
    */
    buffer(size_t size///size to be alloced by buffer object
        ){
        initialize(size);
    }
    ///////////////////////////////////////////////////////////////////////////////
    os_result initialize(size_t size///size to be alloced by buffer object
        ){
        /// Allows re-initialization
        free();
        void*vm;
        try(status=(::KeGetCurrentIrql()<=DISPATCH_LEVEL)?ok:interrupt_level_mismatch);
        //trace("  limit:%llx align:%llx\n",(1ull<<limit_t)-1,alignment_t>0?1ull<<alignment_t:0);
        // Note: the physical address boundary parameter is different from the
        // alignment requirement from OSX. (The alignment on Windows is always pagesize)
        try(status=(vm=
            (limit_t==64)&&(alignment_t==0)?
            ::ExAllocatePoolWithTag(::NonPagedPool,size,'dxsm'):
            ::MmAllocateContiguousMemorySpecifyCache(size,
                physical_address(0ll),
                physical_address((1ull<<limit_t)-1),
                physical_address(0ll)/*physical_address(alignment_t>0?1ull<<alignment_t:0)*/,::MmCached))?ok:no_resources);
        try(status=super::initialize(vm,size));
//		trace("  vm:%p size:%d\n",vm,size);
        this->vm=vm;
        return ok;
    catch:;
        free();
        return status;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief WDK buffer memory destructor
    */
    virtual~buffer(){
        free();
    }
};
#pragma warning(pop)


///\cond
///////////////////////////////////////////////////////////////////////////////
template<typename vm_t>class __map{
protected:
    vm_t*_vm=nullptr;	///< virtual address mapping
public:
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief operator to use mapping like pointer
    */
    vm_t*operator->(){return _vm;}
    ///////////////////////////////////////////////////////////////////////////////
    operator vm_t*()const{return _vm;}
    ///////////////////////////////////////////////////////////////////////////////
    os_result status=not_initialized;	///< status of mapping object
};
///\endcond


///////////////////////////////////////////////////////////////////////////////
/**
\brief WDK memory mapping
*/
template<scope scope_t=kernel,typename vm_t=unsigned int>class map;

///////////////////////////////////////////////////////////////////////////////
/**
\brief WDK: user space memory mapping
*/
template<typename vm_t>class map<user,vm_t>:public __map<vm_t>{
    ///\cond
    ::MDL*mdl=nullptr;
    ///\endcond
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief private (disabled) copy constructor
    \todo check if copy constructor is needed.
    */
    map(const map&)=delete;
public:
    ///////////////////////////////////////////////////////////////////////////////
    map()=default;
    ///////////////////////////////////////////////////////////////////////////////
    map(::MDL*mdl///MDL describing user space mapping to be created
        )
    {
        initialize(mdl);
    }
    ///////////////////////////////////////////////////////////////////////////////
    os_result initialize(::MDL*mdl///MDL describing user space mapping to be created
        ){
        free();
        // If AccessMode is UserMode and the specified pages cannot be mapped, the
        // routine raises an exception. Callers that specify UserMode must wrap the
        // call to MmMapLockedPagesSpecifyCache in a try/except block
        // If AccessMode is UserMode, the caller must be running at IRQL <= APC_LEVEL.
        // If AccessMode is KernelMode, the caller must be running at IRQL <= DISPATCH_LEVEL.
        try(status=(::KeGetCurrentIrql()<=APC_LEVEL)?ok:interrupt_level_mismatch);
        try(status=mdl?ok:invalid);
        __try{
            _vm=static_cast<vm_t*>(::MmMapLockedPagesSpecifyCache(mdl,
                                                                  ::UserMode,
                                                                  ::MmCached,
                                                                  nullptr,
                                                                  FALSE,
                                                                  ::NormalPagePriority));
            this->mdl=mdl;
        }
        __except(EXCEPTION_EXECUTE_HANDLER){
            try(status=::GetExceptionCode());
        }
    catch:
        return status;
    }
    ///////////////////////////////////////////////////////////////////////////////
    void free(){
        if(_vm&&mdl)
            ::MmUnmapLockedPages(_vm,mdl);
        _vm=nullptr;
        mdl=nullptr;
        status=not_initialized;
    }
    ///////////////////////////////////////////////////////////////////////////////
    ~map(){
        free();
    }
};


///////////////////////////////////////////////////////////////////////////////
/**
\brief WDK: kernel space memory mapping

this class does nothing; it is required for IOKit compatibility only
*/
template<typename vm_t>class map<kernel,vm_t>:public __map<vm_t>{
    ///////////////////////////////////////////////////////////////////////////////
    /**
    \brief private (disabled) copy constructor
    \todo check if copy constructor is needed.
    */
    map(const map&)=delete;
public:
    ///////////////////////////////////////////////////////////////////////////////
    map()=default;
    ///////////////////////////////////////////////////////////////////////////////
    map(const::MDL*mdl///MDL describing kernel space mapping to be created
        ){
        initialize(mdl);
    }
    ///////////////////////////////////////////////////////////////////////////////
    os_result initialize(const::MDL*mdl///kernel space mapping to be created
        ){
        _vm=static_cast<vm_t*>(MmGetMdlVirtualAddress(mdl));
//		trace("  vm:%p\n",_vm);
        return status=_vm?ok:not_initialized;
    }
    ///////////////////////////////////////////////////////////////////////////////
    void free(){
        _vm=nullptr;
        status=not_initialized;
    }
};
}//namespace dxd
