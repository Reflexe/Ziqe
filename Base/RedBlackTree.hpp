/**
 * @file RedBlackTree.hpp
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
#ifndef ZIQE_REDBLACKTREE_H
#define ZIQE_REDBLACKTREE_H

#include "Base/Macros.hpp"
#include "Base/Memory.hpp"
#include "Base/IteratorTools.hpp"
#include "Base/Checks.hpp"

namespace Ziqe {
namespace Base {

struct BinaryTreeNodeData {
};

template<class KeyType,
         class T,
         class NodeDataType=BinaryTreeNodeData,
         class CompareType=IsLessThan<KeyType>,
         class IsEqualType=IsEqual<KeyType>>
class BinaryTree
{
public:
    struct Node {
        template<class...Args>
        Node(Node *parent, Node *right, Node *left, const KeyType &key, Args&&...args)
            : mKeyAndValue{Base::move (key), {Base::forward<Args>(args)...}}, mParent{parent}, mRight{right}, mLeft{left}
        {
        }

        ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*&, getRight, (), { return mRight; })
        ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*&, getLeft, (), { return mLeft; })

    //    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getRightOrLeft, (), { return (mRight == nullptr) ? mLeft : mRight; })
    //    ZQ_DEFINE_CONST_AND_NON_CONST (const Node*, Node*, getLeftOrRight, (), { return (mLeft == nullptr) ? mRight : mLeft; })

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

        Node *mParent;
        Node *mRight;
        Node *mLeft;

        NodeDataType mFields;
    };

    struct _IteratorBase {
        explicit _IteratorBase(Node *node)
            : mCurrentNode{node}
        {
        }

        ZQ_ALLOW_COPY_AND_MOVE (_IteratorBase)

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
                // If we're a right node, we should switch to our
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

        ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (_IteratorBase, mCurrentNode)

    protected:
        Node *mCurrentNode;
    };

    struct Iterator : _IteratorBase {
        using _IteratorBase::_IteratorBaseType;
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

    struct ConstIterator : _IteratorBase {
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

    // Node *parent, Node *right, Node *left, const KeyType &key, Args&&...args
    template<class...Args>
    Pair<Iterator, bool> insert (const KeyType &key, Args&&... args) {
        auto result = findNode (key);

        // If this tree doesn't have an head.
        if (result.resultParent == nullptr) {
            mHead = mRightest = mLeftest = new Node{nullptr, nullptr, nullptr,
                                                    key, Base::forward<Args>(args)...};

            return {mHead, true};
        }

        ZQ_ASSERT (result.pResultAtParent != nullptr);

        // If there is a node with the same key already.
        if (*(result.pResultAtParent) != nullptr)
            return {*(result.pResultAtParent), false};


        auto newNode = new Node{result.resultParent, nullptr, nullptr,
                                key, Base::forward<Args>(args)...};

        *(result.pResultAtParent) = newNode;

        if (newNode->_isLeft ()) {
            if (mLeftest == newNode->getParent()) {
                mLeftest = newNode;
            }
        } else {
            if (mRightest == newNode->getParent()) {
                mRightest = newNode;
            }
        }

        return {newNode, true};
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
            return {Base::prev(iterator), iterator};
    })

    void erase (Node *node)
    {
        ZQ_UNUSED (node);
        ZQ_NOT_IMPLEMENTED ();
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (ZQ_ARG(Triple<ConstIterator, ConstIterator, ConstIterator>),
                                   ZQ_ARG(Triple<Iterator, Iterator, Iterator>),
                                   findBeforeAndAfter, (const KeyType &key),
    {
        auto findNodeResult = findNode(key);

        if (findNodeResult.first == nullptr)
            return {nullptr, nullptr, nullptr};


    })

protected:
    /**
        @brief  Try to find a node with key == @a key.
        @param key
        @return
    */
    struct FindNodeResult {
        Node *resultParent;
        Node **pResultAtParent;
    };

    FindNodeResult findNode(const KeyType &key) {
        Node *nextNodeParent  = nullptr;
        Node **nextNode       = &mHead;

        while (*nextNode != nullptr) {
            auto compareResult = mCompare(key, nextNode->getKey ());

            // Check if we have found the right node.
            if (compareResult == CompareType::Equal)
                return {nextNodeParent, nextNode};

            // If not, move to the next node.
            nextNodeParent = *nextNode;

            if (compareResult == CompareType::GreaterThan)
                nextNode = &(nextNodeParent->getRight());
            else
                nextNode = &(nextNodeParent->getLeft ());
        }

        // We have reached the end and didn't found the
        // required key. nextNode would be a pointer to nullptr.
        return {nextNode, nextNodeParent};
    }

    Node *mHead;
    Node *mLeftest; /* The smallest element in the tree */
    Node *mRightest;

    SizeType mSize;

    CompareType    pComare;
    IsEqualType    pIsEqual;
};

struct RedBlackTreeNodeData {
    enum class Color : bool{
        Red = true,
        Black = false
    };
    enum class Direction : bool{
        Right = true,
        Left = false
    };

    /**
     * @brief mColor  Describes this node's color.
     */
    Color mColor;

    /**
     * @brief mDirection  Describes this node's direction: Right or Left.
     */
    Direction mDirection;
};

// TODO: a very BIG TODO
template<class KeyType,
         class T,
         class CompareType=IsLessThan<KeyType>,
         class IsEqualType=IsEqual<KeyType>>
class RedBlackTree : private BinaryTree<KeyType, T, RedBlackTreeNodeData, CompareType, IsEqualType>
{
private:
    typedef BinaryTree<KeyType, T, RedBlackTreeNodeData, CompareType, IsEqualType> BinaryTreeType;

    using Color=RedBlackTreeNodeData::Color;
    using Node=typename BinaryTreeType::Node;

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

    void rotateLeft (Node *node, Node *grandparent) {
        grandparent->mRight = node->getLeft ();

        if (grandparent->hasLeft ())
            grandparent->mRight->mParent = grandparent;
        // We'll update the node right in the next line
        // no need to NULL it here.

        node->mLeft       = grandparent;

        // Update grandfather's previous parent's link.
        if (grandparent->isLeft ())
            grandparent->getParent ()->mLeft = node;
        else
            grandparent->getParent ()->mRight = node;

        grandparent->mParent = node;
    }

    void rotateRight (Node *node, Node *grandparent) {
        // In every time we change node's ownership, we
        // need to modify tree pointers:
        // * The old node's parent's [left|right] pointer -> NULL.
        // * The new parent's [left|right] pointer        -> node.
        // * The node's parent field.                     -> new parent.

        grandparent->mLeft = node->getRight ();
        if (grandparent->hasRight ())
            grandparent->mRight->mParent = grandparent;
        // We'll update the node right in the next line
        // no need to NULL it here.

        node->mRight       = grandparent;

        // Update grandfather's previous parent's link.
        if (grandparent->isLeft ())
            grandparent->getParent ()->mLeft = node;
        else
            grandparent->getParent ()->mRight = node;

        grandparent->mParent = node;
    }

    template<class...Args>
    Pair<Iterator, bool> insert (const KeyType &key, Args&&... args) {
        // It is an empty tree, insert a black one in the head.

        auto iteratorAndIsSucceed = BinaryTreeType::insert (key, Base::forward<Args>(args)...);

        if (! iteratorAndIsSucceed->second)
            return iteratorAndIsSucceed;

        const auto &iterator = iteratorAndIsSucceed.first;
        auto *currentNode = iterator.mCurrentNode;

        while (true) {
            if (! currentNode->hasParent ()) {
                currentNode->mColor = Color::Black;
                break;
            } else if (currentNode->getParent ()->mFields.mColor != Color::Black) {
                auto uncle = currentNode->_getUncle ();
                auto parent = currentNode->getParent ();
                auto grandparent = parent->getParent ();

                if (uncle != nullptr && uncle.mFields.mColor == Color::Red) {
                    parent->mFields.mColor = Color::Black;
                    uncle->mFields.mColor = Color::Black;

                    grandparent->mFields.mColor = Color::Red;

                    currentNode = grandparent;
                    continue;
                } else if (currentNode->_isRight () && grandparent->_isLeft ()) {
                    rotateRight (currentNode->getParent(), grandparent);

                    currentNode = currentNode->getRight ();
                } else if (currentNode->_isLeft() && currentNode->_isRight ()) {
                    rotateLeft (currentNode->getParent(), grandparent);

                    currentNode = currentNode->getRight ();
                }

                currentNode->mParent->mFields.mColor = Color::Black;
                grandparent->mFields.mColor          = Color::Red;

                if (currentNode->_isLeft ()) {
                    rotateRight (grandparent, grandparent->getParent ());
                } else {
                    rotateLeft (grandparent, grandparent->getParent ());
                }

                break;
            }
        }
    }

    Iterator remove(const KeyType &key)
    {
        IgnoreUnused (key);
    }

    Iterator erase(ConstIterator iterator)
    {
        IgnoreUnused (iterator);

    }
};

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_REDBLACKTREE_H
