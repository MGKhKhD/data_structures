//
//  SkipSet.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-11.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef SkipSet_h
#define SkipSet_h

#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "Stack.hpp"

template<typename T>
class SkipSet{
    struct Node{
        T info;
        Node** next;
        int height;
    };
    Node* root;
    int n;
    int h;
    
    Node* newNode(const T& t, const int hv){
        Node* node=new Node;
        node->info=t;
        node->height=hv;
        node->next=new Node*[hv+1];
        return node;
    }
    
    int setHeight() {
        int z=std::rand();
        int m=1;
        int k=0;
        while ((m & z)!=0) {
            k++;
            m<<=1;
        }
        return k;
    }
    
    void deleteNode(Node* node){
        delete [ ] node->next;
        delete node;
        node=nullptr;
    }
    
    void deleteSet(){
        Node* curr=root->next[0];
        while(curr){
            Node* tmp=curr->next[0];
            deleteNode(curr);
            curr=tmp;
            n--;
        }
        delete [] root->next;
        memset(root->next, '\0', sizeof(Node*)*(root->height+1));
        delete root;
        root=nullptr;
        h=0;
    }
    
public:
    SkipSet();
    SkipSet(const SkipSet<T>& ss);
    SkipSet<T>& operator=(const SkipSet<T>& ss);
    SkipSet(SkipSet<T>&& ss);
//    SkipSet<T>& operator=(SkipSet<T>&& ss);
    ~SkipSet(){
        if(root){
            deleteSet();
        }
    }
    
    void add(const T& t);
    bool find(const T& t) const;
    void remove(const T& t);
    
    void printList() const;
    void swap(SkipSet<T>& other) noexcept;
};

//template<typename T>
//SkipSet<T>& SkipSet<T>::operator=(SkipSet<T> &&ss){
//    SkipSet<T> c{std::move(ss)};
//    swap(c);
//    return *this;
//}

template<typename T>
SkipSet<T>::SkipSet(SkipSet<T>&& ss): root{ss.root}, n{ss.n}, h{ss.h}{
    ss.root=nullptr;
}

template<typename T>
void SkipSet<T>::swap(SkipSet<T> &other) noexcept{
    std::swap(root, other.root);
    std::swap(n, other.n);
    std::swap(h, other.h);
}

template<typename T>
SkipSet<T>& SkipSet<T>::operator=(const SkipSet<T> &ss){
    if(this!=&ss){
        SkipSet<T> c{ss};
        swap(c);
    }
    return *this;
}

template<typename T>
SkipSet<T>::SkipSet(const SkipSet<T>& ss): SkipSet(){
    for(Node* node=ss.root->next[0]; node; node=node->next[0])
        add(node->info);
}

template<typename T>
SkipSet<T>::SkipSet(): root{newNode(static_cast<T>(NULL), sizeof(int)*8)}, h{0}, n{0}{
    memset(root->next, '\0', sizeof(Node*)*(root->height+1));
}

template<typename T>
void SkipSet<T>::remove(const T &t){
    Node* curr=root;
    Node* del=nullptr;
    int r=h;
    bool found{false};
    while(r>=0){
        while(curr->next[r] && curr->next[r]->info<t){
            curr=curr->next[r];
        }
        if(curr->next[r] && curr->next[r]->info==t){
            found=true;
            del=curr->next[r];
            curr->next[r]=del->next[r];
            if(curr==root && !curr->next[r])
                h--;
        }
        r--;
    }
    if(found){
        deleteNode(del);
        n--;
    }
}

template<typename T>
bool SkipSet<T>::find(const T &t) const{
    Node* curr=root;
    int r=h;
    while(r>=0){
        while(curr->next[r] && curr->next[r]->info<t){
            curr=curr->next[r];
        }
        r--;
    }
    if(curr->next[0] && curr->next[0]->info==t)
        return true;
    return false;
}

template<typename T>
void SkipSet<T>::add(const T &t){
    Node** list=new Node*[root->height+1];
    
    Node* curr=root;
    int r=h;
    while(r>=0){
        while(curr->next[r] && curr->next[r]->info<t){
            curr=curr->next[r];
        }
        if(curr->next[r] && curr->next[r]->info==t)
            return;
        
        list[r--]=curr;
    }
    Node* node=newNode(t, setHeight());
    while(node->height>=h)
    {
        list[++h]=root;
    }
    for(int i=0; i<=node->height;++i){
        node->next[i]=list[i]->next[i];
        list[i]->next[i]=node;
    }
    n++;
    delete [] list;
}

template<typename T>
void SkipSet<T>::printList() const{
    const size_t perLine{20};
    size_t num=0;
    for(Node* node=root->next[0]; node; node=node->next[0]){
        std::cout<<node->info<<std::setw(5);
        if(!(++num % perLine)) std::cout<<"\n";
    }
    std::cout<<"\n";
}


#endif /* SkipSet_h */
