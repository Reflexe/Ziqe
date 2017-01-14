#include <linux/module.h>
#include <linux/bug.h>

#include "ZqAPI/Logging.h"

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
