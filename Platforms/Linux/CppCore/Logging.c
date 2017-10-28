#include <linux/module.h>
#include <linux/bug.h>

#include "CppCore/Logging.h"

void ZQ_SYMBOL(ZqLogText) (const char *string)
{
    printk(KERN_INFO "%s", string);
}

void ZQ_SYMBOL(ZqLogWarning) (const char *string)
{
    printk(KERN_WARNING"%s", string);
}

void ZQ_SYMBOL(ZqOnBug) (const char *string) {
    printk(KERN_ERR"%s", string);
//    BUG ();
}
