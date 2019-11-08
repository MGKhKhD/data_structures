//
//  SGfinger.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-23.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef SGfinger_h
#define SGfinger_h

#include "Stack.hpp"
#include "Queue.hpp"
#include <iostream>
#include <iomanip>

template<typename T>
class SGfinger{
    struct Node{
        T info;
        Node** list_p;
        T* range;
        int size;
        
        explicit Node(const T& t): info{t}, list_p{new Node*[3]}, range{new T[2]}, size{1} {
            for(int i=0; i<3; list_p[i]=nullptr, ++i);
            range[0]=t;
            range[1]=t;
        }
    };
    Node* root;
    mutable Stack<Node*> finger;
    int n;
    int q;
    
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
    Node* findPrevNode(const T& t) const;
    void searchFinger(const T& t) const;
    const int getDepth(Node* node) const;
    const int getSize(Node* node) const;
    bool addChild(Node* prev, Node* node);
    void updateSize(Node* node, bool decrease);
    void updateRange(Node* node);
    T getRange0(Node* node) const;
    T getRange1(Node* node) const;
    void rebuildSubTree(Node* node);
    int packToArray(Node* node, Node** arr, int i);
    Node* buildBalance(Node** arr, int i, int ns);
    bool setRange(Node* node);
    void splice(Node* node);
    
    static int log32(const int qv){
        static double log23=2.4663034623764317;
        return static_cast<int>(std::ceil(std::log(qv)*log23));
    }
    
public:
    SGfinger(): root{nullptr}, n{0}, q{0}{}
    ~SGfinger(){
        if(root) destroyTree();
        q=0;
    }
    
    void add(const T& t);
    bool find(const T& t) const;
    void remove(const T& t);
    void inorderTraversal() const;
    void bfs() const;
};

template<typename T>
void SGfinger<T>::remove(const T& t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        Node* par=nullptr;
        if(!prev->list_p[0] || !prev->list_p[1]){
            par=prev->list_p[2];
            splice(prev);
            deleteNode(prev);
        }else{
            Node* curr=prev->list_p[1];
            while(curr->list_p[0])
                curr=curr->list_p[0];
            prev->info=curr->info;
            updateRange(prev);
            par=curr->list_p[2];
            splice(curr);
            deleteNode(curr);
        }
        n--;
        if(par) {
            updateSize(par, true);
            updateRange(par);
        }
        if(2*n > q){
            rebuildSubTree(root);
            q=n;
        }
    }
}

template<typename T>
void SGfinger<T>::splice(Node* node){
    Node* succ=node->list_p[node->list_p[0]? 0:1];
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
bool SGfinger<T>::find(const T& t) const{
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t)
        return true;
    return false;
}

template<typename T>
bool SGfinger<T>::setRange(Node* node){
    T low=node->range[0];
    T up=node->range[1];
    node->range[0]=node->list_p[0]? std::min(node->list_p[0]->range[0], low): node->info;
    node->range[1]=node->list_p[1]? std::max(node->list_p[1]->range[1], up):node->info;
    if(node->range[0]==low && node->range[1]==up){
        return true;
    }
    return false;
}

template<typename T>
void SGfinger<T>::updateRange(Node* node){
    bool stop{false};
    while (node && !stop) {
        stop=setRange(node);
        node=node->list_p[2];
    }
}

template<typename T>
void SGfinger<T>::updateSize(Node* node,  bool decrease){
    if(decrease){
        while (node) {
            node->size--;
            node=node->list_p[2];
        }
    }else{
        while (node) {
            node->size++;
            node=node->list_p[2];
        }
    }
}

template<typename T>
bool SGfinger<T>::addChild(Node* prev, Node* node){
    if(prev && prev->info==node->info) return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info>node->info? 0:1]=node;
        node->list_p[2]=prev;
        updateSize(prev, false);
    }
    n++;
    q++;
    return true;
}

template<typename T>
const int SGfinger<T>::getDepth(Node* node) const{
    int d=0;
    while (node!=root) {
        d++;
        node=node->list_p[2];
    }
    return d;
}

template<typename T>
void SGfinger<T>::add(const T& t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        updateRange(prev);
        int depth=getDepth(node);
        if(depth>0 && depth >= log32(q)){
            Node* par=prev->list_p[2];
            while (3*prev->size <=2*par->size) {
                prev=par;
                par=par->list_p[2];
            }
            rebuildSubTree(par);
        }
        return;
    }
    deleteNode(node);
}

template<typename T>
void SGfinger<T>::rebuildSubTree(Node* node){
    if(node->size<2) return;
    Node** arr=new Node*[node->size];
    Node* par=node->list_p[2];
    packToArray(node, arr, 0);
    if(!par){
        root=buildBalance(arr, 0, node->size);
        root->list_p[2]=nullptr;
    }else{
        int r=node==par->list_p[0]? 0:1;
        par->list_p[r]=buildBalance(arr, 0, node->size);
        par->list_p[r]->list_p[2]=par;
    }
    
    delete [] arr;
}

template<typename T>
typename SGfinger<T>::Node* SGfinger<T>::buildBalance(Node **arr, int i, int ns){
    if(ns==0) return nullptr;
    int m=ns/2;
    arr[i+m]->size=1;
    for(int r=0; r<2; ++r){
        arr[i+m]->list_p[r]=buildBalance(arr, r==0? i: i+m+1, r==0? m: ns-m-1);
        if(arr[i+m]->list_p[r]){
            arr[i+m]->list_p[r]->list_p[2]=arr[i+m];
            arr[i+m]->size+=arr[i+m]->list_p[r]->size;
        }
    }
    arr[i+m]->range[0]=arr[i+m]->range[1]=arr[i+m]->info;
    setRange(arr[i+m]);
    return arr[i+m];
}

template<typename T>
int SGfinger<T>::packToArray(Node* node, Node **arr, int i){
    if(!node) return i;
    i=packToArray(node->list_p[0], arr, i);
    arr[i++]=node;
    return packToArray(node->list_p[1], arr, i);
}

template<typename T>
void SGfinger<T>::searchFinger(const T& t) const {
    if(finger.isEmpty()) return;
    while (!finger.isEmpty()) {
        Node* node=finger.top();
        if(node->range[0]<=t && node->range[1]>=t){
            return;
        }
        finger.pop();
    }
}

template<typename T>
typename SGfinger<T>::Node* SGfinger<T>::findPrevNode(const T& t) const{
    Node* node=root;
    searchFinger(t);
    if(!finger.isEmpty()) {
        node=finger.top();
        finger.pop();
    }
    Node* prev=nullptr;
    while (node) {
        finger.push(node);
        if(node->info==t){
            return node;
        }else{
            prev=node;
            node=node->list_p[prev->info>t? 0:1];
        }
    }
    return prev;
}

template<typename T>
void SGfinger<T>::destroyTree(){
    Node* node=root;
    Stack<Node*> stack;
    while (node || !stack.isEmpty()) {
        if(node){
            stack.push(node);
            node=node->list_p[0];
        }else{
            node=stack.top();
            stack.pop();
            Node* tmp=node->list_p[1];
            deleteNode(node);
            node=tmp;
            n--;
        }
    }
}

template<typename T>
void SGfinger<T>::inorderTraversal() const{
    Node* node=root;
    Stack<Node*> s;
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[0];
        }else{
            node=s.top();
            s.pop();
            std::cout<<"("<<node->info<<","<<node->size-getSize(node)<<","<<node->range[0]-getRange0(node)<<","<<node->range[1]- getRange1(node)<<")"<<std::setw(6);
            node=node->list_p[1];
        }
    }
    std::cout<<"\n";
}

template<typename T>
const int SGfinger<T>::getSize(Node* node) const{
    Stack<Node*> stack;
    int s=0;
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

template<typename T>
T SGfinger<T>::getRange0(Node* node) const{
    T range0=node->range[0];
    while (node) {
        range0=node->range[0];
        node=node->list_p[0];
    }
    return range0;
}

template<typename T>
T SGfinger<T>::getRange1(Node* node) const{
    T range1=node->range[1];
    while (node) {
        range1=node->range[1];
        node=node->list_p[1];
    }
    return range1;
}

template<typename T>
void SGfinger<T>::bfs() const{
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

#endif /* SGfinger_h */
