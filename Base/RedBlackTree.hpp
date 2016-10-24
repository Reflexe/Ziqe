/**
 * @file RedBlackTree.hpp
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
#ifndef ZIQE_REDBLACKTREE_H
#define ZIQE_REDBLACKTREE_H

#include "Base/Macros.hpp"
#include "Base/Memory.hpp"
#include "Base/Checks.hpp"

namespace Ziqe {
namespace Base {

template<class KeyType, class T>
struct BinaryTreeNode {
    typedef BinaryTreeNode Node;

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getRight, (), { return mRight; })
    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getLeft, (), { return mLeft; })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getRightOrLeft, (), { return (mRight == nullptr) ? mLeft : mRight; })
    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getLeftOrRight, (), { return (mLeft == nullptr) ? mRight : mLeft; })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getUncle, (),
    {
        if (! hasParent ())
            return nullptr;

        return _getUncle ();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getBrother, (),
    {
        if (! hasParent ())
            return nullptr;

        return _getBrother ();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getRightBrother, (),
    {
        if (! hasParent ())
            return nullptr;

        return _getRightBrother ();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getLeftBrother, (),
    {
        if (! hasParent ())
            return nullptr;

        return _getLeftBrother ();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getGrandparent, (),
    {
        if (! hasParent ())
            return nullptr;

        return mParent->mParent;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getParent, (),
    {
        return mParent;
    })


    bool isRight() const{
        if (! hasParent ())
            return false;

        return _isRight ();
    }

    bool isLeft() const{
        if (! hasParent ())
            return false;

        return _isLeft ();
    }

    bool hasParent() const
    {
        return mParent != nullptr;
    }

    bool hasRight () const
    {
        return mRight != nullptr;
    }

    bool hasLeft () const
    {
        return mLeft != nullptr;
    }

    // Other version for some functions that doesn't do
    // the "parent check".
    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, _getUncle, (),
    {
        return mParent->getBrother ();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, _getBrother, (),
    {
        if (mParent->mLeft == this)
            return mParent->mRight;
        else
            return mParent->mLeft;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, _getRightBrother, (),
    {
        return  mParent->mRight;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, _getLeftBrother, (),
    {
        return  mParent->mRight;
    })

    bool _isRight () const
    {
        return (mParent->mRight == this);
    }

    bool _isLeft () const
    {
        return (mParent->mRight == this);
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, _getGrandparent, (),
    {
        return  mParent->mParent;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getLeftestNode, (),
    {
        // Node*
        auto node = mLeft;
        if (node == nullptr)
            return this;

        while (node->getLeft () != nullptr)
        {
            node = node->getLeft ();
        }

        return node;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getRightestNode, (),
    {
        // Node*
        auto node = mRight;
        if (node == nullptr)
            return this;

        while (node->getRight () != nullptr)
        {
            node = node->getRight ();
        }

        return node;
    })

    Pair<KeyType, T> mKeyAndValue;

    const KeyType &getKey () const
    {
        return mKeyAndValue.first;
    }

    Node *mRight;
    Node *mLeft;
    Node *mParent;

//    int getKey () const
//    {
        // SHOULD BE IMPLEMENTED by a BinaryTree Implementation.
//    }
};

template<class KeyType,
         class T,
         class NodeType=BinaryTreeNode<KeyType, T>,
         class CompareType=IsLessThan<KeyType>,
         class IsEqualType=IsEqual<KeyType>>
class BinaryTree
{
public:
    typedef NodeType Node;

    template<class IteratorNodeType>
    struct _IteratorBase {
        explicit _IteratorBase(IteratorNodeType *node)
            : mCurrentNode{node}
        {
        }

        ZQ_ALLOW_COPY_AND_MOVE (_IteratorBase)

        // from left to right.
        void next () {
            // First, let's try the simplest case.
            if (! mCurrentNode->hasParent ()) {
                mCurrentNode = static_cast<IteratorNodeType*>(mCurrentNode->getRight ());

            // We have a parent, therefore we can safely use the _* functions.
            } else if (mCurrentNode->_isLeft ()) {
                // If we're a left node, we should switch to our
                // left brother's most left node. If we have no right
                // brother we should switch to our parent.
                auto rightBrother = static_cast<IteratorNodeType*>(mCurrentNode->_getRightBrother());
                if (rightBrother != nullptr)
                    mCurrentNode = static_cast<IteratorNodeType*>(rightBrother->getLeftestNode ());
                else
                    mCurrentNode = static_cast<IteratorNodeType*>(mCurrentNode->getParent ());
            } else {
                // If we have a parent and we are a right node,
                // we should switch to our parent.
                mCurrentNode = static_cast<IteratorNodeType*>(mCurrentNode->getParent ());
            }
        }

        // from right to left.
        void previous () {
            // First, let's try the simplest case.
            if (! mCurrentNode->hasParent ()) {
                mCurrentNode = static_cast<IteratorNodeType*>(mCurrentNode->getLeft ());

            // We have a parent, therefore we can safely use the _* functions.
            } else if (mCurrentNode->_isRight ()) {
                // If we're a right node, we should switch to our
                // left brother's most right node (sound like a very sad
                // story). If we have no left brother, we should switch
                // to our parent.
                auto leftBrother = static_cast<IteratorNodeType*>(mCurrentNode->_getLeftBrother());
                if (leftBrother != nullptr)
                    mCurrentNode = static_cast<IteratorNodeType*>(leftBrother->getRightestNode ());
                else
                    mCurrentNode = static_cast<IteratorNodeType*>(mCurrentNode->getParent ());
            } else {
                // If we have a parent and we are left node,
                // we should switch to our parent.
                mCurrentNode = static_cast<IteratorNodeType*>(mCurrentNode->getParent ());
            }
        }

        _IteratorBase &operator ++ () {
            next ();

            return *this;
        }

        _IteratorBase &operator -- () {
            previous ();

            return *this;
        }

        ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (_IteratorBase, mCurrentNode)

    protected:
        IteratorNodeType *mCurrentNode;
    };

    struct Iterator : _IteratorBase<Node> {
        typedef _IteratorBase<Node> _IteratorBase;

        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        Pair<KeyType, T> *operator ->() const
        {
            return &(this->mCurrentNode->mKeyAndValue);
        }

        Pair<KeyType, T> &operator *() const
        {
            return this->mCurrentNode->mKeyAndValue;
        }
    };

    struct ConstIterator : _IteratorBase<const Node> {
        typedef _IteratorBase<const Node> _IteratorBase;

        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        const Pair<KeyType, T> *operator ->() const
        {
            return &(this->mCurrentNode->mKeyAndValue);
        }

        const Pair<KeyType, T> &operator *() const
        {
            return this->mCurrentNode->mKeyAndValue;
        }
    };

    ZQ_DEFINE_CONST_AND_NON_CONST (ConstIterator, Iterator, find, (const KeyType &key),
    {
        return {findNode (key)};
    })

    bool isExist (const KeyType &key) const
    {
        return find (key) != end ();
    }

    Iterator begin()
    {
        return Iterator{mLeftest};
    }

    ConstIterator cbegin() const
    {
        return ConstIterator{mLeftest};
    }

    ConstIterator begin() const
    {
        return cbegin ();
    }

    Iterator before_end()
    {
        return Iterator{mRightest};
    }

    ConstIterator cbefore_end() const
    {
        return ConstIterator{mRightest};
    }

    ConstIterator before_end() const
    {
        return cend ();
    }

    Iterator end()
    {
        return Iterator{nullptr};
    }

    ConstIterator cend() const
    {
        return ConstIterator{nullptr};
    }

    ConstIterator end() const
    {
        return cend ();
    }

    SizeType size() const
    {
        return mSize;
    }

    SizeType getSize() const
    {
        return mSize;
    }

    template<class...Args>
    Pair<Iterator, bool> insert (const KeyType &key, Args&&... args) {
        ZQ_UNUSED (key);

        NOT_IMPLEMENTED ();
    }

    Pair<Iterator, Iterator>
    findBefore(const KeyType &key);


    Pair<ConstIterator, ConstIterator>
    findBefore(const KeyType &key) const;

    ZQ_DEFINE_CONST_AND_NON_CONST (ZQ_ARG (Pair<ConstIterator, ConstIterator>),
                                   ZQ_ARG (Pair<Iterator, Iterator>),
                                   findAfter, (const KeyType &key),
    {
        auto findNodeResult = findNode(key);

        if (findNodeResult.first == nullptr)
            return {nullptr, nullptr};


        Iterator iterator{findNodeResult.first};
        if (findNodeResult.second == false)
            return {iterator, end()};
        else
            return {Base::previous(iterator), iterator};
    })

    Pair<Iterator, Iterator>
    findAfter(const KeyType &key);

    Pair<ConstIterator, ConstIterator>
    findAfter(const KeyType &key) const;

    void erase (Node *node)
    {
        ZQ_UNUSED (node);
        NOT_IMPLEMENTED ();
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (ZQ_ARG(Triple<ConstIterator, ConstIterator, ConstIterator>),
                                   ZQ_ARG(Triple<Iterator, Iterator, Iterator>),
                                   findBeforeAndAfter, (const KeyType &key),
    {
        auto findNodeResult = findNode(key);

        if (findNodeResult.first == nullptr)
            return {nullptr, nullptr, nullptr};


    })

    Pair<Node*, bool> findNode(const KeyType &key) {
        if (mHead == nullptr)
            return {nullptr, false};

        Node *currentNode = mHead;
        Node *nextNode    = mHead;

        do {
            currentNode = nextNode;

            auto compareResult = mCompare(key, currentNode->getKey ());

            if (compareResult == CompareType::Equal)
                return {currentNode, true};
            else if (compareResult == CompareType::GreaterThan)
                nextNode = currentNode->getLeft ();
            else
                nextNode = currentNode->getRight ();
        } while(nextNode != nullptr);

        // We have reached the end and didn't found the
        // required key.
        return {currentNode, false};
    }

    Node *mHead;
    Node *mLeftest;
    Node *mRightest;

    SizeType mSize;

    CompareType    pComare;
    IsEqualType    pIsEqual;
};

template<class T, class KeyType>
struct RedBlackTreeNode : BinaryTreeNode<T, KeyType> {
    enum class Color : bool{
        Red = true,
        Black = false
    };
    enum class RightOrLeft : bool{
        Right = true,
        Left = false
    };

    /**
     * @brief color  Describes this node's color.
     */
    Color color;

    /**
     * @brief rightOrLeft  Describes this node's direction: Right or Left.
     */
    RightOrLeft rightOrLeft;
};

// a very BIG TODO
template<class KeyType,
         class T,
         class CompareType=IsLessThan<KeyType>,
         class IsEqualType=IsEqual<KeyType>>
class RedBlackTree : private BinaryTree<KeyType, T, RedBlackTreeNode<KeyType, T>, CompareType, IsEqualType>
{
public:
    using Node=RedBlackTreeNode<KeyType, T>;
private:
    typedef BinaryTree<KeyType, T, Node, CompareType, IsEqualType> BinaryTreeType;

public:
    using Iterator=typename BinaryTreeType::Iterator;
    using ConstIterator=typename BinaryTreeType::Iterator;

    explicit RedBlackTree() = default;
    ZQ_ALLOW_COPY_AND_MOVE (RedBlackTree)

    using BinaryTreeType::begin;
    using BinaryTreeType::cbegin;

    using BinaryTreeType::end;
    using BinaryTreeType::cend;

    using BinaryTreeType::findAfter;
    using BinaryTreeType::findBefore;
    using BinaryTreeType::findBeforeAndAfter;

    using BinaryTreeType::isExist;
    using BinaryTreeType::find;

    template<class...Args>
    Pair<Iterator, bool> insert (const KeyType &key, Args&&... args) {

    }

    Iterator remove(const KeyType &key)
    {

    }

    Iterator erase(ConstIterator iterator)
    {

    }
};

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_REDBLACKTREE_H
