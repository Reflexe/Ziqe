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

/**
 * @brief HashTable  An hash table container with unique keys (similer to std::unordered_map).
 * @tparam KeyType   The hash table's key type.
 * @tparam T         The actual data type.
 * @tparam _Hash     The hash function to hash @tparam KeyType s.
 *
 * @todo Add @tparam not_equal_function.
 *
 * The desing of this hash table is simple: the table (mTable) is a vector of iterators
 * to a linked list (Ziqe::LinkedList mKeysList). Every item (PairType) in the keys list have a
 * key (.first), data (.second) and an iterator to its slot in the table {tableIterator}.
 *
 * So mKeysList contains a lot of PairType "mini-lists": every list is has its own
 * hash result (and a entry in mTable) they must be one after another in the list.
 * To identify a end of one of those "mini-lists" we need to compare each PairType's
 * tableIterator: if it not equal to the previous key entry, this key entry's key has
 * different hash that the previous one and therefore, it is not a part of this "mini-list".
 */
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

   /**
     * @brief isExist  Check if @param key exist in this hash table.
     * @param key
     * @return
     */
    bool isExist(const KeyType &key)
    {
        return find (key) != end ();
    }

   /**
     * @brief count  Count the times @param key is exist in this hash table.
     * @param key
     * @return
     */
    SizeType count(const KeyType &key)
    {
        return (isExist (key)) ? 1 : 0;
    }

   /**
     * @brief insert  Try to insert a new entry in to this table.
     * @param key     The new entry's key.
     * @param args Arguments for @tparam T's constructor.
     * @return On success, a Ziqe::Pair with .first=true  and .second=the newly created iterator.
     *         On failure, .first=false and .second set to the iterator of the entry with the same
     *                     key.
     */
    template<class... Args>
    Pair<bool, Iterator> insert(const KeyType &key,
                                Args&&... args)
    {
        // The hash entry in the table.
        TableType::Iterator     tableEntryIterator = getTableIterator (hash (key));
        // The hash's first key entry.
        KeysListType::Iterator  firstKeyEntry = *tableEntryIterator;
        KeysListType::Iterator  listEnd       = mKeysList.end();
        bool                    isEmptyHashEntry = (firstKeyEntry == listEnd);

        if (! isEmptyHashEntry) {
            // Check if there's an entry with the same key.
            auto entry = lookupKeyInKeyList (firstKeyEntry, key);
            if (entry != listEnd)
                return {false, entry};
        }

        // Insert our new key entry to the keys list: insert it after the first key entry
        // that has the same hash (firstKeyEntry) or mKeysList.end() is there're no
        // more key entries with the same hash.
        auto newKeyEntry = mKeysList.emplace_before (isEmptyHashEntry ? listEnd : ++firstKeyEntry,
                                                     key,
                                                     tableEntryIterator,
                                                     std::forward<Args>(args)...);

        // If there were no entries with this hash, set this key entry
        // as this hash's first key entry.
        if (isEmptyHashEntry)
            *tableEntryIterator = newKeyEntry;

        return {true, newKeyEntry};
    }

   /**
     * @brief Try to insert a new entry or replace its data if it
     *        exist already.
     * @param key  The new entry's key.
     * @param args Arguments for @tparam T's constructor.
     * @return The new / replaced entry's iterator.
     */
    template<class... Args>
    Iterator insertOrAssign(const KeyType &key,
                        Args&&... args)
    {
        auto result = insert (key, std::forward<Args>(args)...);

        if (! result.first)
            result.second->second = T{std::forward<Args>(args)...};

        return result.second;
    }

    // Overloads for copy and move constructors to reduce the overhead
    // of creating a temporeraly T and moving it to .second (if it exists already).
    Iterator insertOrAssign(const KeyType &key,
                            const T& args)
    {
        auto result = insert (key, std::forward<T&>(args));

        if (! result.first)
            result.second->second = args;

        return result.second;
    }

    Iterator insertOrAssign(const KeyType &key,
                            T&& args)
    {
        auto result = insert (key, std::move (args));

        if (! result.first)
            result.second->second = std::move (args);

        return result.second;
    }

    /**
     * @brief erase  Remove an entry from this table.
     * @param iterator
     * @return (++ @param iterator)
     */
    Iterator erase (Iterator &iterator) {
        auto tableIterator = iterator->tableIterator;
        bool isFirstEntry = (*tableIterator == iterator);
        auto nextEntry = mKeysList.erase (iterator);

        // If this is the first entry with this hash.
        if (isFirstEntry) {
            // If there're more entries with this hash.
            if (nextEntry != mKeysList.end() && nextEntry->tableIterator == tableIterator) {
                *tableIterator = nextEntry;
            } else {
                // Set this table iterator to null, there're no entries with this hash.
                *tableIterator = Iterator{};
            }
        }

        return nextEntry;
    }

    /**
     * @brief find  Find an iterator for @param key.
     * @param key  The requested iterator's key.
     * @return @param key's iterator if found, this->end() otherwise.
     */
    Iterator find (const KeyType &key) {
        auto firstEntry = getTableEntry (hash (key));

        return lookupKeyInKeyList (firstEntry, key);
    }

    ConstIterator find (const KeyType &key) const{
        auto firstEntry = getTableEntry (hash (key));

        return lookupKeyInKeyList (firstEntry, key);
    }

   /**
     * @brief begin  Reutrn the first iterator.
     * @return
     */
    Iterator begin()
    {
        return mKeysList.begin ();
    }

    /**
     * @brief end  Reutrn the past-last iterator.
     * @return
     */
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
     * @param listIterator        Iterator in mKeyList that you've got from mTable.
     * @param key                 The key we're looking for.
     * @return                    If found, an iterator for the PairType that contains @param key,
     *                            mKeysList.end() otherwise.
     */
    Iterator lookupKeyInKeyList (Iterator listIterator,
                                 const KeyType &key)
    {
        auto end = mKeysList.end();

        // The iterators in mTable are set to end when no key in mKeysList
        // match their index (hash).
        if (listIterator == end)
            return end;

        auto tableIterator = listIterator->tableIterator;

        // Walk through the keys list and look for @param key. Make sure
        // we stay in our hash ranges (by making sure listIterator->tableIterator
        // == tableIterator).
        while (listIterator->first != key) {
            ++listIterator;

            // We didn't found our key and already reached another table entry or
            // the end.
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
