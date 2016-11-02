/**
 * @file HashTable.hpp
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
#ifndef ZIQE_HASHTABLE_H
#define ZIQE_HASHTABLE_H

#include "Types.hpp"
#include "Vector.hpp"
#include "Checks.hpp"
#include "Base/Macros.hpp"

#include "LinkedList.hpp"

namespace Ziqe {
namespace Base {

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

template<>
struct Hash<RawArray<const uint8_t>>
{
    // djb2 by Dan Bernstein @see http://www.cse.yorku.ca/~oz/hash.hpptml.
    SizeType operator () (const RawArray<const uint8_t> &array) {
        SizeType hash = 5381;

        for (uint8_t element : array) {
            hash = ((hash << 5) + hash) + element;
        }
        return hash;
    }
};

template<>
struct Hash<RawArray<uint8_t>>
{
    // djb2 by Dan Bernstein @see http://www.cse.yorku.ca/~oz/hash.hpptml.
    SizeType operator () (const RawArray<uint8_t> &array) {
        SizeType hash = 5381;

        for (uint8_t element : array) {
            hash = ((hash << 5) + hash) + element;
        }
        return hash;
    }
};

template<>
struct Hash<Vector<uint8_t>>
{
    SizeType operator () (const Vector<uint8_t> &vector)
    {
        return mHash(RawArray<const uint8_t>{vector.data (), vector.size ()});
    }

private:
    Hash<RawArray<const uint8_t>> mHash;
};

template<class T>
struct DerefrenceHash
{
    SizeType operator () (const T &value)
    {
        return mHash (*value);
    }

private:
    Hash<decltype (T::operator *())> mHash;
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
    static constexpr const SizeType sValue = min(SizeType{std::numeric_limits<KeyType>::max ()},
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

    // The Pair<> type.
    typedef Pair<KeyType, T> RealPairType;
public:
    typedef _Hash Hash;
    typedef _IsEqual IsEqual;
    typedef typename KeysListType::Iterator Iterator;
    typedef typename KeysListType::ConstIterator ConstIterator;

    struct PairType : RealPairType {
        PairType() = default;

        template<class... Args>
        explicit PairType(const KeyType &key,
                          const typename TableType::Iterator &parameterTableIterator,
                           Args&&... args)
            : RealPairType{key, T{Base::forward<Args>(args)...}},
              tableIterator{parameterTableIterator}
        {
        }

        typename TableType::Iterator tableIterator;
    };

    static constexpr TableSizeType DefaultTableSize = GetVectorInitialSize<KeyType>::sValue;

    _HashTableBase() {
        // No need for mKeysList.end (), default construction of KeysListType::Iterator
        // is nullptr anyway (and that's the same as mKeysList.end ()).
        mTable.resize (DefaultTableSize);
    }

    explicit _HashTableBase(TableSizeType tableSize) {
        // See comment above.
        mTable.resize (tableSize);
    }

    ZQ_ALLOW_MOVE (_HashTableBase)
    // FIXME: Copy shouldn't work because that the Iterators in the vector
    // would still be owned by the first owner.

    ZQ_DEFINE_CONST_AND_NON_CONST (ConstIterator, Iterator, begin, (), { return mKeysList.begin (); })
    ZQ_DEFINE_CONST_AND_NON_CONST (ConstIterator, Iterator, end, (), { return mKeysList.end (); })

     /**
      * @brief find  Find an iterator for @param key.
      * @param key  The requested iterator's key.
      * @return @param key's iterator if found, this->end() otherwise.
      */
     Iterator find (const KeyType &key) {
         auto firstEntry = getTableEntry (this->hash (key));

         return lookupKeyInKeyList (firstEntry, key);
     }

     ConstIterator find (const KeyType &key) const{
         auto firstEntry = getTableEntry (this->hash (key));

         return lookupKeyInKeyList (firstEntry, key);
     }

     /**
       * @brief isExist  Check if @param key exist in this hash table.
       * @param key
       * @return
       */
      bool isExist(const KeyType &key) const
      {
          return find (key) != end ();
      }

      /**
       * @brief erase  Remove an entry from this table.
       * @param iterator
       * @return (++ @param iterator)
       */
      Iterator erase (const Iterator &iterator) {
          auto tableIterator = iterator->tableIterator;
          bool isFirstEntry = (*tableIterator == iterator);
          auto nextEntry = mKeysList.erase (iterator);

          // If this is the first entry with this hash.
          if (isFirstEntry)
              updateEraseFirstHashEntryTableEntry (tableIterator, nextEntry);

          return nextEntry;
      }

      void erase (const Iterator &begin, const Iterator &end) {
          TableIterator firstEntryTableIterator;
          bool isAFirstEntryRemoved = false;

          // Clear mTable from iterators that going to be invalid soon.
          for (auto iterator = begin;
               iterator != end;
               ++iterator)
          {
              auto tableIterator = iterator->tableIterator;

              if (! isAFirstEntryRemoved) {
                  // is @iterator its hash's first entry?
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

          mKeysList.erase (begin, end);
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

      ConstIterator cbegin() const
      {
          return mKeysList.cbegin ();
      }

      ConstIterator cend() const
      {
          return mKeysList.cend ();
      }

private:
      // Key & Data iteration functions and type defs.

      // a few helpers for the type defs
      static const KeyType &GetConstKeyAction (const RealPairType &keyAndData)
      {
          return keyAndData.first;
      }

      static KeyType &GetKeyAction (RealPairType &keyAndData)
      {
          return keyAndData.first;
      }

      static const T &GetConstDataAction (const RealPairType &keyAndData)
      {
          return keyAndData.second;
      }

      static T &GetDataAction (RealPairType &keyAndData)
      {
          return keyAndData.second;
      }

public:
      typedef ActionIterator<Iterator, decltype (&_HashTableBase::GetKeyAction)> KeyIterator;
      typedef ActionIterator<ConstIterator, decltype (&_HashTableBase::GetConstKeyAction)> ConstKeyIterator;

      KeyIterator keysBegin ()
      {
          return {begin (), &_HashTableBase::GetKeyAction};
      }

      KeyIterator keysEnd ()
      {
          return KeyIterator{end (), &_HashTableBase::GetKeyAction};
      }

      ConstKeyIterator keysBegin () const
      {
          return ConstKeyIterator{begin (), &_HashTableBase::GetConstKeyAction};
      }

      ConstKeyIterator keysEnd () const
      {
          return ConstKeyIterator{end (), &_HashTableBase::GetConstKeyAction};
      }

      typedef ActionIterator<Iterator, decltype (&_HashTableBase::GetDataAction)> DataIterator;
      typedef ActionIterator<ConstIterator, decltype (&_HashTableBase::GetConstDataAction)> ConstDataIterator;

      DataIterator dataBegin ()
      {
          return {begin (), &_HashTableBase::GetDataAction};
      }

      DataIterator dataEnd ()
      {
          return {end (), &_HashTableBase::GetDataAction};
      }

      ConstDataIterator dataBegin () const
      {
          return {begin (), &_HashTableBase::GetConstDataAction};
      }

      ConstDataIterator dataEnd () const
      {
          return KeyIterator{end (), &_HashTableBase::GetConstDataAction};
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
     ConstIterator lookupKeyInKeyList (ConstIterator listIterator,
                                  const KeyType &key) const
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

     /// @brief Transform a key to an hash.
     SizeType hash(const KeyType &key) const
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

     mutable Hash mHash;
     mutable IsEqual mIsEqual;
};

template<class KeyType,
         class T,
         class _IsEqual=IsEqual<KeyType>,
         class _Hash=Hash<KeyType>,
         bool  _sAllowMultiKeys=false>
class HashTable : public _HashTableBase<KeyType, T, _IsEqual, _Hash>
{
public:
    typedef _HashTableBase<KeyType, T, _IsEqual, _Hash> HashTableType;
    typedef typename HashTableType::Iterator Iterator;
    typedef typename HashTableType::ConstIterator ConstIterator;
    typedef typename HashTableType::TableType TableType;
    typedef typename HashTableType::KeysListType KeysListType;

    using HashTableType::HashTableType;
    using HashTableType::operator =;

    using HashTableType::find;
    using HashTableType::erase;
    using HashTableType::size;
    using HashTableType::isEmpty;
    using HashTableType::isExist;
    using HashTableType::getSize;
    using HashTableType::begin;
    using HashTableType::end;
    using HashTableType::cbegin;
    using HashTableType::cend;

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
        typename TableType::Iterator    tableEntryIterator = this->getTableIterator (this->hash (key));
        // The hash's first key entry: listEnd if empty.
        typename KeysListType::Iterator firstKeyEntry = *tableEntryIterator;
        typename KeysListType::Iterator listEnd       = this->mKeysList.end();
        bool                            isEmptyHashEntry = (firstKeyEntry == listEnd);

        if (! isEmptyHashEntry) {
            // Check if there's an entry with the same key.
            auto entry = this->lookupKeyInKeyList (firstKeyEntry, key);
            if (entry != listEnd)
                return {false, entry};
        }

        // Insert our new key entry to the keys list: insert it after the first key entry
        // that has the same hash (firstKeyEntry) or mKeysList.end() is there're no
        // more key entries with the same hash.
        auto newKeyEntry = this->mKeysList.emplace_before (isEmptyHashEntry ? listEnd : next (firstKeyEntry),
                                                           key,
                                                           tableEntryIterator,
                                                           Base::forward<Args>(args)...);

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
        auto result = insert (key, Base::forward<Args>(args)...);

        if (! result.first)
            result.second->second = T{Base::forward<Args>(args)...};

        return result.second;
    }

    // Overloads for copy and move constructors to reduce the overhead
    // of creating a temporeraly T and moving it to .second (if it exists already).
    Iterator insertOrAssign(const KeyType &key,
                            const T& args)
    {
        auto result = insert (key, Base::forward<T&>(args));

        if (! result.first)
            result.second->second = args;

        return result.second;
    }

    Iterator insertOrAssign(const KeyType &key,
                            T&& args)
    {
        auto result = insert (key, Base::move (args));

        if (! result.first)
            result.second->second = Base::move (args);

        return result.second;
    }

    /**
     * @brief erase  Erase all of the entries with this key.
     * @param key
     */
    Iterator erase (const KeyType &key) {
        auto iterator = this->find (key);
        if (iterator == this->end ())
            return iterator;

        return this->erase (iterator);
    }
};

// An specialization of HashTable that allows multi keys.
template<class KeyType,
         class T,
         class _IsEqual,
         class _Hash>
class HashTable<KeyType, T, _IsEqual, _Hash, true> : public _HashTableBase<KeyType, T, _IsEqual, _Hash>
{
public:
    typedef _HashTableBase<KeyType, T, _IsEqual, _Hash> HashTableType;
    typedef typename HashTableType::Iterator Iterator;
    typedef typename HashTableType::ConstIterator ConstIterator;
    typedef typename HashTableType::TableType TableType;
    typedef typename HashTableType::KeysListType KeysListType;

    using HashTableType::HashTableType;
    using HashTableType::operator =;

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
     * @param args Arguments for @tparam T's constructor.
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
        auto newKeyEntry = this->mKeysList.emplace_after (isEmptyHashEntry
                                                          ? this->mKeysList.beforeEnd ()
                                                          : firstKeyEntry,
                                                          key,
                                                          tableEntryIterator,
                                                          Base::forward<Args>(args)...);

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
        auto equalIterators = getEqualRange (key);

        erase (equalIterators.first, equalIterators.second);
    }

    Pair<Iterator, Iterator> getEqualRange (const KeyType &key) {
        auto firstIterator = find (key);
        if (firstIterator == this->end ())
            return;

        auto lastIteator = firstIterator;

        // find the last iterator with this key.
        do {
            ++lastIteator;
        } while (lastIteator != this->end() && mIsEqual (key, lastIteator->first));


        return Pair<Iterator, Iterator>{firstIterator, lastIteator};
    }

    Pair<ConstIterator, ConstIterator> getEqualRange (const KeyType &key) const{
        auto firstIterator = find (key);
        if (firstIterator == this->end ())
            return;

        auto lastIteator = firstIterator;

        // find the last iterator with this key.
        do {
            ++lastIteator;
        } while (lastIteator != this->end() && mIsEqual (key, lastIteator->first));

        return Pair<Iterator, Iterator>{firstIterator, lastIteator};
    }
};

template<class KeyType,
         class T,
         class _IsEqual=IsEqual<KeyType>,
         class _Hash=Hash<KeyType>>
using MultiHashTable=HashTable<KeyType, T, _IsEqual, _Hash, true>;

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_HASHTABLE_H
