#include <linux/module.h>
#include <linux/bug.h>

#include "ZiqeAPI/Logging.h"

// TODO: consider making this functions inline

void ZqLogText (const char *string)
{
    printk(KERN_INFO "%s", string);
}

void ZqLogWarning (const char *string)
{
    printk(KERN_WARNING"%s", string);
}

void ZqOnBug (const char *string) {
    printk(KERN_ERR"%s", string);
    BUG ();
}
