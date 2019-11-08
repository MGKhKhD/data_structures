//
//  Ex7_9.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-15.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex7_9_h
#define Ex7_9_h

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Queue.hpp"
#include "Stack.hpp"

template<typename T>
class Ex7_9{
    struct Node{
        T info;
        Node** list_p;
        T* range;
        int p;
        
        explicit Node(const T& t): info{t}, range{new T[2]}, p{std::rand()}, list_p{new Node*[3]} {
            for(int i=0; i<3; ++i)
                list_p[i]=nullptr;
            for(int i=0; i<2; ++i)
                range[i]=t;
        }
    };
    Node* root;
    Stack<Node*> finger;
    
    void deleteNode(Node* node){
        delete [] node->list_p;
        delete [] node->range;
        delete node;
        node=nullptr;
    }
    
    
    
    Node* findPrevNode(const T& t, Node* start);
    void deleteTree(Node* node);
    void searchFinger(const T& t);
    bool addChild(Node* prev, Node* node);
    void inorder(Node* node) const;
    T getRange(Node* node, const int r) const;
    
    void rotate(Node* node, int r);//r=0 rotate right; r=1 rotate left
    void bubbleUp(Node* node);
    void updateParams(Node* node);
    bool setRange(Node* node);
    void splice(Node* node);
    void trickleDown(Node* node);
    
public:
    Ex7_9(): root{nullptr} {}
    ~Ex7_9(){
        deleteTree(root);
    }
    
    void add(const T& t);
    void remove(const T& t);
    bool search(const T& t);
    
    void inorderTraversal() const{
        inorder(root);
        std::cout<<"\n";
    }
    void breathFirstPrint() const;
};

template<typename T>
bool Ex7_9<T>::search(const T &t){
    Node* prev=findPrevNode(t, root);
    if(prev && prev->info==t)
        return true;
    return false;
}

template<typename T>
void Ex7_9<T>::splice(Node *node){
    Node* succ=node->list_p[0]? node->list_p[0]: node->list_p[1];
    Node* par=nullptr;
    if(node==root){
        root=succ;
    }else{
        par=node->list_p[2];
        par->list_p[node==par->list_p[0]? 0:1]=succ;
    }
    if(succ)
        succ->list_p[2]=par;
}

template<typename T>
void Ex7_9<T>::trickleDown(Node *node){
    while(node->list_p[0] || node->list_p[1]){
        if(!node->list_p[0]){
            rotate(node, 1);
        }else if(!node->list_p[1]){
            rotate(node, 0);
        }else{
            rotate(node, node->list_p[0]->p < node->list_p[1]->p? 0: 1);
        }
        if(node==root)
            root=node->list_p[2];
    }
}

template<typename T>
void Ex7_9<T>::remove(const T &t){
    Node* prev=findPrevNode(t, root);
    if(prev && prev->info==t){
        trickleDown(prev);
        
        Node* node=prev->list_p[2];
        splice(prev);
        deleteNode(prev);
        if(node){
            setRange(node);
        }
        Node* par=node->list_p[2];
        if(par)
            updateParams(par);
    }
}

template<typename T>
void Ex7_9<T>::add(const T &t){
    Node* prev=findPrevNode(t, root);
    Node* node=new Node(t);
    if(addChild(prev, node)){
        if(prev) updateParams(prev);
        bubbleUp(node);
        return;
    }
    deleteNode(node);
}

template<typename T>
bool Ex7_9<T>::setRange(Node *node){
    T up=node->range[1];
    T low=node->range[0];
    node->range[0]=node->list_p[0]? std::min(node->list_p[0]->range[0], low):node->info;
    node->range[1]=node->list_p[1]? std::max(node->list_p[1]->range[1], up):node->info;
    if(node->range[0]==low && node->range[1]==up) return true;
    return false;
}


template<typename T>
void Ex7_9<T>::updateParams(Node* node){
    bool stop{false};
    while (node && !stop) {
        stop=setRange(node);
        node=node->list_p[2];
    }
}

template<typename T>
void Ex7_9<T>::bubbleUp(Node *node){
    while(node->list_p[2] && node->list_p[2]->p > node->p){
        rotate(node->list_p[2], node->list_p[2]->list_p[0]==node? 0: 1);
    }
    if(!node->list_p[2])
        root=node;
}

template<typename T>
void Ex7_9<T>::rotate(Node *node, int r){
    Node* w=node->list_p[r];
    w->list_p[2]=node->list_p[2];
    if(w->list_p[2])
        w->list_p[2]->list_p[node->list_p[2]->list_p[0]==node? 0 : 1]=w;
    
    int r_hat=(r+1)%2;
    node->list_p[r]=w->list_p[r_hat];
    if(node->list_p[r])
    {
        node->list_p[r]->list_p[2]=node;
    }
    node->list_p[2]=w;
    w->list_p[r_hat]=node;
    setRange(node);
    updateParams(w);
    
    if(node==root){
        root=w;
        w->list_p[2]=nullptr;
    }
}

template<typename T>
void Ex7_9<T>::breathFirstPrint() const{
    Node* node=root;
    Queue<Node*> q;
    q.enqueue(node);
    int s=q.getSize();
    while(!q.isEmpty()){
        if(s==0)
            std::cout<<"\n";
        s=q.getSize();
        while (s>0) {
            node=q.dequeue();
            std::cout<<"("<<node->range[0]-getRange(node, 0)<<","<<node->info<<","<<node->range[1]-getRange(node, 1)<<")"<<std::setw(6);
            if(node->list_p[0]) q.enqueue(node->list_p[0]);
            if(node->list_p[1]) q.enqueue(node->list_p[1]);
            s--;
        }
    }
    std::cout<<"\n";
}

template<typename T>
T Ex7_9<T>::getRange(Node* node, const int r) const{
    T ret=node->range[r];
    while(node->list_p[r]){
        node=node->list_p[r];
        ret=node->range[r];
    }
    return ret;
}

template<typename T>
bool Ex7_9<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info==node->info)
        return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info > node->info ? 0: 1]=node;
        node->list_p[2]=prev;
    }
    return true;
}

template<typename T>
void Ex7_9<T>::inorder(Node* node) const{
    if(node){
        int pri=node->list_p[2]? node->list_p[2]->p: 0;
        char c=node->p-pri>0? '+': '-';
        inorder(node->list_p[0]);
        std::cout<<"("<<node->info<<","<<c<<")"<<std::setw(6);
        inorder(node->list_p[1]);
    }
}

template<typename T>
void Ex7_9<T>::searchFinger(const T& t){
    bool found{false};
    while(!finger.isEmpty() && !found){
        Node* node=finger.top();
        if(node->range[0] <t && node->range[1]>t)
        {
            found=true;
        }else{
            finger.pop();
        }
    }
}

template<typename T>
void Ex7_9<T>::deleteTree(Node* node){
    if(node){
        deleteNode(node->list_p[0]);
        deleteNode(node->list_p[1]);
        deleteNode(node);
    }
}

template<typename T>
typename Ex7_9<T>::Node* Ex7_9<T>::findPrevNode(const T &t, Node *start){
    Node* curr=nullptr;
    searchFinger(t);
    if(finger.isEmpty()){
        curr=start;
    }else{
        curr=finger.top();
        finger.pop();
    }
    
    Node* prev=nullptr;
    while (curr) {
        finger.push(curr);
        if(curr->info==t){
            return curr;
        }else{
            prev=curr;
            curr=curr->list_p[curr->info > t? 0:1];
        }
    }
    return prev;
}

#endif /* Ex7_9_h */
