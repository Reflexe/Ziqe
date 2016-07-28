/**
 * @file HashTable.h
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
#ifndef ZIQE_HASHTABLE_H
#define ZIQE_HASHTABLE_H

#include "Types.h"
#include "Vector.h"
#include "Checks.h"

#include "LinkedList.h"

namespace Ziqe {

template <class T>
struct Hash
{
    void operator () (const T &) = delete;
};

#define ziqe_define_trivial_hash(type) \
template<>\
struct Hash<type>\
{\
    SizeType operator () (const type &value)\
    {\
        return static_cast<SizeType>(value);\
    }\
};

ziqe_define_trivial_hash(uint8_t)
ziqe_define_trivial_hash(int8_t)
ziqe_define_trivial_hash(uint16_t)
ziqe_define_trivial_hash(int16_t)
ziqe_define_trivial_hash(uint32_t)
ziqe_define_trivial_hash(int32_t)
ziqe_define_trivial_hash(uint64_t)
ziqe_define_trivial_hash(int64_t)

#undef ziqe_define_trivial_hash

template<class KeyType, class T, class _Hash=Hash<KeyType>>
class HashTable
{
public:
    struct PairType;

private:

    typedef LinkedList<PairType> KeysListType;

    typedef Vector<typename KeysListType::Iterator> TableType;
    typedef typename TableType::Iterator TableIterator;
    typedef typename TableType::SizeType TableSizeType;

    /// @b Default vector size.
    static const TableSizeType kVectorInitialSize = 1000;
public:
    typedef _Hash Hash;
    typedef typename KeysListType::Iterator Iterator;
    typedef const Iterator ConstIterator;

    struct PairType {
        PairType()
        {
        }

        template<class... Args>
        PairType(const KeyType &key,
                 const typename TableType::Iterator &tableIterator,
                 Args&&... args)
            : first{key},
              second{std::forward<Args>(args)...},
              tableIterator{tableIterator}
        {
        }

        KeyType first;
        T second;

        typename TableType::Iterator tableIterator;
    };

    HashTable()
    {
        mTable.resize (getVectorInitialSize (), mKeysList.end ());
    }

    ALLOW_COPY_AND_MOVE (HashTable)

    T &operator[] (const KeyType &key)
    {
        return (insert (key).second)->second;
    }

    bool isExist(const KeyType &key)
    {
        return getTableEntry (key) != mKeysList.end ();
    }

    SizeType count(const KeyType &key)
    {
        return (isExist (key)) ? 1 : 0;
    }

    template<class... Args>
    Pair<bool, Iterator> insert(const KeyType &key,
                                Args&&... args)
    {
        auto tableEntryIterator = getTableIterator (hash (key));
        auto &firstEntry = *tableEntryIterator;
        auto listEnd = mKeysList.end();

        // Check if there's a entry with the same key.
        auto entry = lookupKeyInKeyList (firstEntry, key);
        if (entry != listEnd)
            return {false, entry};

        entry = mKeysList.emplace (firstEntry, key, tableEntryIterator, std::forward<Args>(args)...);

        // If there were no entries with this hash.
        // Set the first entry to this entry (firstEntry is a reference).
        if (firstEntry == listEnd)
            firstEntry = entry;

        return {true, entry};
    }

    template<class... Args>
    void insertOrAssign(const KeyType &key,
                        Args&&... args)
    {
        auto result = insert (key, std::forward<Args>(args)...);

        // TODO: create a specialization for T as Args in order not to
        //       move twice (or copy and move).
        if (! result.first)
            result.second->second = T{std::forward<Args>(args)...};
    }

    void insertOrAssign(const KeyType &key,
                        T& args)
    {
        auto result = insert (key, std::forward<T&>(args));

        if (! result.first)
            result.second->second = std::forward<T&>(args);
    }

    void insertOrAssign(const KeyType &key,
                        T&& args)
    {
        auto result = insert (key, std::forward<T&&>(args));

        if (! result.first)
            result.second->second = std::forward<T&&>(args);
    }

    Iterator erase (Iterator &iterator) {
        auto tableIterator = iterator->tableIterator;
        bool isFirstEntry = (*tableIterator == iterator);
        auto nextEntry = mKeysList.erase (iterator);

        // If this is the first entry with this hash.
        if (isFirstEntry) {
            // If ther're more entries with this hash.
            if (nextEntry != mKeysList.end() && nextEntry->tableIterator == tableIterator) {
                *tableIterator = nextEntry;
            } else {
                *tableIterator = mKeysList.end();
            }
        }

        return nextEntry;
    }

    Iterator find (const KeyType &key) {
        auto firstEntry = getTableEntry (hash (key));

        return lookupKeyInKeyList (firstEntry, key);
    }

    ConstIterator find (const KeyType &key) const{
        auto firstEntry = getTableEntry (hash (key));

        return lookupKeyInKeyList (firstEntry, key);
    }

    Iterator begin()
    {
        return mKeysList.begin ();
    }


    Iterator end()
    {
        return mKeysList.end ();
    }

    ConstIterator begin() const
    {
        return mKeysList.begin ();
    }


    ConstIterator end() const
    {
        return mKeysList.end ();
    }

private:

    /**
     * @brief lookupKeyInKeyList  Look for a key in a KeysList.
     * @param listIterator        Iterator in mKeyList that you got from mTable.
     * @param key                 The key we're looking for.
     * @return                    If found, an iterator for the PairType that contains @param key,
     *                            mKeysList.end() otherwise.
     */
    Iterator lookupKeyInKeyList (Iterator listIterator,
                                 const KeyType &key)
    {
        auto end = mKeysList.end();

        if (listIterator == end)
            return end;

        auto tableIterator = listIterator->tableIterator;

        // listIterator may be a end, but if not, it must have the correct tableIterator.
        // Therefore, there's no need to check that in the first time.
        while (listIterator->first != key) {
            ++listIterator;

            // We didn't found our key and already in another table entry or
            // in the end.
            if (listIterator == end || listIterator->tableIterator != tableIterator)
                return end;
        }

        // Return the found entry.
        return listIterator;
    }

    /// @b Get the initial vector size for this type of HashTable.
    static constexpr const TableSizeType getVectorInitialSize() {
        return min(std::numeric_limits<KeyType>::max(),
                   kVectorInitialSize);
    }

    /// @brief Transform a key to an hash.
    static SizeType hash(const KeyType &key)
    {
        return Hash () (key);
    }

    Iterator &getTableEntry(TableSizeType index)
    {
        return mTable[index % mTable.size()];
    }

    const Iterator &getTableEntry(TableSizeType index) const
    {
        return mTable[index % mTable.size()];
    }

    TableIterator getTableIterator(TableSizeType index)
    {
        return mTable.begin() + (index % mTable.size());
    }

    const TableIterator getTableIterator(TableSizeType index) const
    {
        return mTable.begin() + (index % mTable.size());
    }

    TableType mTable;

    KeysListType mKeysList;
};

} // namespace Ziqe

#endif // ZIQE_HASHTABLE_H
