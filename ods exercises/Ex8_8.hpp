//
//  Ex8_8.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-23.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex8_8_h
#define Ex8_8_h

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Stack.hpp"
#include "Queue.hpp"

template<typename T>
class Ex8_8{
    struct Node{
        T info;
        Node** list_p;
        int size;
        
        explicit Node(const T& t): info{t}, list_p{new Node*[3]}, size{1} {
            for(int i=0; i<3; list_p[i]=nullptr, ++i);
        }
    };
    Node* root;
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->list_p;
//            for(int i=0; i<3; node->list_p[i]=nullptr, ++i);
            delete node;
            node=nullptr;
        }
    }
    
    bool addChild(Node* prev, Node* node);
    Node* findPrevNode(const T& t) const;
    void destroyTree(Node* node);
    void updateSize(Node* node, bool decrease);
    const int getSize(Node* node) const;
    void rebuild(Node* node);
    void buildSubTree(Node* node);
    int packToArray(Node* node, Node** arr, int i);
    Node* buildBalanced(Node** arr, int i, int ns);
    void splice(Node* node);
    
public:
    Ex8_8(): root{nullptr} {}
    ~Ex8_8(){
        if(root) destroyTree(root);
    }
    
    void add(const T& t);
    void remove(const T& t);
    
    void inorderTraversal() const;
    void bfs() const;
};

template<typename T>
void Ex8_8<T>::remove(const T& t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        Node* par=nullptr;
        if(!prev->list_p[0] || !prev->list_p[1]){
            par=prev->list_p[2];
            updateSize(prev, true);
            splice(prev);
            deleteNode(prev);
        }else{
            Node* curr=prev->list_p[1];
            while (curr->list_p[0]) {
                curr=curr->list_p[0];
            }
            par=curr->list_p[2];
            prev->info=curr->info;
            updateSize(curr, true);
            splice(curr);
            deleteNode(curr);
        }
        if(par) rebuild(par);
    }
}

template<typename T>
void Ex8_8<T>::splice(Node* node){
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
typename Ex8_8<T>::Node* Ex8_8<T>::buildBalanced(Node **arr, int i, int ns){
    if(ns==0) return nullptr;
    int m=ns/2;
    arr[i+m]->size=1;
    for(int r=0; r<2; ++r){
        arr[i+m]->list_p[r]=buildBalanced(arr, r==0? i: i+m+1, r==0? m:ns-m-1);
        if(arr[i+m]->list_p[r]){
            arr[i+m]->list_p[r]->list_p[2]=arr[i+m];
            arr[i+m]->size+=arr[i+m]->list_p[r]->size;
        }
    }
    return arr[i+m];
}

template<typename T>
int Ex8_8<T>::packToArray(Node *node, Node **arr, int i){
    if(!node) return i;
    i=packToArray(node->list_p[0], arr, i);
    arr[i++]=node;
    return packToArray(node->list_p[1], arr, i);
}

template<typename T>
void Ex8_8<T>::buildSubTree(Node* node){
    int ns=node->size;
    if(ns<2){
        return;
    }
    Node** arr=new Node*[ns];
    packToArray(node, arr, 0);
    Node* par=node->list_p[2];
    if(!par){
        root=buildBalanced(arr, 0, ns);
        root->list_p[2]=nullptr;
    }else{
        int r=par->list_p[0]? 0:1;
        par->list_p[r]=buildBalanced(arr, 0, ns);
        par->list_p[r]->list_p[2]=par;
    }
    
    delete [] arr;
}

template<typename T>
void Ex8_8<T>::rebuild(Node* node){
    Node* par=node->list_p[2];
    while(par){
        if(2*par->size<3*node->size ){
            buildSubTree(par);
        }
        node=par;
        par=node->list_p[2];
    }
}

template<typename T>
void Ex8_8<T>::add(const T &t){
    Node* prev=findPrevNode(t);
    Node* node=new Node(t);
    if(addChild(prev, node)){
        if(prev) rebuild(prev);
        return;
    }
    deleteNode(node);
}

template<typename T>
void Ex8_8<T>::updateSize(Node *node, bool decrease){
    if(decrease){
        while(node){
            node->size--;
            node=node->list_p[2];
        }
    }else{
        while(node){
            node->size++;
            node=node->list_p[2];
        }
    }
}

template<typename T>
bool Ex8_8<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info == node->info) return false;
    if(!prev) {
        root=node;
    }else{
        prev->list_p[prev->info>node->info? 0:1]=node;
        node->list_p[2]=prev;
        updateSize(prev, false);
    }
    return true;
}

template<typename T>
typename Ex8_8<T>::Node* Ex8_8<T>::findPrevNode(const T& t) const{
    Node* node=root;
    Node* prev=nullptr;
    while (node) {
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
void Ex8_8<T>::destroyTree(Node *node){
    if(node){
        destroyTree(node->list_p[0]);
        destroyTree(node->list_p[1]);
        deleteNode(node);
    }
}

template<typename T>
const int Ex8_8<T>::getSize(Node* node) const{
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

template<typename T>
void Ex8_8<T>::inorderTraversal() const{
    Node* node=root;
    Stack<Node*> stack;
    while (node || !stack.isEmpty()) {
        if(node){
            stack.push(node);
            node=node->list_p[0];
        }else{
            node=stack.top();
            stack.pop();

            std::cout<<"("<<node->info<<","<<node->size-getSize(node)<<")"<<std::setw(6);
            node=node->list_p[1];
        }
    }
    std::cout<<"\n";
}

template<typename T>
void Ex8_8<T>::bfs() const{
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

#endif /* Ex8_8_h */
