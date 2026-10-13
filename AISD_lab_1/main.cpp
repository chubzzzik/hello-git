#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cstdlib>
#include <memory>
#include<cctype>
#include <cmath>

using namespace std;

template <typename T>
class Stack; //"прототип" нужен для класса Node

template <typename T>
class Node{
    private:

    T data;
    Node* next;

    public:

    Node(T d) : data(d), next(nullptr){}
    friend class Stack<T>;
};

template <typename T>
class Stack {
    private:

    Node<T>* head;

    public:

    Stack():head(nullptr){}
    ~Stack(){
        while(head != nullptr){
            pop();
        }
    }

    void push(T data){
        Node<T>* newNode = new Node<T>(data);
        newNode->next = head;
        head = newNode;
    }

    void pop(){
        if(head == nullptr){
            return;
        }
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }

    T peek(){
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

template <typename T>
class DoublyLinkedList; // "прототип", необходимый для DNode

template <typename T>
class DNode{
    private:

    T data;
    DNode* next;
    DNode* prev;

    public:

    DNode(T d): data(d), next(nullptr), prev(nullptr){}
    friend class DoublyLinkedList<T>;
};

template <typename T>
class DoublyLinkedList{
    private:

    DNode<T>* head;
    DNode<T>* tail;

    public:

    DoublyLinkedList():head(nullptr),tail(nullptr){}

    ~DoublyLinkedList(){
        while(head != nullptr){
            pop_front();
        }
    }

    void push_front(T data){
        DNode<T>* newNode = new DNode<T>(data);
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
        DNode<T>* temp = head;
        head = head->next;
        if(head != nullptr){
            head->prev = nullptr;
        }
        else{
            tail = nullptr;
        }
        delete temp;  
    }

    void push_back(T data){
        DNode<T>* newNode = new DNode<T>(data);
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
        DNode<T>* temp = tail;
        tail = tail->prev;
        if(tail != nullptr){
            tail->next = nullptr;
        }
        else{
            head = nullptr;
        }
        delete temp;       
    }

    T front(){
        if(head == nullptr){
            throw std::runtime_error("List is empty");
        }
        return head->data;
    }

    T back(){
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

template <typename T>
class DynamicArray{ // начальный размер == 8
    private:
    T* data;
    int current_size;
    int capacity;

    void resize(){
        int new_capacity = capacity * 2;
        T* new_data = new T[new_capacity];
        for(int idx = 0; idx < current_size; idx+=1){
            new_data[idx] = data[idx];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    public:

    DynamicArray(): current_size(0), capacity(8){
    data = new T[capacity];
    }

    ~DynamicArray(){
        delete[] data;
    }

    void push_back(T value){
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

    T get(int index){
        if(index >= 0 && index < current_size){
            return data[index];
        }
        else{
            throw std::out_of_range("Wrong index");
        }
    }

    void set(int index, T value){
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


DynamicArray<string> token_slicer(){
    DynamicArray<string> tokens_array;
    string input_line;
    getline(cin,input_line);
    stringstream ss(input_line);
    string token;
    while(getline(ss,token,' ')){
        tokens_array.push_back(token);
    }
    return tokens_array;
}

int priority(const string& token){
    if(token == "+" || token == "-"){
        return 1;
    }
    if(token == "*" || token == "/"){
        return 2;
    }
    if(token == "^"){
        return 3;
    }
    if(token == "sin" || token == "cos"){
        return 4;
    }
    return 0;
}

bool is_digit(const string& token){
    int len = token.length();
    for(int idx = 0; idx < len; idx +=1){
        if(isdigit(token[idx]) == false){return false;}
    }
    return true;
}

DynamicArray<string> sort_station(DynamicArray<string>& input_array){
    DynamicArray<string> output_array;
    Stack<string> operator_stack;
    for(int idx = 0; idx < input_array.get_size(); idx += 1){
        string token = input_array.get(idx);
        if(is_digit(token)){
            output_array.push_back(token);
        }
        else if(token == "sin" || token == "cos"){
            operator_stack.push(token);
        }
        else if(token == "+" || token == "-" || token == "*" || token == "/" || token == "^"){

            while(operator_stack.is_empty() == false && operator_stack.peek() != "("){
                int head_pr = priority(operator_stack.peek());
                int token_pr = priority(token);
                if(head_pr > token_pr || (head_pr == token_pr  && token != "^")){
                    output_array.push_back(operator_stack.peek());
                    operator_stack.pop();
                }
                else{
                    break;
                }

            }
            operator_stack.push(token);

        }
        else if(token == "("){
            operator_stack.push(token);
        }
        else if(token == ")"){
            while(operator_stack.is_empty() == false && operator_stack.peek() != "("){
                output_array.push_back(operator_stack.peek());
                operator_stack.pop();
            }
            if(operator_stack.is_empty()){
                throw std::runtime_error("Mismatched parentheses");
            }
            operator_stack.pop();
            if(operator_stack.is_empty() == false && (operator_stack.peek() == "sin" || operator_stack.peek() == "cos")){
                output_array.push_back(operator_stack.peek());
                operator_stack.pop();
            }
        }
    }
    while(operator_stack.is_empty() == false){
        if(operator_stack.peek() == "("){
            throw std::runtime_error("Mismatched parentheses");
        }
        output_array.push_back(operator_stack.peek());
        operator_stack.pop();
    }
    return output_array;
}

double calculate_rpn(DynamicArray<string>& rpn_array){
    Stack<double> number_stack;
    for(int idx = 0; idx < rpn_array.get_size(); idx += 1){
        string token = rpn_array.get(idx);
        if(is_digit(token)){
            number_stack.push(stod(token));
        }
        else if(token == "sin" || token == "cos"){
            double a = number_stack.peek();
            number_stack.pop();
            if(token == "sin"){
                number_stack.push(sin(a));
            }
            else{
                number_stack.push(cos(a));
            }
        }
        else if(token == "+" || token == "-" || token == "*" || token == "/" || token == "^"){
            double b = number_stack.peek();
            number_stack.pop();
            double a = number_stack.peek();
            number_stack.pop();
            if(token == "+"){
                number_stack.push(a + b);
            }
            else if(token == "-"){
                number_stack.push(a - b);
            }
            else if(token == "*"){
                number_stack.push(a * b);
            }
            else if(token == "/"){
                if(b == 0){
                    throw std::runtime_error("Division by zero");
                }
                number_stack.push(a / b);
            }
            else{
                number_stack.push(pow(a, b));
            }
        }
    }
    double result = number_stack.peek();
    number_stack.pop();
    if(number_stack.is_empty() == false){
        throw std::runtime_error("Wrong expression");
    }
    return result;
}

int main()
{
    cout << "Введите строку вида: sin ( 1 + 2 ) / 3 + 52 * 67" << endl;
    DynamicArray<string> input_line = token_slicer();
    DynamicArray<string> output_line = sort_station(input_line);
    cout << "\nСтрока в виде RPN: ";
    for(int idx = 0; idx < output_line.get_size(); idx += 1){
        cout <<output_line.get(idx) << " ";
    }
    cout << "\nОтвет: " << calculate_rpn(output_line) << endl;
    return 0;
}