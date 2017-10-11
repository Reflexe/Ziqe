/**
 * @file EntryPoint
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Main: copyright (C) 2016 Shmuel Hazan
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

#define _LINUX
#include <linux/module.h>

#include "CppCore/EntryPoints.h"

/**
 * @brief A global variable for private data that being forwarded to the
 *        clients.
 */
static void *g_private_data_ptr;

static int __init linux_init(void) {
        ZQ_SYMBOL (ZqOnLoad) (&g_private_data_ptr);

        return 0;
}


static void __exit linux_exit(void)
{
        ZQ_SYMBOL(ZqOnUnload) (&g_private_data_ptr);
}

module_init(linux_init);
module_exit(linux_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shmuel H.");
MODULE_DESCRIPTION("A ZiqeAPI Module");
