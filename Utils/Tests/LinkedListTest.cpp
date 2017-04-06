/**
 * @file LinkedListTest
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2016 Shmuel Hazan
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

#include "Utils/LinkedList.hpp"
#include "Utils/IteratorTools.hpp"
#include "ZqAPI/EntryPoints.h"

#define N 20

void ZQ_SYMBOL(ZqOnLoad) (void*)
{
    using Ziqe::Utils::LinkedList;
    using Ziqe::Utils::next;
    using Ziqe::Utils::prev;

    LinkedList<int> list;
    LinkedList<int> anotherList;

    // c: emplace_back
    for (int i = 1; i < N; ++i)
    {
        list.emplace_back (i);
    }

    // emplace_front
    list.emplace_front (0);

    {
        int i = 0;
        for (int n : list)
        {
            ZQ_ASSERT (i == n);

            ++i;
        }
    }

    // copy consturctor
    anotherList = list;
    ZQ_ASSERT (list.size () == N);
    ZQ_ASSERT (anotherList.size () == N);

    // end, before end
    ZQ_ASSERT (list.end () == next (list.beforeEnd ()));

    // erase
    list.erase (list.begin ());
    list.erase (list.beforeEnd ());

    list.insert (list.cbegin (), anotherList.begin (), next (anotherList.begin ()));
    list.insert (list.cend (), anotherList.beforeEnd (), anotherList.end ());

    {
        int i = 0;
        for (int n : list)
        {
            ZQ_ASSERT (i == n);

            ++i;
        }
    }

    {
        int i = 0;
        for (int n : anotherList)
        {
            ZQ_ASSERT (i == n);

            ++i;
        }
    }
}

void ZQ_SYMBOL(ZqOnUnload) (void*)
{
}
