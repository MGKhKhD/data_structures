//
//  Dlink.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-25.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Dlink_h
#define Dlink_h

#include <iostream>
#include <iomanip>
#include <stdexcept>

template<typename T>
class Dlink{
    struct Node{
        T info;
        Node* next, *prev;
        
        explicit Node(const T& t): info{t}, next{nullptr}, prev{nullptr} {}
    };
    Node* header;
    Node* trailer;
    int n;
    
    std::ostream& printList(std::ostream& out) const;
    void destoryList();
    
    bool addInEmptyList(Node* node){
        if(header->next==trailer){
            header->next=node;
            trailer->prev=node;
            node->prev=header;
            node->next=trailer;
            return true;
        }
        return false;
    }
    
    T removeFromListWithSingleEntry(){
        T res=static_cast<T>(NULL);
        Node* node=header->next;
        if(node->next==trailer){
            res=node->info;
            trailer->prev=header;
            header->next=trailer;
            delete node;
            node=nullptr;
            n--;
        }
        return res;
    }
    
    Node* findNode(const T& t) const{
        if(header->next!=trailer){
            bool found{false};
            for(Node* node=header->next; node!=trailer && !found; node=node->next){
                if(node->info==t) {
                    return node;
                }
            }
        }
        return nullptr;
    }
    
    void swap(Dlink<T>& dl) noexcept{
        std::swap(header, dl.header);
        std::swap(trailer, dl.trailer);
        std::swap(n, dl.n);
    }
    
    
public:
    Dlink(): header{new Node(static_cast<T>(NULL))}, trailer{new Node(static_cast<T>(NULL))}, n{0} {
        header->next=trailer;
        trailer->prev=header;
    }
    Dlink(const Dlink<T>& dl);
    Dlink<T>& operator=(const Dlink<T>& dl);
    ~Dlink(){
        destoryList();
    }
    
    bool isEmpty(){return header->next=trailer;}
    const int getSize() const{return n;}
    void addFront(const T& t);
    void addBack(const T& t);
    T removeFront();
    T removeBack();
    void remove(const T& t);
    bool find(const T& t) const;
    
    friend std::ostream& operator<<(std::ostream& out, const Dlink<T>& dl){
        return dl.printList(out);
    }
};

template<typename T>
Dlink<T>& Dlink<T>::operator=(const Dlink<T> &dl){
    if(this!=&dl){
        Dlink<T> c{dl};
        swap(c);
    }
    return *this;
}

template<typename T>
Dlink<T>::Dlink(const Dlink<T>& dl): Dlink(){
    for(Node* node=dl.header->next; node!=dl.trailer; addBack(node->info), node=node->next);
}

template<typename T>
bool Dlink<T>::find(const T& t) const{
    Node* node=findNode(t);
    if(node) return true;
    return false;
}

template<typename T>
void Dlink<T>::remove(const T& t){
    Node* node=findNode(t);
    if(node){
        if(node->next==trailer && node->prev==header){
            removeFromListWithSingleEntry();
        }else{
            
            node->prev->next=node->next;
            node->next->prev=node->prev;
        }
        n--;
    }
}

template<typename T>
void Dlink<T>::addFront(const T& t){
    Node* node=new Node(t);
    if(!addInEmptyList(node)){
        node->next=header->next;
        node->prev=header;
        header->next->prev=node;
        header->next=node;
    }
    n++;
}

template<typename T>
void Dlink<T>::addBack(const T& t){
    Node* node=new Node(t);
    if(!addInEmptyList(node)){
        node->next=trailer;
        node->prev=trailer->prev;
        trailer->prev->next=node;
        trailer->prev=node;
    }
    n++;
}

template<typename T>
T Dlink<T>::removeFront(){
    T res=removeFromListWithSingleEntry();
    if(!res){
        Node* node=header->next;
        res=node->info;
        header->next=node->next;
        node->next->prev=header;
        delete node;
        node=nullptr;
         n--;
    }
    return res;
}

template<typename T>
T Dlink<T>::removeBack(){
    T res=removeFromListWithSingleEntry();
    if(!res){
        Node* node=trailer->prev;
        res=node->info;
        trailer->prev=node->prev;
        node->prev->next=trailer;
        n--;
        delete node;
        node=nullptr;
    }
    return res;
}

template<typename T>
std::ostream& Dlink<T>::printList(std::ostream &out) const{
    const size_t perLine{20};
    size_t num{0};
    for(Node* node=header->next; node!=trailer; node=node->next){
        out<<node->info<<std::setw(6);
        if(!(++num % perLine)) out<<"\n";
    }
    out<<"\n";
    return out;
}

template<typename T>
void Dlink<T>::destoryList(){
    while (!isEmpty()) {
        removeFront();
    }
    delete header;
    delete trailer;
    header=nullptr;
    trailer=nullptr;
}

#endif /* Dlink_h */
