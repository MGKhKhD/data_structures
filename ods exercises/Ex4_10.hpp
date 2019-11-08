//
//  Ex4_10.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-16.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex4_10_h
#define Ex4_10_h

#include <iostream>
#include <iomanip>
#include <algorithm>

template<typename T>
class Ex4_10{
    struct Node{
        T info;
        int height;
        Node** next;
        
        Node(const T& t, const int hv): info{t}, height{hv}, next{new Node*[hv+1]}{
            for(int i=0; i<=hv; ++i)
                next[i]=nullptr;
        }
    };
    Node* root;
    int h;
    int n;
    Node** finger;
    
    int setHeight(){
        int z=std::rand();
        int k=0;
        int m=1;
        while ((m&z)!=0) {
            k++;
            m<<=1;
        }
        return k;
    }
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->next;
            delete node;
            node=nullptr;
        }
    }
    
    void destorySet(){
        Node* curr=root->next[0];
        while (curr) {
            Node* tmp=curr->next[0];
            deleteNode(curr);
            curr=tmp;
            n--;
        }
        delete [] root->next;
        memset(root->next, '\0', sizeof(Node*)*(root->height+1));
        delete [] finger;
        delete root;
        root=nullptr;
        h=0;
    }
    
    int searchFinger(const T& t) const{
        int r=h;
        while(r>=0 && (finger[r]==root || (finger[r]!=root && finger[r]->info<t)))
              r--;
        return r;
        
    }
    
public:
    Ex4_10(): root{new Node(static_cast<T>(NULL), sizeof(int)*8)}, h{0}, n{0} {
        memset(root->next, '\0', sizeof(Node*)*(root->height+1));
        finger=new Node*[root->height+1];
        for(int i=0; i<root->height+1; ++i)
            finger[i]=root;
    }
    
    void add(const T& t);
    
};

template<typename T>
void Ex4_10<T>::add(const T &t){
    Node* curr=finger[searchFinger(t)];
    Node* node=new Node(t, setHeight());
    for(int i=0; i<=node->height; ++i){
        
    }
}


#endif /* Ex4_10_h */
