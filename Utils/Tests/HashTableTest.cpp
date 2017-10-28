#include "Utils/HashTable.hpp"

#include "PerDriver/EntryPoints.h"

// Dummy hash function to check same hash.
template<class T, Ziqe::SizeType hash>
struct SameHash {
    Ziqe::SizeType operator () (const T&)
    {
        return hash;
    }
};

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnLoad) (void*) {
    using namespace Ziqe;

    Utils::HashTable<uint32_t, uint64_t> table;

    // isEmpty.
//    ZQ_ASSERT (table.isEmpty ());

//    // Check insert() and its ret val.
//    {
//        auto result = table.insert (1u, 0xffu);
//        ZQ_ASSERT (result.first);

//        ZQ_ASSERT (result.second->first == 1);
//        ZQ_ASSERT (result.second->second == 0xff);
//    }

//    // !isEmpty, isExist (count), size.
//    ZQ_ASSERT (! table.isEmpty ());
//    ZQ_ASSERT (table.isExist (1u));
//    ZQ_ASSERT (table.size () == 1);

//    // Check unique keys.
//    {
//        auto result = table.insert (1u, 2u);

//        ZQ_ASSERT (result.first == false);
//    }

//    // check insert again.
//    {
//        auto result = table.insert (2u, 0xffu);
//        ZQ_ASSERT (result.first);

//        ZQ_ASSERT (result.second->first == 2);
//        ZQ_ASSERT (result.second->second == 0xff);
//    }

//    ZQ_ASSERT (table.count (2u));
//    ZQ_ASSERT (table.size() == 2);

//    // check insertOrAssign.
//    {
//        auto result = table.insertOrAssign (2u, 0xfeedu);

//        ZQ_ASSERT (result != table.end ());
//        ZQ_ASSERT (result->first == 2u);
//        ZQ_ASSERT (result->second == 0xfeedu);
//    }

//    // Check regular erase.
//    {
//        auto nextIterator = table.erase (table.begin());

//        ZQ_ASSERT (nextIterator == table.begin ());
//        ZQ_ASSERT (table.size () == 1);
//    }

//    // Check range erase.
//    {
//        table.erase (table.begin (), table.end ());
//        ZQ_ASSERT (table.isEmpty ());
//    }

//    // Check same hash.
//    {
//        Utils::HashTable<uint32_t,
//                        uint64_t,
//                        Utils::IsEqual<uint32_t>,
//                        SameHash<uint32_t, 1>>
//                sameHashTable;

//        auto firstResult = sameHashTable.insert (2u, 3u);
//        auto secondResult = sameHashTable.insertOrAssign (3u, 4u);

//        ZQ_ASSERT (firstResult.first);
//        ZQ_ASSERT (secondResult != sameHashTable.end ());
//        ZQ_ASSERT (secondResult->second == 4u);
//        ZQ_ASSERT (firstResult.second->second == 3u);
//    }

    for (int i = 0; i < 0xfffff; ++i) {
        table.insert(i, 3u);
    }

//    for (int i = 0; i < 0xffffff; ++i) {
//        table.insert(i, 3u);
//    }
}

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnUnload) (void*)
{

}
