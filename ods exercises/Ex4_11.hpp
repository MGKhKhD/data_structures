//
//  Ex4_11.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-20.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex4_11_h
#define Ex4_11_h

#include <algorithm>
#include <iomanip>
#include <iostream>

template<typename T>
class Ex4_11{
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
    
    Node* newNode(const T& t, const int hv){
        Node* node=new Node(t, hv);
        return node;
    }
    
    int setHeight() {
        int z=std::rand();
        int k=0;
        int m=1;
        while ((m & z)!=0) {
            k++;
            m<<=1;
        }
        return k;
    }
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->next;
            for(int i=0; i<=node->height;++i)
                node->next[i]=nullptr;
            delete [] node->length;
            delete node;
            node=nullptr;
        }
    }
    
    Node* getNode(const int i) const{
        Node* curr=root;
        Node* prev=nullptr;
        int r=h;
        int j=-1;
        while (r>=0) {
            while(curr->next[r] && (!prev || (prev && prev!=curr)) && j+curr->length[r]<i){
                prev=curr;
                j+=curr->length[r];
                curr=curr->next[r];
            }
            r--;
        }
        return curr->next[0];
    }
    
    void destroyList();
    std::ostream& printList(std::ostream& out) const;
    
public:
    Ex4_11(): root{newNode(static_cast<T>(NULL), sizeof(int)*8)}, h{0}, n{0} {}
    Ex4_11(const Ex4_11<T>& sl);
    ~Ex4_11(){
        if(root) destroyList();
    }
    
    void add(const T& t, const int i);
    T remove(const int i);
    Ex4_11<T> truncate(const int i);
    friend std::ostream& operator<<(std::ostream& out, const Ex4_11<T>& sl){
        return sl.printList(out);
    }
};

template<typename T>
Ex4_11<T>::Ex4_11(const Ex4_11<T>& ss): Ex4_11(){
    int i=0;
    for(Node* node=ss.root->next[0]; node; add(node->info,i++), node=node->next[0]);
}

template<typename T>
Ex4_11<T> Ex4_11<T>::truncate(const int i){
    assert(i>0 && i<n);
    
    Ex4_11<T> sl;
    Node* node=getNode(i);
    int j=0;
    while (node) {
        sl.add(node->info, j++);
        remove(i);
        node=getNode(i);
    }
    return sl;
}

template<typename T>
T Ex4_11<T>::remove(const int i){
    assert(i>=0 && i<n);
    
    Node* curr=root;
    Node* del=nullptr;
    Node* prev=nullptr;
    int r=h;
    int j=-1;
    while (r>=0) {
        while (curr->next[r] && (!prev || (prev && prev!=curr)) && j+curr->length[r]<i) {
            prev=curr;
            j+=curr->length[r];
            curr=curr->next[r];
        }
        curr->length[r]--;
        if(j+1+curr->length[r]==i && curr->next[r]){
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
void Ex4_11<T>::add(const T &t, const int i){
    assert(i>=0 && i<=n);
    
    Node* curr=root;
    Node* node=newNode(t, setHeight());
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
void Ex4_11<T>::destroyList(){
    Node* curr=root->next[0];
    while (curr) {
        Node* tmp=curr->next[0];
        deleteNode(curr);
        curr=tmp;
        n--;
    }
    deleteNode(root);
    h=0;
}

template<typename T>
std::ostream& Ex4_11<T>::printList(std::ostream &out) const{
    const size_t perLine{20};
    size_t num=0;
    for(Node* node=root->next[0]; node; node=node->next[0]){
        out<<node->info<<std::setw(5);
        if(!(++num % perLine)) out<<"\n";
    }
    out<<"\n";
    return out;
}


#endif /* Ex4_11_h */
