/**
 * @file LinkedList.h
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
#ifndef ZIQE_LINKEDLIST_H
#define ZIQE_LINKEDLIST_H

#include "Memory.h"
#include "Checks.h"
#include "Macros.h"
#include "Types.h"

namespace Ziqe {

template <class T>
class LinkedList
{
public:
    // TODO: use a XOR linked list.
    struct DoubleLinkedListNode {
        template<class ...Args>
        DoubleLinkedListNode(struct DoubleLinkedListNode *previous,
                             struct DoubleLinkedListNode *next,
                             Args&&... args)
            : data{new T{std::forward<Args>(args)...}},
              previous{previous},
              next{next}
        {
        }

        DoubleLinkedListNode()
        {
        }

        UniquePointer<T> data;

        struct DoubleLinkedListNode *previous = nullptr;
        struct DoubleLinkedListNode *next     = nullptr;
    };

    typedef DoubleLinkedListNode NodeType;

    struct DoubleLinkedListIterator {
        DoubleLinkedListIterator ()
            : mCurrent{nullptr}
        {
        }

        DoubleLinkedListIterator (NodeType *current)
            : mCurrent{current}
        {
        }

        ALLOW_COPY_AND_MOVE (DoubleLinkedListIterator)

        DoubleLinkedListIterator &operator++ () {
            CHECK (mCurrent->next != nullptr);
            mCurrent = mCurrent->next;

            return *this;
        }

        DoubleLinkedListIterator &operator-- () {
            CHECK (mCurrent->previous != nullptr);
            mCurrent = mCurrent->previous;

            return *this;
        }

        DoubleLinkedListIterator next() const
        {
            return {mCurrent->next};
        }

        DoubleLinkedListIterator previous() const
        {
            return {mCurrent->previous};
        }

        T &operator* () {
            CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

        T *operator-> () {
            CHECK (mCurrent != nullptr && mCurrent->data);

            return mCurrent->data.get();
        }

        const T &operator* () const{
            CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

        const T *operator-> () const{
            CHECK (mCurrent != nullptr && mCurrent->data);

            return mCurrent->data.get();
        }

        DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER(DoubleLinkedListIterator, mCurrent)
    private:
        friend class LinkedList;

        NodeType *mCurrent;
    };

    typedef SizeType SizeType;
    typedef const DoubleLinkedListIterator ConstIterator;
    typedef DoubleLinkedListIterator Iterator;

    LinkedList()
        : mBegin{new NodeType{}}, mEnd{mBegin}, mSize{0}
    {
    }

    template<class InputIterator>
    LinkedList(InputIterator begin, InputIterator end)
        : LinkedList()
    {
        insert(begin(), begin, end);
    }

    LinkedList (LinkedList &&other)
    {
        swap(other);
    }

    LinkedList (const LinkedList &other)
        : LinkedList (other.mBegin, other.mEnd)
    {
    }

    LinkedList &operator= (LinkedList &&other)
    {
        swap (other);
    }

    LinkedList &operator= (const LinkedList &other) {
        clear ();

        insert (mBegin, other.mBegin, mEnd);
    }

    ~LinkedList()
    {
        deleteAll ();
        delete mEnd.mCurrent;
    }

    // Insert before.
    template<class InputIterator>
    void insert (ConstIterator &where,
                 const InputIterator &insertBegin,
                 const InputIterator &insertEnd)
    {
        static_assert(std::is_same<decltype (*insertBegin), T>::value,
                      "Invalid InputIterator");

        if (insertBegin == insertEnd)
            return;

        // Insert one element before @where and then add all the
        // others after it.
        auto     inputIterator   = insertBegin;
        emplace (where, *inputIterator);
        ++inputIterator;

        if (inputIterator != insertEnd)
            insertAfter (where, inputIterator, insertEnd);
    }

    // Emplace before.
    template<class ...Args>
    Iterator emplace(ConstIterator &where, Args&&... args) {
        CHECK (where.mCurrent);

        NodeType *newNode;

        newNode = new NodeType{where.mCurrent->previous, where.mCurrent, std::forward<Args>(args)...};
        where.mCurrent->previous = newNode;

        ++mSize;

        if (mBegin == where) {
            mBegin = DoubleLinkedListIterator{newNode};
            return mBegin;
        } else {
            return DoubleLinkedListIterator{newNode};
        }
    }

    void erase(ConstIterator &begin, ConstIterator &end) {
        CHECK (begin.mCurrent);

        if (begin == end)
            return;

        DoubleLinkedListIterator self = mBegin;
        NodeType *savedCurrentPrevious = begin.mCurrent->previous;

        do {
            auto currentCopy = self.mCurrent;
            if (self == mBegin)
                mBegin = mEnd;

            ++self;
            --mSize;
            delete currentCopy;

        } while (self != end);

        if (savedCurrentPrevious) {
            savedCurrentPrevious->next = self.mCurrent;
            self.mCurrent->previous    = savedCurrentPrevious;
        }

        if (begin == mBegin)
            mBegin = end;
    }

    Iterator erase(Iterator &iterator) {
        auto next = iterator.mCurrent->next;

        if (iterator.mCurrent->next)
            iterator.mCurrent->next->previous = iterator.mCurrent->previous;
        if (iterator.mCurrent->previous)
            iterator.mCurrent->previous->next = iterator.mCurrent->next;

        if (iterator == mBegin)
            mBegin = mEnd;

        delete iterator.mCurrent;

        --mSize;

        return Iterator{(next != nullptr) ? next : end()};
    }

    void swap(LinkedList &other) {
        Iterator begin = other.mBegin, end = other.mEnd;
        SizeType size = other.mSize;

        other.mBegin = mBegin;
        other.End    = mEnd;
        other.size   = mSize;

        mBegin       = begin;
        mEnd         = end;
        mSize        = size;
    }

    SizeType size() const
    {
        return mSize;
    }

    ConstIterator begin() const
    {
        return mBegin;
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

    T &back()
    {
        return *(--mEnd);
    }

    const T &back() const
    {
        return *(--mEnd);
    }

    void clear()
    {
        deleteAll();
    }

    bool isEmpty()
    {
        return mBegin == mEnd;
    }

    bool empty()
    {
        return isEmpty();
    }

private:
    template<class InputIterator>
    void insertAfter(ConstIterator &where,
                     const InputIterator &insertBegin,
                     const InputIterator &insertEnd)
    {
        static_assert(std::is_same<decltype (*insertBegin), T>::value,
                      "Invalid InputIterator");

        DoubleLinkedListIterator self = where;
        auto previousNext = where.mCurrent->next;

        for (auto iterator = insertBegin;
             iterator != insertEnd;
             ++iterator, ++mSize) {
            self.mCurrent->next = new NodeType{self.mCurrent, nullptr, *iterator};
            ++self;
        }

        self.mCurrent->next = previousNext;
        if (previousNext)
            previousNext->previous = self.mCurrent;
    }

    /**
     * @brief deleteAll Delete all of the entries in this list except mEnd.
     */
    void deleteAll() {
        if (isEmpty ())
            return;

        erase (mBegin, mEnd);

        mSize = 0;
    }

    Iterator mBegin;
    Iterator mEnd;

    SizeType mSize;
};

} // namespace Ziqe

#endif // ZIQE_LINKEDLIST_H
