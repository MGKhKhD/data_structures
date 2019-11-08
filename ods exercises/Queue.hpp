//
//  Queue.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-14.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Queue_h
#define Queue_h

#include <stdexcept>

template<typename T>
class Queue{
    struct Node{
        T info;
        Node* next=nullptr, *prev=nullptr;
    };
    Node* header=nullptr, *trailer=nullptr;
    int n;
    
    void swap(Queue<T> other) noexcept{
        std::swap(header, other.header);
        std::swap(trailer, other.trailer);
        std::swap(n, other.n);
    }
    
public:
    Queue(): header{new Node}, trailer{new Node}{
        header->next=trailer;
        trailer->prev=header;
        n=0;
    }
    
    Queue(const Queue<T>& q): Queue(){
        for(Node* node=q.trailer->prev; node!=q.header; enqueue(node->info), node=node->prev);
    }
    
    Queue<T>& operator=(const Queue<T>& q){
        if(this != &q){
            Queue<T> c{q};
            swap(c);
        }
        return *this;
    }
    
    Queue(Queue<T>&& q): header{q.header}, trailer{q.trailer}, n{q.n}{
//        delete q.header;
//        delete q.trailer;
        q.header=nullptr;
        q.trailer=nullptr;
    }
    
//    Queue<T>& operator=(Queue<T>&& q){
//        Queue<T> c{std::move(q)};
//        swap(c);
//        return *this;
//    }
    
    ~Queue(){
        while(!isEmpty()){
            dequeue();
        }
        delete header;
        delete trailer;
        header=nullptr;
        trailer=nullptr;
    }
    
    bool isEmpty( ) const {return header->next==trailer;}
    const int getSize() const{return n;}
    
    void enqueue(const T& t){
        Node* node=new Node;
        node->info=t;
        if(header->next==trailer){
            node->next=trailer;
            node->prev=header;
            header->next=node;
            trailer->prev=node;
        }else{
            node->next=header->next;
            node->prev=header;
            header->next->prev=node;
            header->next=node;
        }
        n++;
    }
    
    T dequeue(){
        T ret=static_cast<T>(NULL);
        if(header->next!=trailer){
            Node* node=trailer->prev;
            ret=node->info;
            if(node->prev==header){
                header->next=trailer;
                trailer->prev=header;
            }else{
                node->prev->next=trailer;
                trailer->prev=node->prev;
            }
            delete node;
            node=nullptr;
            n--;
        }
        return ret;
    }
    
};

#endif /* Queue_h */
