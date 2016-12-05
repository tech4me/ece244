#include "TreeDB.h"
#include <iostream>

void TreeDB::resetCount() const
{
    probesCount = 0;
    activeCount = 0;
}

bool TreeDB::recursionInsert(TreeNode* ptr, TreeNode* pre_ptr, void (TreeNode::*func_ptr) (TreeNode*), DBentry* entry)
{
    if (ptr == NULL)
    {
        if (pre_ptr == NULL)
        {
            root = new TreeNode(entry);
            return false;
        }
        (pre_ptr->*func_ptr)(new TreeNode(entry));
        return false;
    }
    if (ptr->getEntry()->getName() == entry->getName())
        return true;
    else if (ptr->getEntry()->getName() > entry->getName())
        return recursionInsert(ptr->getLeft(), ptr, &TreeNode::setLeft, entry);
    else
        return recursionInsert(ptr->getRight(), ptr, &TreeNode::setRight, entry);
}

DBentry* TreeDB::recursionFind(TreeNode* ptr, std::string name) const
{
    if (ptr == NULL)
        return NULL;
    if (ptr->getEntry()->getName() == name)
        return ptr->getEntry();
    else if (ptr->getEntry()->getName() > name)
    {
        ++probesCount;
        return recursionFind(ptr->getLeft(), name);
    }
    else
    {
        ++probesCount;
        return recursionFind(ptr->getRight(), name);
    }
}

TreeDB::twoNodes TreeDB::prePtrFind(TreeNode* ptr, TreeNode* pre_ptr, std::string name, void (TreeNode::* const & func_ptr) (TreeNode*)) const
{
    if (ptr == NULL)
    {
        return twoNodes(NULL, NULL, NULL);
    }
    if (ptr->getEntry()->getName() == name)
        return twoNodes(ptr, pre_ptr, func_ptr);
    else if (ptr->getEntry()->getName() > name)
        return prePtrFind(ptr->getLeft(), ptr, name, &TreeNode::setLeft);
    else
        return prePtrFind(ptr->getRight(), ptr, name, &TreeNode::setRight);
}

void TreeDB::inorderPrint(TreeNode* ptr) const
{
    if (ptr != NULL)
    {
        inorderPrint(ptr->getLeft());
        std::cout << ptr;
        inorderPrint(ptr->getRight());
    }
}

void TreeDB::inorderactiveCount(TreeNode* ptr) const
{
    if (ptr != NULL)
    {
        inorderactiveCount(ptr->getLeft());
        if (ptr->getEntry()->getActive())
            ++activeCount;
        inorderactiveCount(ptr->getRight());
    }
}

void TreeDB::postorderClear(TreeNode* ptr)
{
    if (ptr != NULL)
    {
        postorderClear(ptr->getLeft());
        postorderClear(ptr->getRight());
        delete ptr;
    }
}

TreeDB::TreeDB()
    : root(NULL)
    , probesCount(0)
    , activeCount(0)
{

}

TreeDB::~TreeDB()
{
    clear();
}

bool TreeDB::insert(DBentry* newEntry)
{
    return recursionInsert(root, root, NULL, newEntry);
}

DBentry* TreeDB::find(string name)
{
    resetCount();
    return recursionFind(root, name);
}

bool TreeDB::remove(string name)
{
    TreeDB::twoNodes temp = prePtrFind(root, NULL, name, NULL);
    TreeNode* ptr = temp.currNode;
    TreeNode* pre_ptr = temp.prevNode;
    void (TreeNode::*func_ptr) (TreeNode*) = temp.func_ptr;
    if (ptr == NULL)
        return false;
    else if ((ptr->getLeft() == NULL) && (ptr->getRight() == NULL))
    {
        if (pre_ptr == NULL) // When removing the root node
            root = NULL;
        else
            (pre_ptr->*func_ptr)(NULL);
        delete ptr;
        return true;
    }
    else if ((ptr->getLeft() == NULL) ^ (ptr->getRight() == NULL))
    {
        TreeNode* temp = (ptr->getLeft() == NULL) ? ptr->getRight() : ptr->getLeft();
        if (pre_ptr == NULL) // When removing the root node
            root = temp;
        else
            (pre_ptr->*func_ptr)(temp);
        delete ptr;
        return true;
    }
    else
    {
        TreeNode* temp_left = ptr->getLeft();
        TreeNode* temp_right = ptr->getRight();
        TreeNode* pre_temp = temp_left;
        TreeNode* pre_pre_temp = ptr;
        while (temp_left != NULL)
        {
            pre_pre_temp = pre_temp;
            pre_temp = temp_left;
            temp_left = temp_left->getRight();
        }
        if (pre_ptr == NULL) // When removing the root node
            root = pre_temp;
        else
            (pre_ptr->*func_ptr)(pre_temp);
        pre_pre_temp->setRight(NULL); // Set the end of the Node to NULL
        TreeNode* pre_temp_most_left = pre_temp->getLeft();
        TreeNode* pre_pre_temp_most_left = pre_temp->getLeft();
        while (pre_temp_most_left != NULL)
        {
            pre_pre_temp_most_left = pre_temp_most_left;
            pre_temp_most_left = pre_temp_most_left->getLeft();
        }
        pre_pre_temp_most_left->setLeft(ptr->getLeft());
        pre_temp->setRight(temp_right);
        delete ptr;
        return true;
    }
}

void TreeDB::clear()
{
    postorderClear(root);
    root = NULL;
}

void TreeDB::printProbes() const
{
    std::cout << probesCount + 1 << std::endl;
}

void TreeDB::countActive() const
{
    resetCount();
    inorderactiveCount(root);
    std::cout << activeCount << std::endl;
}

ostream& operator<< (ostream& out, const TreeDB& rhs)
{
    rhs.inorderPrint(rhs.root);
    return out;
}

ostream& operator<< (ostream& out, TreeNode* rhs)
{
    return out << *(rhs->getEntry());
}