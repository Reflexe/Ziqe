#define _LINUX

#include "../Logging.h"

#include <linux/module.h>
#include <linux/bug.h>

// TODO: consider making this functions inline

void ZqLogText (const char *string)
{
    pr_info("Hello! ZqLogText");
    printk(KERN_INFO "%s", string);
}

void ZqLogWarning (const char *string)
{
    pr_info("Hello! ZqLogWarning");
    printk(KERN_WARNING"%s", string);
}

void ZqOnBug (const char *string) {
    pr_info("Hello! ZqOnBug");
    printk(KERN_ERR"%s", string);
    BUG ();
}
