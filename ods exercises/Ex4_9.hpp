//
//  Ex4_9.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-11.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex4_9_h
#define Ex4_9_h

#include <stdexcept>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "Stack.hpp"

template<typename T>
class Ex4_9{
    struct Node{
        T info;
        int height;
        Node** next;
        int rank;
    };
    Node* root;
    int h;
    int n;
    Node** list;
    
    Node* newNode(const T& t, const int hv){
        Node* node=new Node;
        node->info=t;
        node->height=hv;
        node->rank=-1;
        node->next=new Node*[hv+1];
        return node;
    }
    
    int setHeight(){
        int z=std::rand();
        int m=1;
        int k=0;
        while( (m & z)!=0){
            k++;
            m<<=1;
        }
        return k;
    }
    
    void deleteNode(Node* node){
        delete [] node->next;
        delete node;
        node=nullptr;
    }
    
    void destroySet(){
        Node* curr=root->next[0];
        while(curr){
            Node* tmp=curr->next[0];
            deleteNode(curr);
            curr=tmp;
            n--;
        }
        delete [] root->next;
        
        delete [] list;
        
        memset(root->next, '\0', sizeof(Node*)*(root->height+1));
        delete root;
        root=nullptr;
        h=0;
    }
    
public:
    Ex4_9();
    Ex4_9(const Ex4_9<T>& ss);
    Ex4_9<T>& operator=(const Ex4_9<T>& ss);
    Ex4_9(Ex4_9<T>&& ss);
//    Ex4_9<T>& operator=(Ex4_9<T>&& ss);
    ~Ex4_9(){
        if(root){
            destroySet();
        }
    }
    
    void add(const T& t);
    T get(const int i) const;
    bool find(const T& t) const;
    bool removeItem(const T& t);
    T removeIndex(const int i);
    T set(const int i, const T& t);
    
    void printList() const;
    void swap(Ex4_9<T>& other) noexcept;
};

//template<typename T>
//Ex4_9<T>& Ex4_9<T>::operator=(Ex4_9<T>&& ss){
//    Ex4_9<T> c{std::move(ss)};
//    swap(c);
//    return *this;
//}

template<typename T>
void Ex4_9<T>::swap(Ex4_9<T> &other) noexcept{
    std::swap(root, other.root);
    std::swap(n, other.n);
    std::swap(h, other.h);
}

template<typename T>
Ex4_9<T>::Ex4_9(Ex4_9<T>&& ss): root{ss.root}, n{ss.n}, h{ss.h}{
    ss.root=nullptr;
}

template<typename T>
Ex4_9<T>& Ex4_9<T>::operator=(const Ex4_9<T> &ss){
    if(this !=&ss){
        Ex4_9<T> c{ss};
        swap(c);
    }
    return *this;
}

template<typename T>
Ex4_9<T>::Ex4_9(const Ex4_9<T>& s): Ex4_9(){
    for(Node* node=s.root->next[0]; node; add(node->info), node=node->next[0]);
}

template<typename T>
Ex4_9<T>::Ex4_9(): root{newNode(static_cast<T>(NULL), sizeof(int)*8)}, h{0}, n{0} {
    memset(root->next, '\0', sizeof(Node*)*(root->height+1));
    list=new Node*[root->height+1];
}

template<typename T>
T Ex4_9<T>::removeIndex(const int i){
    assert(i<n && i>=0);
    
    Node* curr=root;
    Node* del=nullptr;
    Node* prev=nullptr;
    bool found{false};
    int r=h;
    while(r>=0){
        while(curr->next[r] && (!prev || (prev && prev!=curr)) && curr->next[r]->rank<i){
            prev=curr;
            curr=curr->next[r];
        }
        if(curr->next[r] && curr->next[r]->rank==i){
            found=true;
            del=curr->next[r];
            curr->next[r]=del->next[r];
            if(curr==root && !curr->next[r])
                h--;
        }
        r--;
    }
    T ret;
    if(found){
        ret=del->info;
        for(Node* node=del->next[0]; node; node->rank--, node=node->next[0]);
        deleteNode(del);
        n--;
        return ret;
    }
    return static_cast<T>(NULL);
}

template<typename T>
bool Ex4_9<T>::removeItem(const T &t){
    Node* curr=root;
    Node* del=nullptr;
    Node* prev=nullptr;
    int r=h;
    bool found{false};
    while(r>=0){
        while(curr->next[r] && (!prev || (prev && prev!=curr)) && curr->next[r]->info<t){
            prev=curr;
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
        for(Node* node=del->next[0]; node; node->rank--, node=node->next[0]);
        deleteNode(del);
        n--;
    }
    return found;
}

template<typename T>
bool Ex4_9<T>::find(const T &t) const{
    Node* curr=root;
    Node* prev=nullptr;
    int r=h;
    while(r>=0){
        while (curr->next[r] && (!prev || (prev && prev!=curr)) && curr->next[r]->info<t) {
            prev=curr;
            curr=curr->next[r];
        }
        r--;
    }
    if(curr->next[0] && curr->next[0]->info==t)
        return true;
    return false;
}

template<typename T>
T Ex4_9<T>::set(const int i, const T &t){
    assert(i<n && i>=0);
    
    Node* curr=root;
    Node* prev=nullptr;
    int r=h;
    while(r>=0){
        while(curr->next[r] && (!prev || (prev && prev!=curr)) && curr->next[r]->rank<i){
            prev=curr;
            curr=curr->next[r];
        }
        r--;
    }
    T ret;
    if(curr->next[0] && curr->next[0]->rank==i){
        ret=curr->next[0]->info;
        if(curr!=root && curr->info<t && curr->next[0]->next[0] && curr->next[0]->next[0]->info>t){
            curr->next[0]->info=t;
        }else{
            removeIndex(curr->next[0]->rank);
            add(t);
        }
        return ret;
    }
    return static_cast<T>(NULL);
}

template<typename T>
T Ex4_9<T>::get(const int i) const{
    assert(i>=0 && i<n);
    
    Node* node=root;
    int r=h;
    while(r>=0){
        while(node->next[r] && node->next[r]->rank<i)
            node=node->next[r];
        r--;
    }
    return node->next[0]->info;
}

template<typename T>
void Ex4_9<T>::add(const T &t){
    Node* curr=root;
    Node* prev=nullptr;
    int r=h;
    while(r>=0){
        while(curr->next[r] && (!prev || (prev && prev!=curr)) && curr->next[r]->info<t){
            prev=curr;
            curr=curr->next[r];
        }
        if(curr->next[r] && curr->next[r]->info==t)
            return;
        list[r--]=curr;
    }
    
    Node* node=newNode(t, setHeight());
    node->rank=curr->rank+1;
    
    while(h<=node->height)
        list[++h]=root;
    for(int i=0; i<=node->height; ++i){
        node->next[i]=list[i]->next[i];
        list[i]->next[i]=node;
    }
    n++;
    
    for(Node* tmp=node->next[0]; tmp; tmp->rank++, tmp=tmp->next[0]);
}

template<typename T>
void Ex4_9<T>::printList() const{
    const size_t perLine{20};
    size_t num=0;
    for(Node* node=root->next[0]; node; node=node->next[0]){
        std::cout<<node->info<<std::setw(5);
        if(!(++num % perLine)) std::cout<<"\n";
    }
    std::cout<<"\n";
}

#endif /* Ex4_9_h */
