//
//  SkipList.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-15.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef SkipList_h
#define SkipList_h

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>

template<typename T>
class SkipList{
    struct Node{
        T info;
        Node** next;
        int* length;
        int height;
        
        Node(const T& t, const int hv): info{t}, next{new Node*[hv+1]}, length{new int[hv+1]}, height{hv} {
            for(int i=0; i<=hv; ++i){
                length[i]=-1;
                next[i]=nullptr;
            }
        }
    };
    Node* root;
    int h;
    int n;
    
    int setHeight(){
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
        delete [] node->length;
        for(int i=0; i<=node->height; ++i)
            node->next[i]=nullptr;
        delete [] node->next;
        delete node;
        node=nullptr;
    }
    
    void destoryList(){
        Node* node=root->next[0];
        while (node) {
            Node* tmp=node->next[0];
            deleteNode(node);
            node=tmp;
            n--;
        }
        deleteNode(root);
        h=0;
    }
    
    Node* findNode(const int i) const{
        Node* curr=root;
        Node* prev=nullptr;
        int r=h;
        int j=-1;
        while (r>=0) {
            while (curr->next[r] && (!prev || (prev && prev!=curr)) && j+curr->length[r]<i) {
                prev=curr;
                j+=curr->length[r];
                curr=curr->next[r];
            }
            r--;
        }
        return curr->next[0];
    }
    
    void swap(SkipList<T>& other) noexcept{
        std::swap(root, other.root);
        std::swap(n, other.n);
        std::swap(h, other.h);
    }
    
public:
    SkipList(): root{new Node(static_cast<T>(NULL), sizeof(int)*8)}, h{0}, n{0}{}
    SkipList(const SkipList<T>& ss);
    SkipList<T>& operator=(const SkipList<T>& ss);
    ~SkipList(){
        if(root)
            destoryList();
    }
    
    void add(const T& t, const int i);
    T get(const int i) const;
    T set(const int i, const T& t);
    T remove(const int i);
    
    friend std::ostream& operator<<(std::ostream& out, const SkipList<T>& sl){
        const size_t PerLine{20};
        size_t num{0};
        for(Node* node=sl.root->next[0]; node; node=node->next[0]){
            out<<node->info<<std::setw(6);
            if(!(++num % PerLine)) out<<"\n";
        }
        out<<"\n";
        return out;
    }
    
};

template<typename T>
SkipList<T>& SkipList<T>::operator=(const SkipList<T> &ss){
    if(this!=&ss){
        SkipList<T> c{ss};
        swap(c);
    }
    return *this;
}

template<typename T>
SkipList<T>::SkipList(const SkipList<T>& ss): SkipList(){
    int i=0;
    for(Node* node=ss.root->next[0];node; add(node->info, i++), node=node->next[0]);
}

template<typename T>
T SkipList<T>::remove(const int i){
    assert(i<n && i>=0);
    Node* curr=root;
    Node* prev=nullptr;
    Node* del=nullptr;
    bool found{false};
    int r=h;
    int j=-1;
    while (r>=0) {
        while (curr->next[r] && (!prev || (prev && prev!=curr)) && j+curr->length[r]<i) {
            prev=curr;
            j+=curr->length[r];
            curr=curr->next[r];
        }
        curr->length[r]--;
        if(j+curr->length[r]+1==i && curr->next[r]){
            found=true;
            del=curr->next[r];
            curr->next[r]=del->next[r];
            curr->length[r]+=del->length[r];
            if(curr==root && curr->next[r]==nullptr)
                h--;
        }
        r--;
    }
    T res=static_cast<T>(NULL);
    if(found){
        res=del->info;
        deleteNode(del);
        n--;
    }
    return res;
}

template<typename T>
T SkipList<T>::set(const int i, const T &t){
    assert(i<n && i>=0);
    Node* node=findNode(i);
    T ret=node->info;
    node->info=t;
    return ret;
}

template<typename T>
T SkipList<T>::get(const int i) const{
    assert(i<n && i>=0);
    return findNode(i)->info;
}

template<typename T>
void SkipList<T>::add(const T &t, const int i){
    assert(i<=n && i>=0);
    
    Node* curr=root;
    Node* prev=nullptr;
    Node* node=new Node(t, setHeight());
    if(node->height>h)
        h=node->height;
    int r=h;
    int j=-1;
    while (r>=0) {
        while (curr->next[r] && (!prev || (prev && prev!=curr)) && j+curr->length[r]<i) {
            prev=curr;
            j+=curr->length[r];
            curr=curr->next[r];
        }
        curr->length[r]++;
        if(node->height>=r){
            node->next[r]=curr->next[r];
            node->length[r]=curr->length[r]-(i-j);
            curr->length[r]=i-j;
            curr->next[r]=node;
        }
        r--;
    }
    n++;
}

#endif /* SkipList_h */
