/**
 * @file ForwardList.h
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
#ifndef ZIQE_FORWARDLIST_H
#define ZIQE_FORWARDLIST_H

#include "Types.h"
#include "Memory.h"
#include "Macros.h"

namespace Ziqe {

template <class T>
class ForwardList
{
public:
    class SingleLinkedListIterator {
        friend class ForwardList;

        struct SingleLinkedListNode {
            template<class ...Args>
            SingleLinkedListNode(struct SingleLinkedListNode *next,
                                 Args&&... args)
                : data{new T{std::forward<Args>(args)...}},
                  next{next}
            {
            }

            SingleLinkedListNode()
            {
            }

            UniquePointer<T> data;

            struct SingleLinkedListNode *next = nullptr;
        };

        typedef SingleLinkedListNode NodeType;

        SingleLinkedListIterator ()
            : mCurrent{nullptr}
        {
        }

        SingleLinkedListIterator (NodeType *current)
            : mCurrent{current}
        {
        }

        ALLOW_COPY_AND_MOVE (SingleLinkedListIterator)
        DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER(SingleLinkedListIterator, mCurrent)

        SingleLinkedListIterator &operator++ () {
            CHECK (mCurrent->next != nullptr);
            mCurrent = mCurrent->next;

            return *this;
        }

        T &operator* () {
            CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

        T *operator-> () {
            CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

        const T &operator* () const{
            CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

        const T *operator-> () const{
            CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

    private:
        NodeType *mCurrent;
    };

    typedef SizeType SizeType;
    typedef const SingleLinkedListIterator ConstIterator;
    typedef SingleLinkedListIterator Iterator;
    typedef typename Iterator::NodeType NodeType;

    ForwardList()
        : mBegin{new NodeType{}}, mEnd{nullptr}, mSize{0}
    {
    }

    template<class InputIterator>
    ForwardList(InputIterator begin, InputIterator end)
        : ForwardList()
    {
        insertAfter(mBegin, begin, end);
    }

    ForwardList (ForwardList &&other)
    {
        swap(other);
    }

    ForwardList (const ForwardList &other)
        : ForwardList (other.mBegin, other.mEnd)
    {
    }

    ForwardList &operator= (ForwardList &&other)
    {
        swap (other);
    }

    ForwardList &operator= (const ForwardList &other) {
        clear ();

        insertAfter (mBegin, other.mBegin, mEnd);
    }

    ~ForwardList() {
        deleteAll ();
        delete mBegin.mCurrent;
    }

    void swap(ForwardList &other) {
        Iterator end = other.mEnd, begin = other.mBegin;
        SizeType size = other.mSize;

        other.mSize = mSize;
        other.mEnd = mEnd;
        other.mBegin = mBegin;

        mEnd = end;
        mBegin = begin;
        mSize = size;
    }

    template<class InputIterator>
    void insertAfter (Iterator &where,
                      const InputIterator &insertBegin,
                      const InputIterator &insertEnd)
    {
        static_assert(std::is_same<decltype (*insertBegin), T>::value,
                      "Invalid InputIterator");

        SizeType objectsInserted = 0;
        SingleLinkedListIterator self = *this;
        auto previousNext = where.mCurrent->next;

        for (auto iterator = insertBegin;
             iterator != insertEnd;
             ++iterator, ++objectsInserted) {
            self.mCurrent->next = new NodeType{self.mCurrent, *iterator};
            ++self;
        }

        self.mCurrent->next = previousNext;

        mSize += objectsInserted;
    }

    // Emplace before.
    template<class ...Args>
    Iterator emplaceAfter(Iterator &where, Args&&... args) {
        CHECK (where.mCurrent);

        NodeType *newNode = new NodeType{where.mCurrent->next, std::forward<Args>(args)...};
        where.mCurrent->next = newNode;

        ++mSize;

        return Iterator{newNode};
    }

    void eraseAfter(Iterator &begin, Iterator &end) {
        CHECK (begin.mCurrent);

        if (begin == end)
            return;

        SingleLinkedListIterator self = *this;
        SizeType erasedObjects = 0;

        ++self;

        do {
            auto currentCopy = self.mCurrent->next;
            ++self;
            ++erasedObjects;

            delete currentCopy;

        } while (self != end);

        begin.mCurrent->next = self.mCurrent;

        mSize -= erasedObjects;
    }

    void eraseAfter(Iterator &iterator) {
        CHECK(!isEmpty());

        NodeType *deletedNode = iterator.mCurrent->next;
        CHECK (deletedNode);

        iterator.mCurrent->next = deletedNode->next;

        delete deletedNode;
    }

    SizeType size() const
    {
        return mSize;
    }

    ConstIterator before_begin() const
    {
        return mBegin;
    }

    Iterator before_begin()
    {
        return mBegin;
    }

    ConstIterator begin() const
    {
        return ++mBegin;
    }

    ConstIterator end() const
    {
        return mEnd;
    }

    Iterator begin()
    {
        return mBegin;
    }

    Iterator end()
    {
        return mEnd;
    }

    T &front()
    {
        return *mBegin;
    }

    const T &front() const
    {
        return *mBegin;
    }

    void clear()
    {
        deleteAll();
    }

    bool isEmpty()
    {
        return mSize == 0;
    }

    bool empty()
    {
        return isEmpty();
    }

private:
    /**
     * @brief deleteAll Delete all of the entries in this list except mEnd.
     */
    void deleteAll() {
        if (isEmpty ())
            return;

        eraseAfter (mBegin, mEnd);
        mSize = 0;
    }

    Iterator mBegin;
    Iterator mEnd;

    SizeType mSize;
};

} // namespace Ziqe

#endif // ZIQE_FORWARDLIST_H
