/**
 * @file EntryPoint
 * @author shrek0 (shrek0.tk@gmail.com)
 *
 * Main: copyright (C) 2016 shrek0
 *
 * Main is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Main is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file Linux.c
 * @author shrek0 (shrek0.tk@gmail.com)
 *
 * Ziqe: copyright (C) 2016 shrek0
 *
 * Ziqe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ziqe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>

#include "../EntryPoints.h"

static int __init linux_init(void)
{
        pr_info("in linux_init\n");
        ZqAPIInit();

        return 0;
}


static void __exit linux_exit(void)
{
        ZqAPIExit();
        pr_info("in linux_exit\n");
}

module_init(linux_init);
module_exit(linux_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("shrek0");
MODULE_DESCRIPTION("An ZiqeAPI Module");
