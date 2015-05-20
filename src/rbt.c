#include <stdlib.h>
#include "librune.h"

const rn_uchar rn_RED = 0;
const rn_uchar rn_BLACK = 1;

/**
 * Default rbt handlers.
 * By default, rbt handle it's key as a sint pointer.
 * By default, rbt handle it's value as a structure made with uint byte length
 * and bytes whose length is that uint value.
 */

rn_sint rn_defaultRBTCompare(rn_mem oldKey, rn_mem newKey)
{
    rn_sint old = *((rn_sint*) oldKey);
    rn_sint new = *((rn_sint*) newKey);
    return (rn_sint) (new - old);
}

void rn_defaultRBTKeyFree(rn_mem _key)
{
    rn_sint* key = (rn_sint*) _key;
    free(key);
}

void rn_defaultRBTValueFree(rn_mem _value)
{
    int i;
    rn_DefaultRBTValue* value = (rn_DefaultRBTValue*) _value;
    rn_uint length = value->length;
    for (i = 0; i < length; i++)
    {
        free(value->data[i]);
    }
    free(value);
}

rn_RBTNode rn_DefaultRBTNode = {0, 0, 0, 0, 0, 0};
rn_RBTBody rn_DefaultRBTBody = {0, rn_defaultRBTCompare, rn_defaultRBTKeyFree, rn_defaultRBTValueFree};

rn_rbt rn_rbtNew()
{
    rn_rbt rbt = malloc(sizeof(rn_RBTBody));
    *rbt = rn_DefaultRBTBody;
    return rbt;
}

rn_uint rn_rbtSizeSubTree(rn_RBTNode* node)
{
    if (!node) return (rn_uint) 0;
    return (rn_uint) (rn_rbtSizeSubTree(node->left) + (rn_uint) 1 + rn_rbtSizeSubTree(node->right));
}

rn_uint rn_rbtSize(rn_rbt rbt)
{
    return rn_rbtSizeSubTree(rbt->root);
}

rn_mem rn_rbtGetSubTree(rn_rbt rbt, rn_RBTNode* node, rn_mem key)
{
    rn_sint flag;

    if (!node) return rn_null;

    flag = rbt->compare(node->key, key);

    /* if node's key is equal to the key */
    if (!flag)
    {
        return node->value;
    }

    /* if node's key is lower then the key */
    if (flag < 0)
    {
        return rn_rbtGetSubTree(rbt, node->left, key);
    }

    /* if node's key is higher then the key */
    return rn_rbtGetSubTree(rbt, node->right, key);
}

rn_mem rn_rbtGet(rn_rbt rbt, rn_mem key)
{
    return rn_rbtGetSubTree(rbt, rbt->root, key);
}

rn_RBTNode* rn_rbtNewNode(rn_mem key, rn_mem value)
{
    rn_RBTNode* node = malloc(sizeof(rn_RBTNode));
    *node = rn_DefaultRBTNode;
    node->key = key;
    node->value = value;
    return node;
}

void rn_rbtRotateLeft(rn_rbt rbt, rn_RBTNode* node)
{
    rn_RBTNode* childRight;

    if (!node || !node->right) return;

    childRight = node->right;

    /* if node is root node */
    if (!node->up)
    {
        rbt->root = childRight;
        childRight->up = rn_null;
    }

    /* if node is it's parent node's left child */
    else if (node->up->left == node)
    {
        node->up->left = childRight;
        childRight->up = node->up;
    }

    /* if node is it's parent node's right child */
    else
    {
        node->up->right = childRight;
        childRight->up = node->up;
    }

    node->right = childRight->left;
    if (childRight->left) childRight->left->up = node;

    node->up = childRight;
    childRight->left = node;
}

void rn_rbtRotateRight(rn_rbt rbt, rn_RBTNode* node)
{
    rn_RBTNode* childLeft;

    if (!node || !node->right) return;

    childLeft = node->left;

    /* if node is root node */
    if (!node->up)
    {
        rbt->root = childLeft;
        childLeft->up = rn_null;
    }

    /* if node is it's parent node's left child */
    else if (node->up->left == node)
    {
        node->up->left = childLeft;
        childLeft->up = node->up;
    }

    /* if node is it's parent node's right child */
    else
    {
        node->up->right = childLeft;
        childLeft->up = node->up;
    }

    node->left = childLeft->right;
    if (childLeft->right) childLeft->right->up = node;

    node->up = childLeft;
    childLeft->right = node;
}

void rn_rbtBreakDoubleRed(rn_rbt rbt, rn_RBTNode* node)
{
    rn_RBTNode* uncle;
    rn_uchar uncleColor;
    rn_bool isLeft, isParentLeft;
    /* if node is root */
    if (!node->up)
    {
        node->color = rn_BLACK;
        return;
    }

    /* if parent node is black */
    if (node->up->color) return;

    /* if this node is parent's left child */
    if (node->up->left == node)
    {
        isLeft = rn_true;
    }
    /* if this node is parent's right child */
    else
    {
        isLeft = rn_false;
    }

    /* if parent node is grand parent node's left */
    if (node->up->up->left == node->up)
    {
        isParentLeft = rn_true;
        uncle = node->up->up->right;
        if (uncle) uncleColor = uncle->color;
    }
    /* if parent node is grand parent node's right */
    else
    {
        isParentLeft = rn_false;
        uncle = node->up->up->left;
        if (uncle) uncleColor = uncle->color;
    }

    /* if uncle's color is black */
    if (!uncle || uncleColor)
    {
        if (isParentLeft)
        {
            if (isLeft)
            {
                /*
                 *         B
                 *        / \
                 *       R   B
                 *      /
                 *     N
                 */
                rn_rbtRotateRight(rbt, node->up->up);
                node->up->color = rn_BLACK;
                node->up->right->color = rn_RED;
            }
            else
            {
                /*
                 *         B
                 *        / \
                 *       R   B
                 *        \
                 *         N
                 */
                rn_rbtRotateLeft(rbt, node->up);
                rn_rbtRotateRight(rbt, node->up);
                node->color = rn_BLACK;
                node->right->color = rn_RED;
            }
        }
        else
        {
            if (isLeft)
            {
                /*
                 *         B
                 *        / \
                 *       B   R
                 *          /
                 *         N
                 */
                rn_rbtRotateRight(rbt, node->up);
                rn_rbtRotateLeft(rbt, node->up);
                node->color = rn_BLACK;
                node->left->color = rn_RED;
            }
            else
            {
                /*
                 *         B
                 *        / \
                 *       B   R
                 *            \
                 *             N
                 */
                rn_rbtRotateLeft(rbt, node->up->up);
                node->up->color = rn_BLACK;
                node->up->left->color = rn_RED;
            }
        }
        return;
    }
    //if uncle's color is red
    /*
     *         B       B
     *        / \     / \
     *       R   R   R   R
     *      /             \
     *     N               N
     */
    /* set grand parent's color to red */
    node->up->up->color = rn_RED;
    /* set parent's color to black */
    node->up->color = rn_BLACK;
    /* set uncle's color to black */
    if (isParentLeft)
    {
        node->up->up->right->color = rn_BLACK;
    }
    else
    {
        node->up->up->left->color  = rn_BLACK;
    }

    /**
     * now this node's grandparent is red
     * so repeat this process recursively
     */
    rn_rbtBreakDoubleRed(rbt, node->up->up);
}

void rn_rbtInsertSubTree(rn_rbt rbt, rn_RBTNode* node, rn_mem key, rn_mem value)
{
    rn_sint flag;
    rn_RBTNode* newNode;

    if (!node) return;

    flag = rbt->compare(node->key, key);

    /* if key == node->key */
    if (!flag)
    {
        node->key = key;
        node->value = value;
        return;
    }

    /* if key < node->key */
    if (flag < 0)
    {
        if (node->left) return rn_rbtInsertSubTree(rbt, node->left, key, value);

        newNode = rn_rbtNewNode(key, value);
        node->left = newNode;
        newNode->up = node;
        rn_rbtBreakDoubleRed(rbt, newNode);
    }
    /* if key > node->key */
    else
    {
        if (node->right) return rn_rbtInsertSubTree(rbt, node->right, key, value);

        newNode = rn_rbtNewNode(key, value);
        node->right = newNode;
        newNode->up = node;
        rn_rbtBreakDoubleRed(rbt, newNode);
    }
}

void rn_rbtInsert(rn_rbt rbt, rn_mem key, rn_mem value)
{
    rn_RBTNode* node;

    if (rbt->root) return rn_rbtInsertSubTree(rbt, rbt->root, key, value);

    node = rn_rbtNewNode(key, value);
    node->color = rn_BLACK;
    rbt->root = node;
}

void rn_rbtFreeNode(rn_rbt rbt, rn_RBTNode* node)
{
    rbt->keyFree(node->key);
    rbt->valueFree(node->value);
    free(node);
}

void rn_rbtGiveItAnotherBlack(rn_rbt rbt, rn_RBTNode* node)
{
    rn_RBTNode* parent = node->up;
    rn_RBTNode* cousin;
    rn_bool isLeftChild;

    /* if node is root */
    if (!parent) return;

    /* if node is red */
    if (!node->color) return;

    /* if node has only left child and it's color is red */
    if (!node->right && node->left && !node->left->color)
    {
        node->left->color = rn_BLACK;
        return;
    }

    /* if node has only right child and it's color is red */
    if (!node->left && node->right && !node->right->color)
    {
        node->right->color = rn_BLACK;
        return;
    }

    if (parent->left == node)
    {
        isLeftChild = rn_true;
        cousin = parent->right;
    }
    else
    {
        isLeftChild = rn_false;
        cousin = parent->left;
    }

    /* if cousin is red */
    if (!cousin->color)
    {
        if (isLeftChild)
        {
            rn_rbtRotateLeft(rbt, parent);
            cousin = parent->right;
        }
        else
        {
            rn_rbtRotateRight(rbt, parent);
            cousin = parent->left;
        }
    }
    /* now cousin is black */

    if (isLeftChild)
    {
        /* if far side nephew is red */
        if (cousin->right && !cousin->right->color)
        {
            rn_rbtRotateLeft(rbt, parent);
            cousin->color = parent->color;
            parent->color = rn_BLACK;
            cousin->right->color = rn_BLACK;
            return;
        }

        /* if near side nephew is red */
        if (cousin->left && !cousin->left->color)
        {
            rn_rbtRotateRight(rbt, cousin);
            cousin->color = rn_RED;
            cousin = parent->right;
            cousin->color = rn_BLACK;

            /* now far side nephew is red */
            rn_rbtRotateLeft(rbt, parent);
            cousin->color = parent->color;
            parent->color = rn_BLACK;
            cousin->right->color = rn_BLACK;
            return;
        }
    }
    else
    {
        /* if far side nephew is red */
        if (cousin->left && !cousin->left->color)
        {
            rn_rbtRotateRight(rbt, parent);
            cousin->color = parent->color;
            parent->color = rn_BLACK;
            cousin->left->color = rn_BLACK;
            return;
        }

        /* if near side nephew is red */
        if (cousin->right && !cousin->right->color)
        {
            rn_rbtRotateLeft(rbt, cousin);
            cousin->color = rn_RED;
            cousin = parent->left;
            cousin->color = rn_BLACK;

            /* now far side nephew is red */
            rn_rbtRotateRight(rbt, parent);
            cousin->color = parent->color;
            parent->color = rn_BLACK;
            cousin->left->color = rn_BLACK;
            return;
        }
    }
    /* now cousin and all nephews are black */

    /* if parent is red */
    if (!parent->color)
    {
        parent->color = rn_BLACK;
        cousin->color = rn_RED;
        return;
    }

    /* now parent, cousin, even nephews are all black */
    cousin->color = rn_RED;
    rn_rbtGiveItAnotherBlack(rbt, parent);
}

void rn_rbtDeleteNode(rn_rbt rbt, rn_RBTNode* node)
{
    rn_uint redDistanceLeft, redDistanceRight;
    rn_RBTNode* parent;
    rn_RBTNode* successor;
    rn_RBTNode* successorRight;
    rn_bool isLeftChild = rn_false;

    if (!node) return;

    parent = node->up;

    if (parent && parent->left == node) isLeftChild = rn_true;

    /*
     *        N
     *       / \
     *      X   X
     */
    if (!node->left && !node->right)
    {
        if (!parent)
        {
            rbt->root = rn_null;
            return rn_rbtFreeNode(rbt, node);
        }

        rn_rbtGiveItAnotherBlack(rbt, node);

        if (isLeftChild)
        {
            node->up->left = rn_null;
        }
        else
        {
            node->up->right = rn_null;
        }

        return rn_rbtFreeNode(rbt, node);
    }

    /*
     *        N
     *       / \
     *      O   X
     */
    if (!node->right)
    {
        if (!parent)
        {
            rbt->root = node->left;
            node->left->up = rn_null;
            node->left->color = rn_BLACK;
            return rn_rbtFreeNode(rbt, node);
        }

        rn_rbtGiveItAnotherBlack(rbt, node);

        if (isLeftChild)
        {
            node->up->left = node->left;
        }
        else
        {
            node->up->right = node->left;
        }
        node->left->up = node->up;
        return rn_rbtFreeNode(rbt, node);
    }

    /*
     *        N
     *       / \
     *      X   O
     */
    if (!node->left)
    {
        if (!parent)
        {
            rbt->root = node->right;
            node->right->up = rn_null;
            node->right->color = rn_BLACK;
            return rn_rbtFreeNode(rbt, node);
        }

        rn_rbtGiveItAnotherBlack(rbt, node);

        if (isLeftChild)
        {
            node->up->left = node->right;
        }
        else
        {
            node->up->right = node->right;
        }
        node->right->up = node->up;
        return rn_rbtFreeNode(rbt, node);
    }

    /*
     *        N
     *       / \
     *      O   O
     */

    successor = node->left;

    /* if successor's color is black */
    if (successor->color)
    {
        redDistanceLeft = 1;
    }
    else
    {
        redDistanceLeft = 0;
    }

    while (successor->right)
    {
        successor = successor->right;
        /* if successor's color is black */
        if (successor->color)
        {
            redDistanceLeft++;
        }
        else
        {
            redDistanceLeft = 0;
        }
    }

    /* if successor's color is red */
    if (!successor->color)
    {
        node->key = successor->key;
        node->value = successor->value;
        return rn_rbtDeleteNode(rbt, successor);
    }

    successorRight = node->right;

    /* if successorRight's color is black */
    if (successorRight->color)
    {
        redDistanceRight = 1;
    }
    else
    {
        redDistanceRight = 0;
    }

    while (successorRight->left)
    {
        successorRight = successorRight->left;

        /* if successorRight's color is black */
        if (successorRight->color)
        {
            redDistanceRight++;
        }
        else
        {
            redDistanceRight = 0;
        }
    }

    if (redDistanceRight < redDistanceLeft)
    {
        successor = successorRight;
    }

    node->key = successor->key;
    node->value = successor->value;
    rn_rbtDeleteNode(rbt, successor);
}

rn_mem rn_rbtDeleteAtSubTree(rn_rbt rbt, rn_RBTNode* node, rn_mem key)
{
    rn_sint flag;
    rn_mem value;

    if (!node) return rn_null;

    flag = rbt->compare(node->key, key);

    /* if key == node->key */
    if (!flag)
    {
        value = node->value;
        rn_rbtDeleteNode(rbt, node);
        return value;
    }

    /* if key < node->key */
    if (flag < 0)
    {
        return rn_rbtDeleteAtSubTree(rbt, node->left, key);
    }
    /* if key > node->key */
    else
    {
        return rn_rbtDeleteAtSubTree(rbt, node->right, key);
    }
}

rn_mem rn_rbtDelete(rn_rbt rbt, rn_mem key)
{
    return rn_rbtDeleteAtSubTree(rbt, rbt->root, key);
}

rn_uint rn_rbtForEachNode(rn_RBTForEachHandler handler, rn_RBTNode* node, rn_uint count, rn_mem data)
{
    if (!node) return (rn_uint)0;
    count = rn_rbtForEachNode(handler, node->left, count, data);
    handler(count++, node->key, node->value, data);
    return rn_rbtForEachNode(handler, node->right, count, data);
}

void rn_rbtForEach(rn_rbt rbt, rn_RBTForEachHandler handler, rn_mem data)
{
    rn_rbtForEachNode(handler, rbt->root, (rn_uint)0, data);
}

void rn_rbtFreeNodeRecursive(rn_rbt rbt, rn_RBTNode* node)
{
    if (!node) return;
    rn_rbtFreeNodeRecursive(rbt, node->left);
    rn_rbtFreeNodeRecursive(rbt, node->right);
    rn_rbtFreeNode(rbt, node);
}

void rn_rbtFree(rn_rbt rbt)
{
    rn_rbtFreeNodeRecursive(rbt, rbt->root);
    free(rbt);
}
