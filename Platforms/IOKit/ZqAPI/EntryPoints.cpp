#define KERNEL

#define __THROW
#define __nonnull(...)
#define __attribute_pure__
#define __attribute_malloc__
#define __wur
#define __BEGIN_NAMESPACE_STD namespace std {
#define __END_NAMESPACE_STD }
#define u_int32_t uint32_t
#define u_int64_t u_int64_t

#define fake_t(name) typedef int name;
fake_t (kern_return_t)
fake_t (boolean_t)
fake_t (lck_mtx_t)
fake_t (lck_rw_t)
fake_t (lck_spin_t)
fake_t (vm_size_t)
fake_t (vm_offset_t)
fake_t (task_t)
fake_t (mach_timespec_t)
fake_t (addr64_t)
fake_t (mach_vm_address_t)
fake_t (mach_vm_size_t)
fake_t (vm_map_t)
fake_t (ppnum_t)
fake_t (queue_chain_t)
fake_t (wait_result_t)
fake_t (IOReturn)

typedef IOReturn os_result;

#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>

#include "CppCore/EntryPoints.h"

#include "Platforms/Macros.h"


// IOKit entry points:
// init: load
// free: unload

// attach: onDeviceAttached
// detach: onDeviceDetached.

class ZQ_SYMBOL (ZqIOService) : protected ::IOService
{
    OSDeclareDefaultStructors(ZQ_SYMBOL (ZqIOService))

    typedef ::IOService Super;

private:
    ::IOOptionBits deferred_terminate_options;

    static::IOReturn __wake(::OSObject*owner,void*,void*,void*,void*){
        ::IOReturn status;
        device*device;
        try(status=(device=OSDynamicCast(dxd::device,owner))?ok:device_error);
        return device->wake();
        catch:
        return status;
    }

    static::IOReturn __sleep(::OSObject*owner,void*,void*,void*,void*){
        ::IOReturn status;
        device*device;
        try(status=(device=OSDynamicCast(dxd::device,owner))?ok:device_error);
        return device->sleep();
        catch:
        return status;
    }

    static::IOReturn __conclude(::OSObject*owner,void*,void*,void*,void*){
        ::IOReturn status;
        device*device;
        try(status=(device=OSDynamicCast(dxd::device,owner))?ok:device_error);
        device->conclude();
        catch:
        return status;
    }

    void tear_down(bool conclude=true){
        //start/stop/terminate are not serialized!
        ::IOCommandGate *iokit_cmdgate=this->iokit_cmdgate;

        if(iokit_cmdgate && OSCompareAndSwapPtr (iokit_cmdgate, nullptr, &this->iokit_cmdgate)){
            /// Calls user supplied conclude to let the user driver perfrom finalization and
            if(conclude)
                iokit_cmdgate->runAction(__conclude);
            if(iokit_workloop)
                iokit_workloop->removeEventSource(iokit_cmdgate);
            iokit_cmdgate->release();
        }
        /// stops power management after all device access.
        if(initialized)
            PMstop();
    }

    void *mPrivateData;

protected:
    bool start(IOService *provider) override{
        if(! Super::start(provider))
            return false;

        ZQ_SYMBOL (ZqOnLoad) (&mPrivateData);


        /// Obtains a workloop, assuming start will be called in a single threaded manner for each device object,
        iokit_workloop = IOWorkLoop::workLoop();

        /// creates a command gate to synchronously submit PNP/power request
        iokit_cmdgate=IOCommandGate::commandGate(this);

        /// add the command gate event source,
        iokit_workloop->addEventSource(iokit_cmdgate);

        /// launches device specific initialization,
//        try(launch());

        /// initializes power management,
        PMinit();

        /// and joins the PM tree,
        provider->joinPMtree(this);

        static IOPMPowerState power_states[] =
        {
            { 1, 0,            0,            0,            0, 0, 0, 0, 0, 0, 0, 0 },
            { 1, kIOPMPowerOn, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0 }
        };

        /// registers this driver with power management,
        registerPowerDriver(this, power_states, ZQ_ARRAY_LENGTH (power_states));

        // well, OSX can't know the device is awake at this point in time:
        /// and signals power management to "ON".
        makeUsable();

        /// It registers this device, so it can be found by user clients.
        registerService();

        return true;
    }

    /**
     * @return Called on initialization of the driver.
     */
    virtual bool init () override;

    /**
     * @brief Called on deinitialization of the driver.
     */
    virtual void free () override;

    /**
     \brief balance PNP

     Simulates synchronized PNP by shipping around OS PNP and synchronously
     calling sleep() (cmd gate is needed since terminate() is on callers thread
     and workloop)
     PM is not involved since synchronous device operation deadlocks from here
     if called when device is set asleep
     this is the only possible place to do so because:
      1. request/will/didTerminate() is not called on kextunload
      2. during stop() synchronous device operation will deadlock
     */
    bool terminate(::IOOptionBits options)override{
        if(iokit_cmdgate&&initialized&&getPowerState())
            iokit_cmdgate->runAction(__sleep);
        if(!clients)
            tear_down();
        return super::terminate(options);
    }


        /**
        \brief balance PNP: defer device termination to after last client closed.
        */
    bool didTerminate(::IOService*provider,::IOOptionBits options,bool*defer)override{
        if(clients){
            *defer=true;
            deferred_terminate_options=options;
            return false;
        }else {
            return super::didTerminate(provider,options,defer);
        }
    }

    void stop(::IOService*provider)override{
        // kextunload: help OSX synchronously tear down kext: wait for all
        // clients to get hands off that device (getClient() returns 0 during kextunload).
        while(clients)
            ::IOSleep(1);
        tear_down();
        super::stop(provider);
    }

    IOReturn setPowerState(unsigned long state, IOService *provider) override{
        ///\todo check if cmd gate is needed when provides workloop is device workloop

        if(initialized && (getPowerState() != state)) {
            switch(state) {
            case 0:
                if(iokit_cmdgate)
                    iokit_cmdgate->runAction(__sleep);
                break;
            default:
                if(iokit_cmdgate)
                    iokit_cmdgate->runAction(__wake);
            }
        }

        return kIOPMAckImplied;
    }


    ::IOWorkLoop*iokit_workloop;
    ::IOCommandGate*iokit_cmdgate;
    ///\endcond
public:

    /**
        \brief convenience ioctl accessor for direct inter-kext IOCTL
        */
    template<typename tx_t,typename rx_t>
    os_result ioctl(unsigned int ioctl,const tx_t&tx,rx_t&rx){
        size_t size=sizeof rx;
        return this->ioctl(ioctl,&tx,sizeof tx,&rx,sizeof rx,size);
    }

    /**
        \brief provider property request
        */
    template<typename type_t>os_result property(const char*name,type_t&value)const{
        os_result status;
        ::OSObject*property;
        ::IOService*provider;
        try(status=(provider=getProvider())?ok:not_initialized);
        try(status=(property=provider->getProperty(name))?ok:invalid);
        /// try if property is an OSString
        if(::OSString*string=OSDynamicCast(::OSString,property)){
            ::strncpy(reinterpret_cast<char*>(&value),string->getCStringNoCopy(),sizeof(value));
            return ok;
        }
        /// or, try to get it as OSData
        {::OSData*data;
            try(status=(data=OSDynamicCast(::OSData,property))?ok:invalid)
                    ::strncpy(reinterpret_cast<char*>(&value),reinterpret_cast<const char*>(data->getBytesNoCopy()),sizeof(value));}
        catch:
        return status;
    }

    /**
        \brief device property request
        */
    template<typename type_t>os_result query(const char*key,type_t&value);

    os_result notify_clients(){
        return messageClients(kIOMessageServicePropertyChange);
    }

    /**
        \brief apply executor functor for each user client
        */
    template<class client_t=::IOUserClient,typename return_t=os_result,typename exec_arg_t,typename exec_t>
    return_t each_client(exec_arg_t exec_arg,exec_t exec){
        return each<client_t,return_t>(getClientIterator(),exec_arg,exec);
    }

    /**
        \brief apply executor functor for each child
        */
    template<class client_t=::IOService,typename return_t=os_result,typename exec_arg_t,typename exec_t>
    return_t each_child(exec_arg_t exec_arg,exec_t exec){
        ::IOService*provider;
        try((provider=getProvider())?ok:not_initialized);
        return each<client_t,return_t>(provider->getChildIterator(gIOServicePlane),exec_arg,exec);
        catch:;
        return 0;
    }
};

OSDefineMetaClassAndStructors(ZQ_SYMBOL(ZqIOService), ZQ_SYMBOL(ZqIOService)::Super)

void ZQ_SYMBOL(ZqIOService)::init()
{
    ZQ_SYMBOL (ZqOnLoad) (&mPrivateData);
}

void ZQ_SYMBOL(ZqIOService)::free()
{
    ZQ_SYMBOL (ZqOnUnload) (&mPrivateData);
}
