//
//  Ex4_12.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-20.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex4_12_h
#define Ex4_12_h

#include <iostream>
#include <iomanip>
#include <algorithm>

template<typename T>
class Ex4_12{
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
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->next;
            for(int i=0; i<=node->height; ++i)
                node->next[i]=nullptr;
            delete [] node->length;
            delete node;
            node=nullptr;
        }
    }
    
    int setHeight(){
        int z=std::rand();
        int k=0;
        int m=1;
        while ((m & z)!=0) {
            k++;
            m<<=1;
        }
        return k;
    }
    
    void destroyList();
    std::ostream& printList(std::ostream& out) const;
    
public:
    Ex4_12(): root{new Node(static_cast<T>(NULL), sizeof(int)*8)}, h{0}, n{0} {}
    Ex4_12(const Ex4_12<T>& sl);
    ~Ex4_12(){
        if(root) destroyList();
    }
    
    bool isEmpty() const{ return n==0;}
    void add(const T& t, const int i);
    T remove(const int i);
    void absorb(Ex4_12<T> sl);
    friend std::ostream& operator<<(std::ostream& out, const Ex4_12<T>& sl){
        return sl.printList(out);
    }
};

template<typename T>
void Ex4_12<T>::absorb(Ex4_12<T> sl){
    int i=n-1;
    while(!sl.isEmpty()){
        add(sl.remove(0), ++i);
    }
}

template<typename T>
Ex4_12<T>::Ex4_12(const Ex4_12<T>& sl): Ex4_12(){
    int i=0;
    for(Node* node=sl.root->next[0]; node; add(node->info, i++), node=node->next[0]);
}

template<typename T>
T Ex4_12<T>::remove(const int i){
    assert(i<n && i>=0);
    
    Node* curr=root;
    Node* prev=nullptr;
    Node* del=nullptr;
    int r=h;
    int j=-1;
    while (r>=0) {
        while (curr->next[r] && (!prev || (prev && prev!=curr)) && curr->length[r]+j<i) {
            prev=curr;
            j+=curr->length[r];
            curr=curr->next[r];
        }
        curr->length[r]--;
        if(curr->next[r] && j+curr->length[r]+1==i){
            del=curr->next[r];
            curr->next[r]=del->next[r];
            curr->length[r]+=del->length[r];
            if(curr==root && !curr->next[r])
                h--;
        }
        r--;
    }
    T res=static_cast<T>(NULL);
    if(del){
        res=del->info;
        deleteNode(del);
        n--;
    }
    return res;
}

template<typename T>
void Ex4_12<T>::add(const T &t, const int i){
    assert(i>=0 && i<=n);
    
    Node* curr=root;
    Node* node=new Node(t, setHeight());
    if(node->height>h)
        h=node->height;
    Node* prev=nullptr;
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

template<typename T>
void Ex4_12<T>::destroyList(){
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

template<typename T>
std::ostream& Ex4_12<T>::printList(std::ostream &out) const{
    const size_t perLine{20};
    size_t num{0};
    for(Node* node=root->next[0]; node; node=node->next[0]){
        out<<node->info<<std::setw(6);
        if(!(++num % perLine)) out<<"\n";
    }
    out<<"\n";
    return out;
}


#endif /* Ex4_12_h */
