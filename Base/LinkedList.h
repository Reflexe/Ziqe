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

#include <utility>

namespace Ziqe {

template <class T>
class LinkedList
{
public:
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

        template<class ...Args>
        DoubleLinkedListNode(struct DoubleLinkedListNode *previous,
                             struct DoubleLinkedListNode *next,
                             UniquePointer<T> &&pointer)
            : data{std::move (pointer)},
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

    template<class IteratorNodeType>
    struct _IteratorBase {
        _IteratorBase ()
            : mCurrent{nullptr}
        {
        }

        _IteratorBase (IteratorNodeType *current)
            : mCurrent{current}
        {
        }

        ALLOW_COPY_AND_MOVE (DoubleLinkedListIterator)

        _IteratorBase &operator++ () {
            DEBUG_CHECK (mCurrent->next != nullptr);
            mCurrent = mCurrent->next;

            return *this;
        }

        _IteratorBase &operator-- () {
            DEBUG_CHECK (mCurrent->previous != nullptr);
            mCurrent = mCurrent->previous;

            return *this;
        }

        _IteratorBase next() const
        {
            return {mCurrent->next};
        }

        _IteratorBase previous() const
        {
            return {mCurrent->previous};
        }

        DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER(_IteratorBase, mCurrent)

        bool isNull () const
        {
            return mCurrent == nullptr;
        }

        UniquePointer<T> takePointer()
        {
            return std::move(mCurrent->data);
        }

    protected:
        friend class LinkedList;

        IteratorNodeType *mCurrent;
    };

    struct Iterator : _IteratorBase<NodeType>
    {
        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        T &operator* () const{
            DEBUG_CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

        T *operator-> () const{
            DEBUG_CHECK (mCurrent != nullptr && mCurrent->data);

            return mCurrent->data.get();
        }
    };

    struct ConstIterator : _IteratorBase<const NodeType>
    {
        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        const T &operator* () const{
            DEBUG_CHECK (mCurrent != nullptr && mCurrent->data);

            return *mCurrent->data;
        }

        const T *operator-> () const{
            DEBUG_CHECK (mCurrent != nullptr && mCurrent->data);

            return mCurrent->data.get();
        }
    };

    typedef SizeType SizeType;

    LinkedList()
        : mBegin{nullptr}, mBeforeEnd{mBegin}, mSize{0}
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
        : LinkedList (other.mBegin, other.end ())
    {
    }

    LinkedList &operator= (LinkedList &&other)
    {
        swap (other);
    }

    LinkedList &operator= (const LinkedList &other) {
        clear ();

        insert (mBegin, other.mBegin, other.end ());
    }

    ~LinkedList()
    {
        deleteAll ();
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
        emplace_before (where, *inputIterator);
        ++inputIterator;

        if (inputIterator != insertEnd)
            insertAfter (where, inputIterator, insertEnd);
    }

    template<class ...Args>
    Iterator emplace_before(const ConstIterator &where, Args&&... args) {
        if (where == end ())
            return emplace_after (beforeEnd (), std::forward<Args>(args)...);

        NodeType *newNode;

        newNode = new NodeType{where.mCurrent->previous,
                               where.mCurrent,
                               std::forward<Args>(args)...};
        where.mCurrent->previous = newNode;

        ++mSize;

        if (mBegin == where) {
            mBegin = Iterator{newNode};
            return mBegin;
        } else {
            return Iterator{newNode};
        }
    }

    template<class ...Args>
    Iterator emplace_after(const ConstIterator &where, Args&&... args) {
        NodeType *newNode;

        if (where != end ()) {
            newNode = new NodeType{where.mCurrent,
                                   where.mCurrent->next,
                                   std::forward<Args>(args)...};
        } else {
            newNode = new NodeType{nullptr,
                                   nullptr,
                                   std::forward<Args>(args)...};
        }

        where.mCurrent->next = newNode;

        ++mSize;

        if (mBeforeEnd == where) {
            mBeforeEnd = Iterator{newNode};
            return mBeforeEnd;
        } else {
            return Iterator{newNode};
        }
    }

    template<class ...Args>
    Iterator emplace_back(Args&&... args) {
        return emplace_after (mBeforeEnd,
                               std::forward<Args>(args)...);
    }

    template<class ...Args>
    Iterator emplace_front(Args&&... args) {
        return emplace_before (mBegin,
                               std::forward<Args>(args)...);
    }

    void erase(const Iterator &begin, const Iterator &end) {
        if (begin == end)
            return;

        DEBUG_CHECK (begin.mCurrent);

        Iterator self = mBegin;
        NodeType *savedCurrentPrevious = begin.mCurrent->previous;
        bool isBeginInvalidated = false;
        bool isBeforeEndInvalided = false;

        do {
            auto currentCopy = self.mCurrent;
            if (self == mBegin)
                isBeginInvalidated = true;
            if (self == mBeforeEnd)
                isBeforeEndInvalided = true;

            ++self;
            --mSize;
            delete currentCopy;

        } while (self != end);

        if (savedCurrentPrevious) {
            savedCurrentPrevious->next = self.mCurrent;
            self.mCurrent->previous    = savedCurrentPrevious;
        }

        if (isBeginInvalidated)
            mBegin = end;
        if (isBeforeEndInvalided)
            mBeforeEnd = end;
    }

    Iterator erase(const Iterator &iterator) {
        auto next = Iterator{iterator.mCurrent->next};

        if (iterator.mCurrent->next)
            iterator.mCurrent->next->previous = iterator.mCurrent->previous;
        if (iterator.mCurrent->previous)
            iterator.mCurrent->previous->next = iterator.mCurrent->next;

        if (iterator == mBegin)
            mBegin = next;
        if (iterator == beforeEnd ())
            mBeforeEnd = next;

        delete iterator.mCurrent;

        --mSize;

        return next;
    }

    void pop_back ()
    {
        return erase (beforeEnd ());
    }

    void pop_front ()
    {
        return erase (mBegin);
    }

    void swap(LinkedList &other) {
        Iterator begin = other.mBegin, beforeEnd = other.mBeforeEnd;
        SizeType size = other.mSize;

        other.mBegin     = mBegin;
        other.mBeforeEnd = mBeforeEnd;
        other.size       = mSize;

        mBegin       = begin;
        mBeforeEnd   = beforeEnd;
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
        return Iterator{nullptr};
    }

    Iterator begin()
    {
        return mBegin;
    }

    Iterator end()
    {
        return Iterator{nullptr};
    }

    Iterator beforeEnd()
    {
        return mBeforeEnd;
    }

    ConstIterator beforeEnd() const
    {
        return ConstIterator{mBeforeEnd.mCurrent};
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
        return *(mBeforeEnd);
    }

    const T &back() const
    {
        return *(mBeforeEnd);
    }

    void clear()
    {
        deleteAll();
    }

    bool isEmpty() const
    {
        return begin () == end ();
    }

    bool empty()
    {
        return isEmpty();
    }

private:
    /// @note This function doesn't know to insert to end ().
    template<class InputIterator>
    void insertAfter(const ConstIterator &where,
                     const InputIterator &insertBegin,
                     const InputIterator &insertEnd)
    {
        static_assert(std::is_same<decltype (*insertBegin), T>::value,
                      "Invalid InputIterator");

        Iterator self = where;
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
     * @brief deleteAll Delete all of the entries in this list.
     */
    void deleteAll() {
        if (isEmpty ())
            return;

        erase (mBegin, end ());

        mSize = 0;
    }

    Iterator mBegin;
    Iterator mBeforeEnd;

    SizeType mSize;
};

} // namespace Ziqe

#endif // ZIQE_LINKEDLIST_H
