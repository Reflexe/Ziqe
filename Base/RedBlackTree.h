/**
 * @file RedBlackTree.h
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

#include "Base/Macros.h"
#include "Base/Memory.h"
#include "Base/Checks.h"

namespace Ziqe {

template<class KeyType, class T>
struct BinaryTreeNode {
    typedef BinaryTreeNode Node;

    struct Node *getRight ()
    {
        return mRight;
    }

    struct Node *getLeft ()
    {
        return mLeft;
    }

    struct Node *getParent()
    {
        return mParent;
    }

    struct Node *getRightOrLeft()
    {
        return (mRight == nullptr) ? mLeft : mRight;
    }

    struct Node *getLeftOrRight()
    {
        return (mLeft == nullptr) ? mRight : mLeft;
    }

    struct Node *getUncle () {
        if (! hasParent ())
            return nullptr;

        return _getUncle ();
    }

    struct Node *getBrother () {
        if (! hasParent ())
            return nullptr;

        return _getBrother ();
    }

    struct Node *getRightBrother () {
        if (! hasParent ())
            return nullptr;

        return _getRightBrother ();
    }

    struct Node *getLeftBrother () {
        if (! hasParent ())
            return nullptr;

        return _getLeftBrother ();
    }

    bool isRight () const{
        if (! hasParent ())
            return false;

        return _isRight ();
    }

    bool isLeft () const{
        if (! hasParent ())
            return false;

        return _isLeft ();
    }


    struct Node *getGrandparent () {
        if (! hasParent ())
            return nullptr;

        return parent->parent;
    }

    bool hasParent() const
    {
        return parent != nullptr;
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
    struct Node *_getUncle ()
    {
        return mParent->getBrother ();
    }

    struct Node *_getBrother () {
        if (mParent->mLeft == this)
            return mParent->mRight;
        else
            return mParent->mLeft;
    }

    struct Node *_getRightBrother ()
    {
        return mParent->mRight;
    }

    struct Node *_getLeftBrother ()
    {
        return mParent->mLeft;
    }

    bool _isRight () const
    {
        return (mParent->mRight == this);
    }

    bool _isLeft () const
    {
        return (mParent->mRight == this);
    }

    struct Node *_getGrandparent ()
    {
        return parent->parent;
    }

    struct Node *mRight;
    struct Node *mLeft;
    struct Node *mParent;

    Pair<KeyType, T> mKeyAndValue;
};

template<class KeyType,
         class T,
         class NodeType=BinaryTreeNode<T, KeyType>,
         class CompareType=IsLessThan<KeyType>,
         class IsEqualType=IsEqual<KeyType>>
class BinaryTree
{
public:
    typedef NodeType Node;

    template<class NodeType>
    class _IteratorBase {
        _IteratorBase(NodeType *node)
            : mCurrentNode{node}
        {
        }

        ALLOW_COPY_AND_MOVE (_IteratorBase)

        // from left to right.
        void next () {
            // First, let's try the simplest case.
            if (! mCurrentNode->hasParent ()) {
                mCurrentNode = mCurrentNode->getRight ();

            // We have a parent, therefore we can safely use the _* functions.
            } else if (mCurrentNode->_isLeft ()) {
                // If we're a left node, we should switch to our
                // left brother's most left node. If we have no right
                // brother we should switch to our parent.
                auto rightBrother = mCurrentNode->_getRightBrother();
                if (rightBrother != nullptr)
                    mCurrentNode = rightBrother->getLeftestNode ();
                else
                    mCurrentNode = mCurrentNode->getParent ();
            } else {
                // If we have a parent and we are a right node,
                // we should switch to our parent.
                mCurrentNode = mCurrentNode->getParent ();
            }
        }

        // from right to left.
        void previous () {
            // First, let's try the simplest case.
            if (! mCurrentNode->hasParent ()) {
                mCurrentNode = mCurrentNode->getLeft ();

            // We have a parent, therefore we can safely use the _* functions.
            } else if (mCurrentNode->_isRight ()) {
                // If we'rea right node, we should switch to our
                // left brother's most right node (sound like a very sad
                // story). If we have no left brother, we should switch
                // to our parent.
                auto leftBrother = mCurrentNode->_getLeftBrother();
                if (leftBrother != nullptr)
                    mCurrentNode = leftBrother->getRightestNode ();
                else
                    mCurrentNode = mCurrentNode->getParent ();
            } else {
                // If we have a parent and we are left node,
                // we should switch to our parent.
                mCurrentNode = mCurrentNode->getParent ();
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

        DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (_IteratorBase, mCurrentNode)

    protected:
        NodeType *mCurrentNode;
    };

    struct Iterator : _IteratorBase<Node> {
        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        Pair<KeyType, T> *operator ->() const
        {
            return &(this->mCurrentNode->mKeyAndValue);
        }
    };

    struct ConstIterator : _IteratorBase<const Node> {
        using _IteratorBase::_IteratorBase;
        using _IteratorBase::operator ++;
        using _IteratorBase::operator --;
        using _IteratorBase::operator =;
        using _IteratorBase::operator ==;

        const Pair<KeyType, T> *operator ->() const
        {
            return &(this->mCurrentNode->mKeyAndValue);
        }
    };

    Node *lookup (const KeyType &key) {
        Node *currentNode = mHead;

        while (currentNode != nullptr) {
            const auto &currentNodeKey = currentNode->key ();

            if (mIsEqual (key, currentNodeKey))
                return currentNode;

            if (mComare (key, currentNodeKey))
                currentNode = currentNode->left ();
            else
                currentNode = currentNode->right ();
        }

        // We have reached the end and didn't found the
        // required key.
        return nullptr;
    }

    template<class...Args>
    Pair<Iterator, bool> insert (Args&&... args) {
        Node *previousNode = nullptr;
        Node *currentNode = mHead;

        if (currentNode == nullptr) {
            return (mHead = mTail = new Node{previousNode,
                                             nullptr, nullptr,
                                             std::forward<Args>(args...)});
        }

        do {
            const auto &currentNodeKey = currentNode->key ();

            if (mIsEqual (key, currentNodeKey))
                return currentNode;

            previousNode = currentNode;

            if (mComare (key, currentNodeKey)) {
                currentNode = currentNode->left ();

                if (currentNode == nullptr)
                    return (previousNode->mLeft = new Node{previousNode,
                                                  nullptr, nullptr,
                                                  std::forward<Args>(args...)});
            } else {
                currentNode = currentNode->right ();

                if (currentNode == nullptr)
                    return (previousNode->mRight = new Node{previousNode,
                                                   nullptr, nullptr,
                                                   std::forward<Args>(args...)});
            }
        } while (true);
    }

    void erase (Node *node)
    {
        NOT_IMPLEMENTED ();
    }

    Node *mHead;
    Node *mLeftest;
    Node *mRightest;

    CompareType    mComare;
    IsEqualType    mIsEqual;
};

template<class KeyType,
         class T,
         class CompareType=IsLessThan<KeyType>,
         class IsEqualType=IsEqual<KeyType>>
class RedBlackTree
{
private:
    struct Node : BinaryTreeNode<KeyType, T>
    {
        enum class Color : bool{
            Red = true,
            Black = false
        };
        enum class RightOrLeft : bool{
            Right = true,
            Left = false
        };

        Color color;
        RightOrLeft rightOrLeft;
    };

    typedef BinaryTree<KeyType, T, Node, CompareType, IsEqualType> BinaryTreeType;

    using Iterator=typename BinaryTreeType::Iterator;
    using ConstIterator=typename BinaryTreeType::Iterator;
    using Node=typename BinaryTreeNode::Node;

public:
    RedBlackTree()
    {
    }



private:

    BinaryTreeType mBinaryTree;
};

} // namespace Ziqe

#endif // ZIQE_REDBLACKTREE_H
