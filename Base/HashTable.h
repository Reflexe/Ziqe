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

template<class T>
struct Hash<UniquePointer<T>>
{
    SizeType operator () (const UniquePointer<T> &value)
    {
        return Hash<T>{} (*value);
    }
};

template<class T>
struct Hash<SharedPointer<T>>
{
    SizeType operator () (const SharedPointer<T> &value)
    {
        return Hash<T>{} (*value);
    }
};

/// @brief An generic IsEqual.
template<class T>
struct IsEqual
{
    bool operator () (const T &one,
                      const T &other)
    {
        return !(one != other);
    }
};

// An helper HashTable uses to detect the required initial size for a
// table.
namespace {
/// @b Default vector size.
static const SizeType kVectorInitialSize = 1000;

template<class KeyType, bool value = std::numeric_limits<KeyType>::is_specialized>
struct GetVectorInitialSize
{
    static constexpr const SizeType sValue = kVectorInitialSize;
};

template<class KeyType>
struct GetVectorInitialSize<KeyType, true>
{
    static constexpr const SizeType sValue = min(std::numeric_limits<KeyType>::max (),
                                                 kVectorInitialSize);
};
}

/**
 * @brief _HashTableBase  An hash table container.
 * @tparam KeyType   The hash table's key type.
 * @tparam T         The actual data type.
 * @tparam IsEqual   Used to compare @tparam KeyType. By default it uses the class' != operator.
 *                   Must be a default constructable type that will be initilized once per
 *                   HashTable.
 * @tparam _Hash     The hash function to hash @tparam KeyType s: Must be a default constructable
 *                   type that will be initilized once per HashTable.
 *
 * The desing of this hash table is simple: the table (mTable) is a vector of iterators
 * to a linked list (Ziqe::LinkedList mKeysList). Every item (PairType) in the keys list have a
 * key (.first), data (.second) and an iterator to its slot in the table {tableIterator}.
 *
 * So mKeysList contains a lot of PairType "mini-lists" (ranges): every list has its own
 * hash result (and an entry in mTable). The entries in the list come right after another.
 * To identify a end of one of those "mini-lists" we need to compare each PairType's
 * tableIterator: if it not equal to the previous key entry, this key entry's key has
 * different hash that the previous one and therefore, it is not a part of this "mini-list".
 */
template<class KeyType,
         class T,
         class _IsEqual=IsEqual<KeyType>,
         class _Hash=Hash<KeyType>>
class _HashTableBase
{
public:
    struct PairType;

protected:
    typedef LinkedList<PairType> KeysListType;

    typedef Vector<typename KeysListType::Iterator> TableType;
    typedef typename TableType::Iterator TableIterator;
    typedef typename TableType::SizeType TableSizeType;
public:
    typedef _Hash Hash;
    typedef _IsEqual IsEqual;
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

    _HashTableBase()
    {
        mTable.resize (getVectorInitialSize (), mKeysList.end ());
    }

    ALLOW_COPY_AND_MOVE (_HashTableBase)

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
       * @brief isExist  Check if @param key exist in this hash table.
       * @param key
       * @return
       */
      bool isExist(const KeyType &key)
      {
          return find (key) != end ();
      }

      /**
       * @brief erase  Remove an entry from this table.
       * @param iterator
       * @return (++ @param iterator)
       */
      Iterator erase (Iterator &&iterator) {
          auto tableIterator = iterator->tableIterator;
          bool isFirstEntry = (*tableIterator == iterator);
          auto nextEntry = mKeysList.erase (std::move (iterator));

          // If this is the first entry with this hash.
          if (isFirstEntry)
              updateEraseFirstHashEntryTableEntry (tableIterator, nextEntry);

          return nextEntry;
      }

      void erase (Iterator &&begin, Iterator &&end) {
          TableIterator firstEntryTableIterator;
          bool isAFirstEntryRemoved = false;

          // Clear mTable from iterators that going to be invalid soon.
          for (auto iterator = begin;
               iterator != end;
               ++iterator)
          {
              auto tableIterator = iterator->tableIterator;

              if (! isAFirstEntryRemoved) {
                  // is @iterator is the first entry the its hash?
                  if (*tableIterator == iterator) {
                      isAFirstEntryRemoved = true;
                      firstEntryTableIterator = tableIterator;
                  }
              } else {
                  // is entry before @iterator was the last removed entry with its hash?
                  if (tableIterator != firstEntryTableIterator) {
                      // it's the same as updateEraseFirstHashEntryTableEntry but with
                      // a little optimization (iterator cannot be end() and we will delete
                      // the last entry with this hash so there's no "next entry" to switch
                      // tableIterator to).
                      *firstEntryTableIterator = end ();

                      isAFirstEntryRemoved = false;
                  }
              }
          }

          if (isAFirstEntryRemoved)
              updateEraseFirstHashEntryTableEntry (firstEntryTableIterator,
                                                   end);

          mKeysList.erase (std::move (begin), std::move (end));
      }

      SizeType size() const
      {
          return mKeysList.size ();
      }

      SizeType getSize () const
      {
          return size ();
      }

      bool isEmpty () const
      {
          return size () == 0;
      }

protected:
      /**
        * @brief Update an erased first hash entry's table entry.
        *
        * mTable is pointing it, and it has been erased, we should
        * update mTable to the next entry with the same hash or to
        * end() if there's no more entries with the same hash.
        */
      void updateEraseFirstHashEntryTableEntry (TableIterator tableIterator, Iterator nextEntry) {
          // If there're more entries with this hash.
          if (nextEntry != mKeysList.end() && nextEntry->tableIterator == tableIterator) {
              *tableIterator = nextEntry;
          } else {
              // Set this table iterator to end, there're no entries with this hash.
              *tableIterator = mKeysList.end ();
          }
      }

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
         while (! mIsEqual(listIterator->first, key)) {
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
     static constexpr const TableSizeType getVectorInitialSize()
     {
         return GetVectorInitialSize<KeyType>::sValue;
     }

     /// @brief Transform a key to an hash.
     SizeType hash(const KeyType &key)
     {
         return mHash (key);
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

     Hash mHash;
     IsEqual mIsEqual;
};

template<class KeyType,
         class T,
         class _IsEqual=IsEqual<KeyType>,
         class _Hash=Hash<KeyType>,
         bool  _sAllowMultiKeys=false>
class HashTable : public _HashTableBase<KeyType, T, _IsEqual, _Hash>
{
public:
    HashTable() = default;

    typedef _HashTableBase<KeyType, T, _IsEqual, _Hash> HashTableType;
    typedef typename HashTableType::Iterator Iterator;
    typedef typename HashTableType::ConstIterator ConstIterator;
    typedef typename HashTableType::TableType TableType;
    typedef typename HashTableType::KeysListType KeysListType;

    ALLOW_COPY_AND_MOVE (HashTable)

    T &operator[] (const KeyType &key)
    {
        return (insert (key).second)->second;
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
     * @param args Arguments for @tparam T's constructor, or a UniquePointer<T> xvalue to take the
     *             contain the pointer's data.
     * @return On success, a Ziqe::Pair with .first=true  and .second=the newly created iterator.
     *         On failure, .first=false and .second set to the iterator of the entry with the same
     *                     key.
     */
    template<class... Args>
    Pair<bool, Iterator> insert(const KeyType &key,
                                Args&&... args)
    {
        // The hash entry in the table.
        typename TableType::Iterator    tableEntryIterator = getTableIterator (hash (key));
        // The hash's first key entry.
        typename KeysListType::Iterator firstKeyEntry = *tableEntryIterator;
        typename KeysListType::Iterator listEnd       = this->mKeysList.end();
        bool                            isEmptyHashEntry = (firstKeyEntry == listEnd);

        if (! isEmptyHashEntry) {
            // Check if there's an entry with the same key.
            auto entry = lookupKeyInKeyList (firstKeyEntry, key);
            if (entry != listEnd)
                return {false, entry};
        }

        // Insert our new key entry to the keys list: insert it after the first key entry
        // that has the same hash (firstKeyEntry) or mKeysList.end() is there're no
        // more key entries with the same hash.
        auto newKeyEntry = this->mKeysList.emplace_before (isEmptyHashEntry ? listEnd : ++firstKeyEntry,
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
     * @brief erase  Erase all of the entries with this key.
     * @param key
     */
    void erase (const KeyType &key) {
        auto iterator = find (key);
        if (iterator == this->end ())
            return;

        this->erase (std::move (iterator));
    }
};

// An specialization of HashTable that allows multi keys.
template<class KeyType,
         class T,
         class _IsEqual,
         class _Hash>
class HashTable<KeyType, T, _IsEqual, _Hash, true> : public _HashTableBase<KeyType, T, _IsEqual, _Hash>
{

    HashTable() = default;

    typedef _HashTableBase<KeyType, T, _IsEqual, _Hash> HashTableType;
    typedef typename HashTableType::Iterator Iterator;
    typedef typename HashTableType::ConstIterator ConstIterator;
    typedef typename HashTableType::TableType TableType;
    typedef typename HashTableType::KeysListType KeysListType;

    ALLOW_COPY_AND_MOVE (HashTable)

   /**
     * @brief count  Count the times @param key is exist in this hash table.
     * @param key
     * @return
     */
    SizeType count(const KeyType &key) {
        auto iterator = find (key);

        if (iterator == this->end ())
            return 0;

        SizeType keyCount = 0;

        do {
            ++keyCount;
            ++iterator;
        } while (iterator != this->end() && mIsEqual (key, iterator->first));

        return keyCount;
    }

   /**
     * @brief insert  Try to insert a new entry in to this table.
     * @param key     The new entry's key.
     * @param args Arguments for @tparam T's constructor, or a UniquePointer<T> xvalue to take the
     *             contain the pointer's data.
     * @return the newly created entry's iterator.
     */
    template<class... Args>
    Iterator insert(const KeyType &key,
                                Args&&... args)
    {
        // The hash entry in the table.
        typename TableType::Iterator    tableEntryIterator = getTableIterator (hash (key));
        // The hash's first key entry.
        typename KeysListType::Iterator firstKeyEntry = *tableEntryIterator;
        typename KeysListType::Iterator listEnd       = this->mKeysList.end();
        bool                            isEmptyHashEntry = (firstKeyEntry == listEnd);

        if (! isEmptyHashEntry) {
            // Check if there's an entry with the same key.
            auto keyEntry = lookupKeyInKeyList (firstKeyEntry, key);

            // If there's, we should insert it right after the same key.
            if (keyEntry != listEnd)
                firstKeyEntry = keyEntry;
        }

        // Insert our new key entry to the keys list: insert it after the first key entry
        // that has the same hash/key (firstKeyEntry) or mKeysList.end() is there're no
        // more key entries with the same hash.
        auto newKeyEntry = this->mKeysList.emplace_before (isEmptyHashEntry ? listEnd : ++firstKeyEntry,
                                                           key,
                                                           tableEntryIterator,
                                                           std::forward<Args>(args)...);

        // If there were no entries with this hash, set this key entry
        // as this hash's first key entry.
        if (isEmptyHashEntry)
            *tableEntryIterator = newKeyEntry;

        return newKeyEntry;
    }

    /**
     * @brief erase  Erase all of the entries with this key.
     * @param key
     */
    void erase (const KeyType &key) {
        auto iterator = find (key);
        if (iterator == this->end ())
            return;

        auto lastIteator = iterator;

        // find the last iterator with this key.
        do {
            ++lastIteator;
        } while (lastIteator != this->end() && mIsEqual (key, lastIteator->first));

        erase (std::move (lastIteator), std::move (lastIteator));
    }

private:

};

template<class KeyType,
         class T,
         class _IsEqual=IsEqual<KeyType>,
         class _Hash=Hash<KeyType>>
using MultiHashTable=HashTable<KeyType, T, _IsEqual, _Hash, true>;

} // namespace Ziqe

#endif // ZIQE_HASHTABLE_H
