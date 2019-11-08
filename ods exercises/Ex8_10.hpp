//
//  Ex8_10.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-24.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex8_10_h
#define Ex8_10_h

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Stack.hpp"
#include "Queue.hpp"

template<typename T>
class Ex8_10{
    struct Node{
        T info;
        Node** list_p;
        int size;
        T* range;
        
        explicit Node(const T& t): info{t}, list_p{new Node*[3]}, size{1}, range{new T[2]} {
            for(int i=0; i<3; list_p[i]=nullptr, ++i);
            range[0]=t;
            range[1]=t;
        }
    };
    Node* root;
    int n;
    mutable Stack<Node*> finger;
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->list_p;
//            for(int i=0; i<3; node->list_p[i]=nullptr, ++i);
            delete [] node->range;
            delete node;
            node=nullptr;
        }
    }
    
    void destroyTree();
    void searchFinger(const T& t) const;
    Node* findPrevNode(const T& t) const;
    bool addChild(Node* prev, Node* node);
    void updateSize(Node* node, bool decrease);
    const int getSize(Node* node) const;
    const int getRange(Node* node, const int r) const;
    void updateRange(Node* node);
    bool setRange(Node* node);
    void manageRebuild(Node* node);
    void rebuildSubTree(Node* node);
    int packToArray(Node* node, Node** arr, int i);
    Node* rebuild(Node** arr, int i, int ns);
    void splice(Node* node);
    
public:
    Ex8_10(): root{nullptr}, n{0} {}
    ~Ex8_10(){
        if(root) destroyTree();
    }
    
    void add(const T& t);
    void remove(const T& t);
    void inorderTraversal() const;
    void bfs() const;
};

template<typename T>
void Ex8_10<T>::splice(Node* node){
    Node* succ=node->list_p[node->list_p[0]? 0:1];
    Node* par=nullptr;
    if(node==root){
        root=succ;
    }else{
        par=node->list_p[2];
        par->list_p[node==par->list_p[0]? 0:1]=succ;
    }
    if(succ) succ->list_p[2]=par;
}

template<typename T>
void Ex8_10<T>::remove(const T& t){
    Node* node=findPrevNode(t);
    if(node && node->info==t){
        Node* prev=nullptr;
        if(!node->list_p[0] || !node->list_p[1]){
            prev=node->list_p[2];
            splice(node);
            deleteNode(node);
        }else{
            Node* curr=node->list_p[1];
            while (curr->list_p[0]) {
                curr=curr->list_p[0];
            }
            node->info=curr->info;
            updateRange(node);
            prev=curr->list_p[2];
            splice(curr);
            deleteNode(curr);
        }
        n--;
        if(prev) {
            updateSize(prev, true);
            updateRange(prev);
            manageRebuild(prev);
        }
    }
}

template<typename T>
void Ex8_10<T>::updateRange(Node* node){
    bool stop{false};
    while (node && !stop) {
        stop=setRange(node);
        node=node->list_p[2];
    }
}

template<typename T>
bool Ex8_10<T>::setRange(Node* node){
    T lowRange=node->range[0];
    T upRange=node->range[1];
    node->range[0]=node->list_p[0]? std::min(node->list_p[0]->range[0], lowRange): node->info;
    node->range[1]=node->list_p[1]? std::max(node->list_p[1]->range[1], upRange):node->info;
    if(node->range[0]==lowRange && node->range[1]==upRange) return true;
    return false;
}

template<typename T>
typename Ex8_10<T>::Node* Ex8_10<T>::rebuild(Node **arr, int i, int ns){
    if(ns==0) return nullptr;
    int m=ns/2;
    arr[i+m]->size=1;
    for(int r=0; r<2; ++r){
        arr[i+m]->list_p[r]=rebuild(arr, r==0? i: i+m+1, r==0? m: ns-m-1);
        if(arr[i+m]->list_p[r]){
            arr[i+m]->list_p[r]->list_p[2]=arr[i+m];
            arr[i+m]->size+=arr[i+m]->list_p[r]->size;
        }
        arr[i+m]->range[r]=getRange(arr[i+m], r);
    }
    return arr[i+m];
}

template<typename T>
int Ex8_10<T>::packToArray(Node *node, Node **arr, int i){
    if(!node) return i;
    i=packToArray(node->list_p[0], arr, i);
    arr[i++]=node;
    return packToArray(node->list_p[1], arr, i);
}

template<typename T>
void Ex8_10<T>::rebuildSubTree(Node* node){
    if(node->size<2) return;
    Node** arr=new Node*[node->size];
    Node* par=node->list_p[2];
    packToArray(node, arr, 0);
    if(!par){
        root=rebuild(arr, 0, node->size);
        root->list_p[2]=nullptr;
    }else{
        int i=par->list_p[0]==node? 0:1;
        par->list_p[i]=rebuild(arr, 0, node->size);
        par->list_p[i]->list_p[2]=par;
    }
    
    delete [] arr;
}

template<typename T>
void Ex8_10<T>::manageRebuild(Node* node){
    while (node) {
        double r=static_cast<double>(std::rand()) / RAND_MAX;
        if(r > static_cast<double>(1)/node->size){
            rebuildSubTree(node);
        }
        node=node->list_p[2];
    }
}

template<typename T>
void Ex8_10<T>::add(const T& t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        updateRange(prev);
        if(prev) manageRebuild(prev);
        return;
    }
    deleteNode(node);
}

template<typename T>
void Ex8_10<T>::updateSize(Node *node, bool decrease){
    if(!decrease){
        while (node) {
            node->size++;
            node=node->list_p[2];
        }
    }else{
        while (node) {
            node->size--;
            node=node->list_p[2];
        }
    }
}

template<typename T>
bool Ex8_10<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info == node->info) return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info>node->info? 0:1]=node;
        node->list_p[2]=prev;
        updateSize(prev, false);
    }
    n++;
    return true;
}

template<typename T>
typename Ex8_10<T>::Node* Ex8_10<T>::findPrevNode(const T& t) const{
    Node* curr=root;
    searchFinger(t);
    if(!finger.isEmpty()) {
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
            curr=curr->list_p[prev->info>t? 0:1];
        }
    }
    return prev;
}

template<typename T>
void Ex8_10<T>::searchFinger(const T &t) const{
    if(finger.isEmpty()) return;
    while (!finger.isEmpty()) {
        Node* node=finger.top();
        if(node->range[0]<=t && node->range[1]>=t)
            return;
        finger.pop();
    }
}

template<typename T>
void Ex8_10<T>::destroyTree(){
    Node* node=root;
    Stack<Node*> s;
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[0];
        }else{
            node=s.top();
            s.pop();
            Node* tmp=node->list_p[1];
            deleteNode(node);
            node=tmp;
            n--;
        }
    }
}

template<typename T>
void Ex8_10<T>::inorderTraversal() const{
    Node* node=root;
    Stack<Node*> stack;
    while (node || !stack.isEmpty()) {
        if(node){
            stack.push(node);
            node=node->list_p[0];
        }else{
            node=stack.top();
            stack.pop();
            std::cout<<"("<<node->info<<","<<node->size-getSize(node)<<","<<node->range[0]-getRange(node, 0)<<","<<node->range[1]-getRange(node, 1)<<")"<<std::setw(6);
            node=node->list_p[1];
        }
    }
    std::cout<<"\n";
}

template<typename T>
void Ex8_10<T>::bfs() const{
    Node* node=root;
    Queue<Node*> q;
    q.enqueue(node);
    int s=q.getSize();
    while (!q.isEmpty()) {
        if(s==0) std::cout<<"\n";
        s=q.getSize();
        while (s>0) {
            node=q.dequeue();
            std::cout<<node->info<<std::setw(6);
            if(node->list_p[0]) q.enqueue(node->list_p[0]);
            if(node->list_p[1]) q.enqueue(node->list_p[1]);
            s--;
        }
    }
    std::cout<<"\n";
}

template<typename T>
const int Ex8_10<T>::getRange(Node *node, const int r) const{
    T range=node->range[r];
    while (node) {
        range=node->range[r];
        node=node->list_p[r];
    }
    return range;
}

template<typename T>
const int Ex8_10<T>::getSize(Node* node) const{
    int s=0;
    Stack<Node*> stack;
    while (node || !stack.isEmpty()) {
        if(node){
            stack.push(node);
            node=node->list_p[0];
        }else{
            node=stack.top();
            stack.pop();
            s++;
            node=node->list_p[1];
        }
    }
    return s;
}

#endif /* Ex8_10_h */
