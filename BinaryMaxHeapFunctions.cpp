#include "Headers.hpp"
#include "BinaryMaxHeap.hpp"


//--------------- BinaryMaxNode ---------------

BinaryMaxNode::BinaryMaxNode()
{
    leftChild = NULL;
    rightChild = NULL;
    parent = NULL;
    //cout << "------->  BinaryMaxNode initialized successfully!" << endl << endl;
}


BinaryMaxNode::~BinaryMaxNode()
{
    // delete leftChild;
    // delete rightChild;
    // delete parent;
}


char* BinaryMaxNode::getTelNum()
{
    return telnum;
}


float BinaryMaxNode::getCost()
{
    return cost;
}


BinaryMaxNode*  BinaryMaxNode::getRightChild()
{
    return rightChild;
}


BinaryMaxNode*  BinaryMaxNode::getLeftChild()
{
    return leftChild;
}


BinaryMaxNode*  BinaryMaxNode::getParent()
{
    return parent;
}


void BinaryMaxNode::setTelNum(char* telnum)
{
    strcpy(this->telnum, telnum);
}


void BinaryMaxNode::setCost(float cost)
{
    this->cost = cost;
}


void  BinaryMaxNode::setLeftChild(BinaryMaxNode* leftChild)
{
    this->leftChild = leftChild;
}


void  BinaryMaxNode::setRightChild(BinaryMaxNode* rightChild)
{
    this->rightChild = rightChild;
}


void  BinaryMaxNode::setParent(BinaryMaxNode* parent)
{
    //cout << "in set parent??" << endl;
    if (this->parent != NULL)
        this->parent = parent;
}


//--------------- BinaryMaxHeap ---------------


BinaryMaxHeap::BinaryMaxHeap()
{
    this->root = NULL;
    this->nodeCount = 0;
    cout << "------->  BinaryMaxHeap initialized successfully!" << endl << endl;
}


BinaryMaxHeap::~BinaryMaxHeap()
{
    DestroyHeap(root);
    //cout << "------->  Finished processing the BinaryMaxHeap." << endl;
}


BinaryMaxNode* BinaryMaxHeap::getRoot()
{
    return root;
}


int BinaryMaxHeap::getNodeCount()
{
    return nodeCount;
}


void BinaryMaxHeap::setRoot(BinaryMaxNode* root)
{
    this->root = root;
}


void BinaryMaxHeap::setNodeCount(int nodeCount)
{
    this->nodeCount = nodeCount;
}


void BinaryMaxHeap::Insert(BinaryMaxNode* newNode)
{
    BinaryMaxNode* currentNode;
    BinaryMaxNode* tempNode = root;
    BinaryMaxNode* parentNode = NULL;
    int tempCost;
    int index;
    short mask = 0x80;
    char tempTel[15];

    BinaryMaxNode* testNode = Exists(newNode->getTelNum(), this->root);
    if (testNode != NULL)
    {
        cout << "        Node already exists : " << newNode->getTelNum() << ", adding to total cost." << endl;
        testNode->setCost(newNode->getCost() + testNode->getCost());
        //Update
        // if (testNode->getParent() != NULL)
        // {
        //     parentNode = testNode->getParent();
        //     if (testNode->getCost() > parentNode->getCost())               
        //     {
        //         cout << "        Swapping nodes." << endl;
        //         tempCost = parentNode->getCost();
        //         parentNode->setCost(testNode->getCost());
        //         testNode->setCost(tempCost);
        //         strcpy(tempTel, parentNode->getTelNum());
        //         parentNode->setTelNum(testNode->getTelNum());
        //         parentNode->setTelNum(tempTel);
        //     }
        // }
        return;
    }

    currentNode = new BinaryMaxNode();
    nodeCount++; 
    index = nodeCount;
    while (!(index & mask)) index <<= 1;    
    index <<= 1;

    while (tempNode)
    {
        if (newNode->getCost() > tempNode->getCost())
        {
            tempCost = tempNode->getCost();
            tempNode->setCost(newNode->getCost());
            newNode->setCost(tempCost);
        }
        parentNode = tempNode;
        if (!(index & mask)) 
        {
            tempNode = tempNode->getLeftChild(); 
        }
        else
        {
            tempNode = tempNode->getRightChild();
        }

        if (tempNode)
        {
            index <<= 1;
        }
    }

    currentNode->setLeftChild(NULL);
    currentNode->setRightChild(NULL);
    currentNode->setCost(newNode->getCost());

    if(!parentNode)
    {
        root = currentNode;
        return;
    }

    if (!(index & mask))
    { 
        newNode->setParent(parentNode);
        parentNode->setLeftChild(newNode);
    }
    else 
    {
        newNode->setParent(parentNode);
        parentNode->setRightChild(newNode);
    }
}



void BinaryMaxHeap::DestroyHeap(BinaryMaxNode* node)
{
    if(node != NULL)
    {
        DestroyHeap(node->getLeftChild());
        DestroyHeap(node->getRightChild());
        delete node;
    }
}



BinaryMaxNode* BinaryMaxHeap::Exists(char* key, BinaryMaxNode* node)
{
    if(node != NULL)
    {
        if (strcmp(key, node->getTelNum()) == 0)
        {
            return node;
        }
        if (Exists(key, node->getLeftChild()) == NULL)
        {
            Exists(key, node->getRightChild());
        }   
    }
    else 
    {
        return NULL;
    }
}



float BinaryMaxHeap::Traverse(BinaryMaxNode* node) 
{
    float total_income = 0;
    float total_income_l = 0;
    float total_income_r = 0;
    if(node != NULL) 
    {
        //cout << "        Cost : " << node->getCost() << endl;
        total_income += node->getCost();
        total_income_l = Traverse(node->getLeftChild());
        total_income_r = Traverse(node->getRightChild());
        total_income = total_income_l + total_income_r + total_income;
        
    }

    return total_income;
}



void BinaryMaxHeap::TopSubs(BinaryMaxNode* node, BinaryMaxNode* altNode, float rpercentage)
{
    float remainder;

    if (node == NULL)
    {
        cout << "        Root error. Returning..." << endl;
    }
    else
    {
        remainder = rpercentage - node->getCost();
        cout << "        Node number : " <<  node->getTelNum() << " with cost : " << node->getCost() << endl;
        if (remainder > 0)
        {
            if ((node->getLeftChild() != NULL) && (node->getRightChild() != NULL))
            {
                //cout << "        Left cost : " << node->getLeftChild()->getCost() << " and Right Cost : " << node->getRightChild()->getCost() << endl;
                if ((node->getLeftChild()->getCost() == 0) && (node->getRightChild()->getCost() == 0) && (altNode != NULL)) 
                {
                    TopSubs(altNode, NULL, remainder);
                }
                if (node->getLeftChild()->getCost() > node->getRightChild()->getCost())
                {
                    TopSubs(node->getLeftChild(), node->getRightChild(), remainder);
                }
                else if (node->getLeftChild()->getCost() < node->getRightChild()->getCost())
                {
                    TopSubs(node->getRightChild(), node->getLeftChild(), remainder);
                }
                else
                {
                    TopSubs(node->getLeftChild(), node->getRightChild(), remainder);
                }
            }
            else if ((node->getLeftChild() == NULL) && (node->getRightChild() != NULL))
            {
                TopSubs(node->getRightChild(), NULL, remainder);
            }
            else if ((node->getLeftChild() != NULL) && (node->getRightChild() == NULL))
            {
                TopSubs(node->getLeftChild(), NULL, remainder);
            }
            else
            {
                //cout << "        Childs are NULL." << endl;
                return;
            }
        }
        else
        {
            //cout << "        Remainder equal or lower than zero : " << remainder << endl;
            return;
        }
    }
}