//
//  Stack.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-11.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Stack_h
#define Stack_h

#include <iostream>
#include <iomanip>
#include <stdexcept>

template<typename T>
class Stack{
    struct Node{
        T info;
        Node* next;
    };
    Node* head;
    int n;
    
public:
    Stack(): head{nullptr}, n{0} {}
    
    Stack(const Stack<T>& s): Stack(){
        Stack<T> tmp;
        for(Node* node=s.head; node; tmp.push(node->info) ,node=node->next);
        for(Node* node=tmp.head; node; push(node->info), node=node->next);
    }
    
    Stack<T>& operator=(const Stack<T>& s){
        if(this != &s){
            Stack<T> c{s};
            swap(c);
        }
        return *this;
    }
    
    Stack(Stack<T>&& s): head{s.head}, n{s.n}{
        s.head=nullptr;
    }
    
    Stack<T>& operator=(Stack<T>&& s){
        Stack<T> c{std::move(s)};
        swap(c);
        return *this;
    }
    
    ~Stack(){
        while(!isEmpty())
            pop();
    }
    
    void push(const T& t){
        Node* node=new Node;
        node->info=t;
        node->next=nullptr;
        if(!head){
            head=node;
        }else{
            node->next=head;
            head=node;
        }
        n++;
    }
    
    bool isEmpty() const{
        return head==nullptr;
    }
    
    const int size() const {return  n;}
    
    T top() const{
        assert(head!=nullptr);
        return head->info;
    }
    
    void pop(){
        if(head->next==nullptr){
            delete head;
            head=nullptr;
        }else{
            Node* tmp=head->next;
            delete head;
            head=tmp;
        }
        n--;
    }
    
    void swap(Stack<T>& other) noexcept{
        std::swap(other.head, head);
        std::swap(other.n, n);
    }
};


#endif /* Stack_h */
