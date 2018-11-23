#ifndef NODE_H
#define NODE_H

class Node
{
    public:
        Node();
        Node(int key);
        ~Node();
        Node* getNext();
        int getKey();
        void setNext(Node* next);
        void setKey(int key);

    private:
        int key;    //or char[30]
        Node* next;
};


#endif //!NODE_H
