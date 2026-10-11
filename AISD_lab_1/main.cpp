#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <memory>


class Stack; //"прототип" нужен для класса Node

class Node{
    private:

    double data;
    Node* next;

    public:

    Node(double d) : data(d), next(nullptr){}
    friend class Stack;
};

class Stack {
    private:

    Node* head;

    public:

    Stack():head(nullptr){}
    ~Stack(){
        while(head != nullptr){
            pop();
        }
    }

    void push(double data){
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    void pop(){
        if(head == nullptr){
            return;
        }
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    double peek(){
        if(head == nullptr){
            throw std::runtime_error("Stack is empty");
        }
        return head->data;

    }

    bool is_empty(){
        if(head == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
};

class DoublyLinkedList; // "прототип", необходимый для DNode

class DNode{
    private:

    double data;
    DNode* next;
    DNode* prev;

    public:

    DNode(double d): data(d), next(nullptr), prev(nullptr){}
    friend class DoublyLinkedList;
};

class DoublyLinkedList{
    private:

    DNode* head;
    DNode* tail;

    public:

    DoublyLinkedList():head(nullptr),tail(nullptr){}

    ~DoublyLinkedList(){
        while(head != nullptr){
            pop_front();
        }
    }

    void push_front(double data){
        DNode* newNode = new DNode(data);
        newNode->next = head;
        if(head != nullptr){
            head->prev = newNode;
        }
        else{
            tail = newNode;
        }
        head = newNode;
    }

    void pop_front(){
        if(head == nullptr){
            return;
        }
        DNode* temp = head;
        head = head->next;
        if(head != nullptr){
            head->prev = nullptr;
        }
        else{
            tail = nullptr;
        }
        delete temp;  
    }

    void push_back(double data){
        DNode* newNode = new DNode(data);
        newNode->prev = tail;
        if(tail != nullptr){
            tail->next = newNode;
        }
        else{
            head = newNode;
        }
        tail = newNode;
    }

    void pop_back(){
        if(tail == nullptr){
            return;
        }
        DNode* temp = tail;
        tail = tail->prev;
        if(tail != nullptr){
            tail->next = nullptr;
        }
        else{
            head = nullptr;
        }
        delete temp;       
    }

    double front(){
        if(head == nullptr){
            throw std::runtime_error("List is empty");
        }
        return head->data;
    }

    double back(){
        if(tail == nullptr){
            throw std::runtime_error("List is empty");
        }
        return tail->data;
    }

    bool is_empty(){
        if(head == nullptr && tail == nullptr){
            return true;
        }
        return false;
    }
};

int main()
{

    return 0;
}
