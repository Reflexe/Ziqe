/**
 * @file LinkedList.hpp
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
#ifndef ZIQE_LINKEDLIST_H
#define ZIQE_LINKEDLIST_H

#include "Utils/Memory.hpp"
#include "Utils/Checks.hpp"
#include "Utils/Macros.hpp"
#include "Utils/Types.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

template <class T>
class LinkedList
{
public:
    struct DoubleLinkedListNode {
        template<class ...Args>
        explicit DoubleLinkedListNode(struct DoubleLinkedListNode *parameterPrevious,
                             struct DoubleLinkedListNode *parameterNext,
                             Args&&... args)
            : data{Utils::forward<Args>(args)...},
              previous{parameterPrevious},
              next{parameterNext}
        {
        }

        T data;

        struct DoubleLinkedListNode *previous;
        struct DoubleLinkedListNode *next;
    };

    typedef DoubleLinkedListNode NodeType;

    struct _IteratorBase {
        _IteratorBase ()
            : mCurrent{nullptr}
        {
        }

        explicit _IteratorBase (NodeType *current)
            : mCurrent{current}
        {
        }

        ZQ_ALLOW_COPY_AND_MOVE (_IteratorBase)

        _IteratorBase &operator++ () {
            mCurrent = mCurrent->next;

            return *this;
        }

        _IteratorBase &operator-- () {
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

        ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER(_IteratorBase, mCurrent)

        bool isNull () const
        {
            return mCurrent == nullptr;
        }

    protected:
        friend class LinkedList;

        NodeType *mCurrent;
    };

    struct ConstIterator : _IteratorBase
    {
        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        const T &operator* () const{
            ZQ_ASSERT (this->mCurrent != nullptr);

            return this->mCurrent->data;
        }

        const T *operator-> () const{
            ZQ_ASSERT (this->mCurrent != nullptr);

            return &this->mCurrent->data;
        }
    };

    struct Iterator : _IteratorBase
    {
        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        // Convertion to ConstIterator
        operator ConstIterator() const
        {
            return ConstIterator{this->mCurrent};
        }

        T &operator* () const{
            ZQ_ASSERT (this->mCurrent != nullptr);

            return this->mCurrent->data;
        }

        T *operator-> () const{
            ZQ_ASSERT (this->mCurrent != nullptr);

            return &this->mCurrent->data;
        }
    };

    LinkedList()
        : mBegin{nullptr}, mBeforeEnd{mBegin}, mSize{0}
    {
    }

    template<class InputIterator>
    explicit LinkedList(const InputIterator &begin, const InputIterator &end)
        : LinkedList()
    {
        insert(cbegin(), begin, end);
    }

    LinkedList (LinkedList &&other)
    {
        swap(other);
    }

    LinkedList (const LinkedList &other)
        : LinkedList {other.cbegin (), other.cend ()}
    {
    }

    LinkedList &operator= (LinkedList &&other)
    {
        swap (other);
        return *this;
    }

    LinkedList &operator= (const LinkedList &other) {
        clear ();

        insert (cend (), other.cbegin (), other.cend ());
        return *this;
    }

    ~LinkedList()
    {
        clear ();
    }

    // Insert before.
    template<class InputIterator>
    void insert (const ConstIterator &where,
                 const InputIterator &insertBegin,
                 const InputIterator &insertEnd)
    {
        if (insertBegin == insertEnd)
            return;

        // Insert one element before @where and then add all the
        // others after it.
        auto inputIterator = insertBegin;
        auto newWhere = emplace_before (where, *inputIterator);
        ++inputIterator;

        if (inputIterator != insertEnd)
            insertAfter (newWhere, inputIterator, insertEnd);
    }

    template<class ...Args>
    Iterator emplace_before(const ConstIterator &where, Args&&... args) {
        if (where == cend ())
            return emplace_after (cBeforeEnd (), Utils::forward<Args>(args)...);

        NodeType *newNode;

        newNode = new NodeType{where.mCurrent->previous,
                               where.mCurrent,
                               Utils::forward<Args>(args)...};
        auto previous = where.mCurrent->previous;
        if (previous)
            previous->next = newNode;

        where.mCurrent->previous = newNode;

        ++mSize;

        if (cbegin () == where) {
            mBegin = Iterator{newNode};
            return mBegin;
        } else {
            return Iterator{newNode};
        }
    }

    template<class ...Args>
    Iterator emplace_after(const ConstIterator &where, Args&&... args) {
        NodeType *newNode;

        if (where == cend ()) {
            newNode = new NodeType{nullptr,
                                   nullptr,
                                   Utils::forward<Args>(args)...};

            // mBegin must be cend().
            mBegin = Iterator{newNode};
        } else {
            newNode = new NodeType{where.mCurrent,
                                   where.mCurrent->next,
                                   Utils::forward<Args>(args)...};
            auto next = where.mCurrent->next;
            if (next)
                next->previous = newNode;

            where.mCurrent->next = newNode;
        }

        ++mSize;

        if (where == mBeforeEnd) {
            mBeforeEnd = Iterator{newNode};
            return mBeforeEnd;
        } else {
            return Iterator{newNode};
        }
    }

    template<class ...Args>
    Iterator emplace_back(Args&&... args) {
        return emplace_after (mBeforeEnd,
                               Utils::forward<Args>(args)...);
    }

    template<class ...Args>
    Iterator emplace_front(Args&&... args) {
        return emplace_before (mBegin,
                               Utils::forward<Args>(args)...);
    }

    void erase(const ConstIterator &begin, const ConstIterator &end) {
        if (begin == end)
            return;

        ZQ_ASSERT (begin.mCurrent);

        ConstIterator iterator = begin;
        NodeType *savedCurrentPrevious = begin.mCurrent->previous;
        bool isBeginInvalidated = false;
        bool isBeforeEndInvalided = false;

        do {
            auto currentCopy = iterator.mCurrent;
            if (iterator == cbegin ())
                isBeginInvalidated = true;
            if (iterator == cBeforeEnd ())
                isBeforeEndInvalided = true;

            ++iterator;
            --mSize;
            delete currentCopy;

        } while (iterator != end);

        if (savedCurrentPrevious) {
            savedCurrentPrevious->next = iterator.mCurrent;
            iterator.mCurrent->previous = savedCurrentPrevious;
        }

        if (isBeginInvalidated)
            mBegin = end;
        if (isBeforeEndInvalided)
            mBeforeEnd = end;
    }

    Iterator erase(const ConstIterator &iterator) {
        auto next = Iterator{iterator.mCurrent->next};
        auto previous = Iterator{iterator.mCurrent->previous};

        if (iterator.mCurrent->next)
            iterator.mCurrent->next->previous = iterator.mCurrent->previous;
        if (iterator.mCurrent->previous)
            iterator.mCurrent->previous->next = iterator.mCurrent->next;

        delete iterator.mCurrent;
        --mSize;

        if (iterator == cbegin ())
            mBegin = next;
        if (iterator == cBeforeEnd ()) {
            mBeforeEnd = previous;
            return mBeforeEnd;
        }

        return next;
    }

    void pop_back ()
    {
        erase (cBeforeEnd ());
    }

    void pop_front ()
    {
        erase (cbegin ());
    }

    void swap(LinkedList &other) {
        Iterator begin = other.mBegin, beforeEnd = other.mBeforeEnd;
        SizeType size = other.mSize;

        other.mBegin     = mBegin;
        other.mBeforeEnd = mBeforeEnd;
        other.mSize       = mSize;

        mBegin       = begin;
        mBeforeEnd   = beforeEnd;
        mSize        = size;
    }

    SizeType size() const
    {
        return mSize;
    }

    ConstIterator cbegin() const
    {
        return mBegin;
    }

    ConstIterator cend() const
    {
        return ConstIterator{nullptr};
    }

    ConstIterator begin() const
    {
        return mBegin;
    }

    ConstIterator end() const
    {
        return ConstIterator{nullptr};
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
        return mBeforeEnd;
    }

    ConstIterator cBeforeEnd() const
    {
        return mBeforeEnd;
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

    void clear() {
        if (isEmpty ())
            return;

        erase (cbegin (), cend ());

        ZQ_ASSERT (size() == 0);
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
        ConstIterator self = where;
        auto previousNext = where.mCurrent->next;
        bool isBeforeEndUpdated = false;

        if (where == mBeforeEnd)
            isBeforeEndUpdated = true;

        for (auto iterator = insertBegin;
             iterator != insertEnd;
             ++iterator, ++mSize)
        {
            self.mCurrent->next = new NodeType{self.mCurrent, nullptr, *iterator};
            ++self;
        }

        self.mCurrent->next = previousNext;
        if (previousNext)
            previousNext->previous = self.mCurrent;

        if (isBeforeEndUpdated)
            mBeforeEnd = self;
    }

    Iterator mBegin;
    Iterator mBeforeEnd;

    SizeType mSize;
};

} // namespace Utils
ZQ_END_NAMESPACE

#endif // ZIQE_LINKEDLIST_H
