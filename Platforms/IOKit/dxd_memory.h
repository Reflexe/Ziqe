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
#include<IOKit/IOBufferMemoryDescriptor.h>
#include<IOKit/IOSubMemoryDescriptor.h>


#ifdef __doxygen__
/// dynax driver framework kernel namespace
namespace dxd{
#endif//__doxygen__
///////////////////////////////////////////////////////////////////////////////
/**
\brief direction of memory operation
*/
enum direction{
        in=::kIODirectionIn,
        out=::kIODirectionOut,
        both=::kIODirectionInOut,
};


///////////////////////////////////////////////////////////////////////////////
/**
\brief virtual kernel memory description

The dxd::memory is a kernel memory representation of the kernels virtual memory
description. It is an internally used wrapper to generate a virtual kernel memory
representation from a platform independent pointer:size description.
*/
template<scope scope_t=kernel>
class memory{
        memory(const memory&)=delete;
protected:
        ///\cond
        ::IOMemoryDescriptor*iokit_memory_descriptor=nullptr;
        ///\endcond
public:
        ///////////////////////////////////////////////////////////////////////////////
        os_result status=not_initialized;///<IOKit memory status
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit default constructor

        Use this in conjunction with initialize()
        */
        memory()=default;
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit class constructor
        */
        memory(
                const void*data,///<virtual base address of range to create memory descriptor from
                size_t size///<size of range to create memory descriptor from
                ){
                /// Calls initialize() member.
                initialize(data,size);
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit memory initializer
        */
        os_result initialize(
                const void*data,///<virtual base address of range to create memory descriptor from
                size_t size///<size of range to create memory descriptor from
                );
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit memory initializer
        */
        os_result initialize(
                void*data,///<virtual base address of range to create memory descriptor from
                size_t size///<size of range to create memory descriptor from
                );
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit memory initializer
        */
        os_result initialize(
                ::IOMemoryDescriptor*iokit_memory_descriptor
                ){
                free();
                this->iokit_memory_descriptor=iokit_memory_descriptor;
                if(iokit_memory_descriptor){
                        iokit_memory_descriptor->retain();
                        try(status=iokit_memory_descriptor->prepare());
                }
        catch:;
                return status;
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit memory initializer
        */
        os_result initialize(
                ::IOMemoryDescriptor*iokit_memory_descriptor,
                size_t offset,
                size_t size
                ){
                free();
                try(status=(this->iokit_memory_descriptor=
                        ::IOSubMemoryDescriptor::withSubRange(
                                iokit_memory_descriptor,offset,size,iokit_memory_descriptor->getDirection()))?ok:no_resources);
                iokit_memory_descriptor->retain();
                try(status=iokit_memory_descriptor->prepare());
        catch:;
                return status;
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit memory free
        */
        void free(){
                /// Checks the underlaying OS memory descriptor,
                if(iokit_memory_descriptor){
                        /// synchronizes and unwires it from memory and
                        if(!status)
                                iokit_memory_descriptor->complete();
                        /// releases the memory descriptor.
                        iokit_memory_descriptor->release();
                        iokit_memory_descriptor=nullptr;
                }
                status=not_initialized;
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit memory size
        */
        size_t size()const{return iokit_memory_descriptor?iokit_memory_descriptor->getLength():0;}
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit memory destructor
        */
        ~memory(){
                /// Calls free() member.
                free();
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit IOMemoryDescriptor* implicit cast
        */
        operator const::IOMemoryDescriptor*()const{return iokit_memory_descriptor;}
        operator::IOMemoryDescriptor*(){return iokit_memory_descriptor;}
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit physical address (only relevant for contigous memory)
        */
        operator dxd::uint64()const{return iokit_memory_descriptor->getPhysicalAddress();}
};

///////////////////////////////////////////////////////////////////////////////
/**
\brief IOKit memory initializer from kernel virtual addresses
*/
template<>os_result memory<kernel>::initialize(
        void*data,///<virtual kernel base address of range to create memory descriptor from
        size_t size///<size of range to create memory descriptor from
        ){
        /// Allows re-initialization.
        free();
        //trace("  data:%p size:%d\n",data,size);
        /// Creates OS memory descriptor from kernel space data,size and
        try(status=(iokit_memory_descriptor=::IOMemoryDescriptor::withAddress(data,size,::kIODirectionInOut))?ok:no_resources);
        /// wires it down and synchronizes it.
        try(status=iokit_memory_descriptor->prepare());
        //trace("  data:%p size:%x(%x)",data,iokit_memory_descriptor->getLength(),size);
catch:;
        return status;
}
///////////////////////////////////////////////////////////////////////////////
/**
\brief IOKit memory initializer from kernel virtual addresses
*/
template<>os_result memory<kernel>::initialize(
        const void*data,///<virtual read only kernel base address of range to create memory descriptor from
        size_t size///<size of range to create memory descriptor from
        ){
        /// Allows re-initialization.
        free();
        //trace("  data:%p size:%d\n",data,size);
        /// Creates OS memory descriptor from kernel space data,size and
        try(status=(iokit_memory_descriptor=::IOMemoryDescriptor::withAddress(const_cast<void*>(data),size,::kIODirectionOut))?ok:no_resources);
        /// wires it down and synchronizes it.
        try(status=iokit_memory_descriptor->prepare());
        //trace("  data:%p size:%x(%x)",data,iokit_memory_descriptor->getLength(),size);
catch:;
        return status;
}
///////////////////////////////////////////////////////////////////////////////
/**
\brief IOKit memory initializer from user space virtual addresses
*/
template<>os_result memory<user>::initialize(
        void*data,///<virtual user space base address of range to create memory descriptor from
        size_t size///<size of range to create memory descriptor from
        ){
        /// Allows re-initialization.
        free();
        /// Creates OS memory descriptor from user space data,size and
        try(status=(iokit_memory_descriptor=::IOMemoryDescriptor::withAddressRange(
                (mach_vm_address_t)data,(mach_vm_size_t)size,
                ::kIODirectionInOut,current_task()))?ok:no_resources);
        /// wires it down and syschronizes it.
        try(status=iokit_memory_descriptor->prepare());
catch:;
        return status;
}
///////////////////////////////////////////////////////////////////////////////
/**
\brief IOKit memory initializer from user space virtual addresses
*/
template<>os_result memory<user>::initialize(
        const void*data,///<virtual read only user space base address of range to create memory descriptor from
        size_t size///<size of range to create memory descriptor from
        ){
        /// Allows re-initialization.
        free();
        /// Creates OS memory descriptor from user space data,size and
        try(status=(iokit_memory_descriptor=::IOMemoryDescriptor::withAddressRange(
                (mach_vm_address_t)data,(mach_vm_size_t)size,
                ::kIODirectionOut,current_task()))?ok:no_resources);
        /// wires it down and syschronizes it.
        try(status=iokit_memory_descriptor->prepare());
catch:;
        return status;
}


///\cond
///////////////////////////////////////////////////////////////////////////////
template<typename vm_t>class __map{
protected:
        ::IOMemoryMap*iokit_memory_map=nullptr;
        vm_t*_vm=nullptr;///< kernel virtual address mapping
        ///////////////////////////////////////////////////////////////////////////////
        __map()=default;
        ~__map(){
                free();
        }
public:
        ///////////////////////////////////////////////////////////////////////////////
        os_result status=not_initialized;///< status of mapping object
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief kernel map free
        */
        void free(){
                /// Checks if kernel mapping already exists and
                if(iokit_memory_map){
                        /// does not call unmap(): IOMemoryMap instances will unmap themselves
                        /// upon free, ie. when the last client with a reference calls release.
                        /// unmap() forces the IOMemoryMap to destroy the mapping it represents,
                        /// regardless of the number of clients.
                        /// releases the map object.
                        iokit_memory_map->release();
                        iokit_memory_map=nullptr;
                }
                status=not_initialized;
                _vm=nullptr;
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief kernel mapping size
        */
        size_t size()const{return iokit_memory_map?iokit_memory_map->getLength():0;}
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief operator to use mapping like pointer
        */
        const vm_t*operator->()const{return _vm;}
        vm_t*operator->(){return _vm;}
        ///////////////////////////////////////////////////////////////////////////////
        operator const vm_t*()const{return _vm;}
        operator vm_t*()const{return _vm;}
        ///////////////////////////////////////////////////////////////////////////////
        operator const::IOMemoryMap*()const{return iokit_memory_map;}
        operator::IOMemoryMap*(){return iokit_memory_map;}
};
///\endcond


///////////////////////////////////////////////////////////////////////////////
/**
\brief memory mapping into kernel space
*/
#pragma mark -
#pragma mark map<kernel>
template<scope scope_t=kernel,typename vm_t=unsigned int>class map;
template<typename vm_t>class map<kernel,vm_t>:public __map<vm_t>{
        map(const map&)=delete;
public:
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit default mapping constructor
        */
        map()=default;
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit mapping constructor
        */
        map(
                ::IOMemoryDescriptor*iokit_memory_descriptor///<IOMemoryDescriptor to be mapped into the kernel address space
                ){
                initialize(iokit_memory_descriptor);
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit kernel map initializer

        map<kernel> maps an IOMemoryDescriptor (typically cast of a dxd::memory or
        buffer class) into the kernel address space.
        */
        os_result initialize(
                ::IOMemoryDescriptor*iokit_memory_descriptor///<IOMemoryDescriptor to be mapped into the kernel address space
                ){
                /// Allows re-initialization.
                this->free();
                /// Checks buffer object,
                try(this->status=iokit_memory_descriptor?ok:not_initialized);
                /// maps the buffer into the kernel
                try(this->status=(this->iokit_memory_map=iokit_memory_descriptor->map())?ok:not_initialized);
                /// and obtains a kernel virtual address.
                try(this->status=(this->_vm=(vm_t*)this->iokit_memory_map->getVirtualAddress())?ok:not_initialized);
//		trace("  this:%p iokit:%p vm:%p\n",this,this->iokit_memory_map,this->vm);
        catch:
                return this->status;
        }
};


///////////////////////////////////////////////////////////////////////////////
/**
\brief memory mapping into user space
*/
#pragma mark -
#pragma mark map<user>
template<typename vm_t>class map<user,vm_t>:public __map<vm_t>{
        map(const map&)=delete;
public:
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit default user mapping constructor
        */
        map()=default;
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit mapping constructor
        */
        map(
                ::IOMemoryDescriptor*iokit_memory_descriptor///<IOMemoryDescriptor to be mapped into the user address space
                ){
                initialize(iokit_memory_descriptor);
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit user map initializer

        map<user> maps an IOMemoryDescriptor (typically cast of a dxd::memory or
        buffer class) into the user address space.
        */
        os_result initialize(
                ::IOMemoryDescriptor*iokit_memory_descriptor///<IOMemoryDescriptor to be mapped into the user address space
                ){
                /// Allows re-initialization.
                this->free();
                /// Checks buffer object,
                try(this->status=iokit_memory_descriptor?ok:not_initialized);
                /// maps it into user space and
                try(this->status=(
                        this->iokit_memory_map=iokit_memory_descriptor->createMappingInTask(
                                current_task(),0,::kIOMapAnywhere))?ok:not_initialized);
                /// and obtains a kernel virtual adress.
                try(this->status=(this->_vm=reinterpret_cast<vm_t*>(this->iokit_memory_map->getVirtualAddress()))?ok:not_initialized);
                //trace("  this:%p iokit:%p vm:%p\n",this,iokit_memory_map,vm);
        catch:
                return this->status;
        }
};


///////////////////////////////////////////////////////////////////////////////
/**
\brief kernel memory buffer w/o alignment and boundary requirements

The dxd::buffer is a kernel memory representation with an associated memory
backing store allocation. The memory backing store is owned by this kernel
memory representation and the reference will be released when this object is
destroyed.
*/
#pragma mark -
#pragma mark buffer<limit:64,alignment:0>
template<scope scope_t=kernel,unsigned int limit_t=64,unsigned int alignment_t=0>class buffer;
template<scope scope_t>class buffer<scope_t,64,0>:public memory<scope_t>{
        buffer(const buffer&)=delete;
public:
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit default buffer constructor

        Use this in conjunction with initialize()
        */
        buffer()=default;
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit buffer constructor
        */
        buffer(size_t size//<size to be alloced by buffer object
                ){
                initialize(size);
                //trace("  this:%p iokit:%p\n",this,iokit_memory_descriptor);
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit buffer initializer
        */
        os_result initialize(size_t size///<size to be alloced by buffer object
        ){
                /// Allows re-initialization.
                this->free();
                /// Creates a buffer and associated memory descriptor.
                try(this->status=(
                        this->iokit_memory_descriptor=::IOBufferMemoryDescriptor::withOptions(
                                ::kIODirectionOutIn||((scope_t==user)?::kIOMemoryKernelUserShared:0),
                                size,page_size))?ok:no_resources);
                /// Wires down the backing store memory.
                try(this->status=this->iokit_memory_descriptor->prepare());
                //trace("  this:%p iokit:%p\n",this,iokit_memory_descriptor);
        catch:;
                return this->status;
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit buffer constructor
        */
        buffer(
                size_t size,///<size to be alloced by buffer object
                unsigned int limit,///<limit physical memory address space
                unsigned int alignment,///<enforce physical memory alignment
                bool contigous=false///<enforce contiguos physical memory
                ){
                try(initialize(size,limit,alignment,contigous));
                //trace("  this:%p iokit:%p\n",this,iokit_memory_descriptor);
        catch:;
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit buffer initializer
        */
        os_result initialize(
                size_t size,///<size to be alloced by buffer object
                unsigned int limit,///<limit physical memory address space
                unsigned int alignment,///<enforce physical memory alignment
                bool contigous=false///<enforce contiguos physical memory
        ){
                /// Allows re-initialization.
//		trace(" mask:%llx\n",((1ull<<limit_t)-1)&~((1ull<<alignment_t)-1));

                this->free();
                /// Creates a buffer and associated memory descriptor.
                try(this->status=(
                        this->iokit_memory_descriptor=::IOBufferMemoryDescriptor::inTaskWithPhysicalMask(
//				scope_t==user?current_task():kernel_task,
                                kernel_task,
                                /*::kIODirectionOutIn||*/
                                ((scope_t==user)?::kIOMemoryKernelUserShared:0)|
                                (contigous?::kIOMemoryPhysicallyContiguous:0),
                                size,
                                ((1ull<<limit)-1)&~((1ull<<alignment)-1)
                                ))?ok:no_resources);
                /// Wires down the backing store memory.
                try(this->status=this->iokit_memory_descriptor->prepare());
                //trace("  this:%p iokit:%p\n",this,iokit_memory_descriptor);
        catch:;
                return this->status;
        }
};



///////////////////////////////////////////////////////////////////////////////
/**
\brief kernel memory buffer w/ alignment and boundary requirements

The dxd::buffer is a kernel memory representation with an associated memory
backing store allocation. The memory backing store is owned by this kernel
memory representation and the reference will be released when this object is
destroyed.
*/
#pragma mark -
#pragma mark buffer<>
template<scope scope_t,unsigned int limit_t,unsigned int alignment_t>class buffer:
        public memory<scope_t>{
        buffer(const buffer&)=delete;
public:
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit default buffer constructor

        Use this in conjunction with initialize()
        */
        buffer()=default;
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit buffer constructor
        */
        buffer(
                size_t size,///<size to be alloced by buffer object
                bool contigous=false///<enforce contiguos physical memory
                ){
                try(initialize(size,contigous));
                //trace("  this:%p iokit:%p\n",this,iokit_memory_descriptor);
        catch:;
        }
        ///////////////////////////////////////////////////////////////////////////////
        /**
        \brief IOKit buffer initializer
        */
        os_result initialize(
                size_t size,///<size to be alloced by buffer object
                bool contigous=false///<enforce contiguos physical memory
        ){
                /// Allows re-initialization.
//		trace(" mask:%llx\n",((1ull<<limit_t)-1)&~((1ull<<alignment_t)-1));

                this->free();
                /// Creates a buffer and associated memory descriptor.
                try(this->status=(
                        this->iokit_memory_descriptor=::IOBufferMemoryDescriptor::inTaskWithPhysicalMask(
//				scope_t==user?current_task():kernel_task,
                                kernel_task,
                                /*::kIODirectionOutIn||*/
                                ((scope_t==user)?::kIOMemoryKernelUserShared:0)|
                                (contigous?::kIOMemoryPhysicallyContiguous:0),
                                size,
                                ((1ull<<limit_t)-1)&~((1ull<<alignment_t)-1)
                                ))?ok:no_resources);
                /// Wires down the backing store memory.
                try(this->status=this->iokit_memory_descriptor->prepare());
                //trace("  this:%p iokit:%p\n",this,iokit_memory_descriptor);
        catch:;
                return this->status;
        }
};
#ifdef __doxygen__
}//namespace dxd
#endif//__doxygen__
