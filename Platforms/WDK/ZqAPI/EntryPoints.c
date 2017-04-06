#include <ntddk.h>

#include "ZqAPI/EntryPoints.h"

DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DriverUnload;

void *g_private_parameter;

NTSTATUS DriverEntry(struct _DRIVER_OBJECT *DriverObject,
                     PUNICODE_STRING        RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;

    ZqOnLoad (&g_private_parameter);
}

VOID DriverUnload(struct _DRIVER_OBJECT *DriverObject)
{
    ZqOnUnload (&g_private_parameter);
}
