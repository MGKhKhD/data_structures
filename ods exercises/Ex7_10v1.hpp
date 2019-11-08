//
//  Ex7_10v1.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-22.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex7_10v1_h
#define Ex7_10v1_h

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Stack.hpp"

template<typename T>
class Ex7_10v1{
    struct Node{
        T info;
        int p;
        Node** list_p;
        
        explicit Node(const T& t): info{t}, p{std::rand()}, list_p{new Node*[3]} {
            for(int i=0; i<3; ++i)
                list_p[i]=nullptr;
        }
    };
    Node* root;
    int n;
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->list_p;
//            for(int i=0; i<3; ++i)
//                node->list_p[i]=nullptr;
            delete node;
            node=nullptr;
        }
    }
    
    void deleteTree(Node* node){
        if(node){
            deleteTree(node->list_p[0]);
            deleteTree(node->list_p[1]);
            deleteNode(node);
            n--;
        }
    }
    
    Node* findPrevNode(const T& t) const;
    bool addChild(Node* prev, Node* node);
    void bubbleUp(Node* node);
    void rotate(Node* node, int direction);//r=0: rotate to right
    void trickeDown(Node* node);
    void splice(Node* node);
    
public:
    Ex7_10v1(): root{nullptr}, n{0} {}
    ~Ex7_10v1(){
        if(root) deleteTree(root);
    }
    
    const int getLen() const{return n;}
    void add(const T& t);
    const T getLE(const int i) const;
    void remove(const T& t);
    const T removeLE(const int i);
    void inorderTraversal() const;
};

template<typename T>
const T Ex7_10v1<T>::removeLE(const int i){
    assert(i>=0 && i<n);
    int r=0;
    int lim=i;
    if(i>n/2){
        lim=n-i-1;
        r=1;
    }
    
    Node* node=root;
    int ind=0;
    Stack<Node*> s;
    T res=static_cast<T>(NULL);
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[r];
        }else{
            node=s.top();
            s.pop();
            Node* tmp=node;
            node=node->list_p[(r+1)%2];
            if(ind++==lim){
                res=tmp->info;
                trickeDown(tmp);
                splice(tmp);
                deleteNode(tmp);
                n--;
                return res;
            }
        }
    }
    return res;
}

template<typename T>
void Ex7_10v1<T>::trickeDown(Node* node){
    while (node->list_p[0] || node->list_p[1]) {
        if(!node->list_p[0]){
            rotate(node, 1);
        }else if(!node->list_p[1]){
            rotate(node, 0);
        }else{
            rotate(node, node->list_p[0]->p < node->list_p[1]->p? 0:1);
        }
        if(node==root){
            root=node->list_p[2];
        }
    }
}

template<typename T>
void Ex7_10v1<T>::splice(Node *node){
    Node* succ=node->list_p[node->list_p[0]? 0:1];
    Node* par=nullptr;
    if(node==root){
        root=succ;
    }else{
        par=node->list_p[2];
        par->list_p[par->list_p[0]==node? 0:1]=succ;
    }
    if(succ)
        succ->list_p[2]=par;
}

template<typename T>
void Ex7_10v1<T>::remove(const T& t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        trickeDown(prev);
        splice(prev);
        deleteNode(prev);
        n--;
    }
}

template<typename T>
const T Ex7_10v1<T>::getLE(const int i) const{
    assert(i>=0 && i<n);
    int r=0;
    int lim=i;
    if(i>n/2) {
        r=1;
        lim=n-i-1;
    }
    
    Stack<Node*> s;
    Node* node=root;
    int ind=0;
    T res=static_cast<T>(NULL);
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[r];
        }else{
            node=s.top();
            s.pop();
            if(ind++==lim){
                res=node->info;
                return res;
            }
            node=node->list_p[(r+1)%2];
        }
    }
    return res;
}

template<typename T>
void Ex7_10v1<T>::add(const T &t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        n++;
        bubbleUp(node);
        return;
    }
    deleteNode(node);
}

template<typename T>
void Ex7_10v1<T>::rotate(Node *node, int r){
    Node* w=node->list_p[r];
    w->list_p[2]=node->list_p[2];
    if(w->list_p[2])
        w->list_p[2]->list_p[node->list_p[2]->list_p[0]==node? 0:1]=w;
    int r_hat=(r+1)%2;
    node->list_p[r]=w->list_p[r_hat];
    if(node->list_p[r])
        node->list_p[r]->list_p[2]=node;
    node->list_p[2]=w;
    w->list_p[r_hat]=node;
    if(node==root)
    {
        root=w;
        w->list_p[2]=nullptr;
    }
        
}

template<typename T>
void Ex7_10v1<T>::bubbleUp(Node *node){
    while (node->list_p[2] && node->list_p[2]->p > node->p) {
        rotate(node->list_p[2], node->list_p[2]->list_p[0]==node? 0:1);
    }
    if(!node->list_p[2])
        root=node;
}

template<typename T>
bool Ex7_10v1<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info==node->info)
        return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info>node->info? 0:1]=node;
        node->list_p[2]=prev;
    }
    return true;
}

template<typename T>
typename Ex7_10v1<T>::Node* Ex7_10v1<T>::findPrevNode(const T &t) const{
    Node* curr=root;
    Node* prev=nullptr;
    while (curr) {
        if(curr->info==t){
            return curr;
        }else{
            prev=curr;
            curr=curr->list_p[prev->info>t? 0:1];
        }
    }
    return prev;
}

template<typename T>
void Ex7_10v1<T>::inorderTraversal() const{
    Node* node=root;
    Stack<Node*> s;
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[0];
        }else{
            node=s.top();
            s.pop();
            char c='+';
            if(node->list_p[2])
                c=node->p-node->list_p[2]->p>0? '+': '-';
            std::cout<<"("<<node->info<<","<<c<<")"<<std::setw(6);
            node=node->list_p[1];
        }
    }
    std::cout<<"\n";
}

#endif /* Ex7_10v1_h */
