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


class DynamicArray{ // начальный размер == 8
    private:
    double* data;
    int current_size;
    int capacity;

    void resize(){
        int new_capacity = capacity * 2;
        double* new_data = new double[new_capacity];
        for(int idx = 0; idx < current_size; idx+=1){
            new_data[idx] = data[idx];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    public:

    DynamicArray(): current_size(0), capacity(8){
    data = new double[capacity];
    }

    ~DynamicArray(){
        delete[] data;
    }

    void push_back(double value){
        if(current_size == capacity){
            resize();
        }
        data[current_size] = value;
        current_size += 1;
    }

    void pop_back(){
        if(current_size == 0){return;}
        current_size = current_size - 1;
    }

    double get(int index){
        if(index >= 0 && index < current_size){
            return data[index];
        }
        else{
            throw std::out_of_range("Wrong index");
        }
    }

    void set(int index, double value){
        if(index >= 0 && index < current_size){
            data[index] = value;
        }
        else{
            throw std::out_of_range("Wrong index");
        }
    }

    int get_size() const{
        return current_size;
    }

    bool is_empty(){
        if(current_size == 0){return true;}
        else{return false;}
    }
};
int main()
{

    return 0;
}
