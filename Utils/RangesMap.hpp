/**
 * @file RangesMap.hpp
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
#ifndef ZIQE_RANGESMAP_H
#define ZIQE_RANGESMAP_H

#include "Utils/RedBlackTree.hpp"
#include "Utils/IteratorTools.hpp"



ZQ_BEGIN_NAMESPACE
namespace Utils {

#if 0
//TODO redblack tree
// Is that required at all?
// NO; we should move what required to MemoryMap.

template<class _ValueType, class _IndexType>
struct Range {
    using IndexType=_IndexType;
    using ValueType=_ValueType;

    IndexType begin;
    IndexType end;

    IndexType getBegin () const
    {
        return begin;
    }

    IndexType getEnd () const
    {
        return end;
    }
};
template<class ValueType,
         // TODO
         class RangeType=Range<ValueType, int>,
         class IndexType=typename RangeType::IndexType>
class RangesMap
{
    typedef RedBlackTree<IndexType, ValueType> TreeType;
    typedef typename TreeType::Iterator TreeIterator;
    typedef Pair<TreeIterator,TreeIterator> IteratorRange;

    TreeType mTree;

public:
    RangesMap();

    TreeIterator find (const IndexType &index) const
    {
        return mTree.findBefore (index).first;
    }

    template<class...Args>
    TreeIterator emplace (const RangeType &range,
                          Args&&...args)
    {
        auto begin = range.getBegin ();
        auto end = range.getEnd ();

        ZQ_ASSERT (begin < end);

        auto beginIterator = mTree.insert_or_assign (begin, Utils::forward<Args>(args...));

        auto endIterator = mTree.findBefore (beginIterator);
        bool isEndIteratorEnd = (endIterator == mTree.end ());
        ZQ_ASSERT (endIterator != mTree.begin ());

        auto beforeEndIterator = Utils::prev (endIterator);
        bool isKeyEndExist = (!isEndIteratorEnd
                              && is_keys_equal (endIterator->first, keyEnd));
        // Erase endIterator if there a value duplication between it and
        // std::next (endIterator).
        bool eraseEndToo = (isKeyEndExist && endIterator->second == val);

        // Is there any element between [++keyBegin, keyEnd]?
        if (beginIterator != beforeEndIterator) {
            // If keyEnd is a key in the map, just erase all the collisions.
            // If not, move beforeEndIterator to keyEnd.
            if (!isKeyEndExist) {
                ZQ_ASSERT (mTree.count (keyEnd) == 0);
                endIterator = mTree.emplace_hint (beforeEndIterator, keyEnd,
                                                  Utils::move (beforeEndIterator->second));
            }

            // if eraseEndToo, endIterator != end().
            if (eraseEndToo)
                endIterator = mTree.erase (std::next (beginIterator), std::next (endIterator));
            else
                mTree.erase (std::next (beginIterator), endIterator);

        } else if (beginIterator != mTree.begin ()) {
            // If there are no elements in this range,
            // we should also copy std::prev (beginIterator)
            // in order to stop our interval after --keyEnd.

            auto beforeBegin = std::prev (beginIterator);

            if (! isKeyEndExist) {
                // Copy beforeBegin's value to the new element after the range.
                // Our beginIterator is the only iterator until endIterator, which is
                // greater than keyEnd.
                endIterator = mTree.emplace_hint (beginIterator, keyEnd, V{beforeBegin->second});
            } else if (eraseEndToo) {
                endIterator = mTree.erase (endIterator);
            }
        }
    }
};

#endif

} // namespace Utils
ZQ_END_NAMESPACE

#endif // ZIQE_RANGESMAP_H
