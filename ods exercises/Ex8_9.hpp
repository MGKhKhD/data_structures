//
//  Ex8_9.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-29.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex8_9_h
#define Ex8_9_h

#include <array>
#include <vector>
#include "algorithm"
#include <iostream>
#include <iomanip>
#include "Stack.hpp"
#include "Queue.hpp"


template<typename T, size_t Timer=10>
class Ex8_9{
    struct Node{
        T info;
        std::array<Node*, 3> list_p{nullptr};
        std::array<T, 2> range;
        size_t size;
        size_t timer{Timer};
        
        explicit Node(const T& t): info{t}, size{1} {
            range[0]=range[1]=t;
        }
    };
    Node* root;
    mutable Stack<Node*> finger;
    
    template<typename Func>
    void climbTree(Node* node, Func manipFunc){
        while(node){
            manipFunc(node);
            node=node->list_p[2];
        }
    }
    
    template<typename Func>
    void configurableTraverse(Node* node, Func manipFunc) const{
        Stack<Node*> stack;
        while(node || !stack.isEmpty()){
            if(node){
                stack.push(node);
                node=node->list_p[0];
            }else{
                node=stack.top();
                stack.pop();
                manipFunc(node);
                node=node->list_p[1];
            }
        }
    }
    
    void destroyTree(Node* node);
    void searchFinger(const T& t) const;
    Node* findPrevNode(const T& t) const;
    bool addChild(Node* prev, Node* node);
    void updateSize(Node* node, bool increase);
    void updateRange(Node* node);
    bool setRange(Node* node);
    const int getSizeNode(Node* node) const;
    const T getRange(Node* node, const int r) const;
    void rebuildSubTree(Node* node);
    size_t packToArray(Node* node, std::vector<Node*>& vec, size_t i);
    Node* buildBalanced(std::vector<Node*>& vec, size_t i, size_t ns);
    void splice(Node* node);
    
public:
    Ex8_9(): root{nullptr} {}
    ~Ex8_9(){
        finger.~Stack<Node*>();
        if(root){
            destroyTree(root);
        }
    }
    
    void add(const T& t);
    void remove(const T& t);
    void inorderTraversal() const;
    void bfs() const;
};

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::remove(const T& t){
    Node* node=findPrevNode(t);
    if(node && node->info==t){
        Node* prev=nullptr;
        if(node->list_p[0]==nullptr || node->list_p[1]==nullptr){
            prev=node->list_p[2];
            splice(node);
            delete node;
            node=nullptr;
        }else{
            Node* curr=node->list_p[1];
            while (curr->list_p[0]) {
                curr=curr->list_p[0];
            }
            node->info=curr->info;
            updateRange(node);
            prev=curr->list_p[2];
            splice(curr);
            delete curr;
            curr=nullptr;
        }
        if(prev){
            updateSize(prev, false);
            updateRange(prev);
        }
    }
}

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::splice(Node* node){
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

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::rebuildSubTree(Node* node){
    if(node->size<2) return;
    std::vector<Node*> vec;
    vec.reserve(node->size);
    packToArray(node, vec, 0);
    Node* par=node->list_p[2];
    if(!par){
        root=buildBalanced(vec, 0, node->size);
        root->list_p[2]=nullptr;
    }else{
        int r=par->list_p[0]==node? 0:1;
        par->list_p[r]=buildBalanced(vec, 0, node->size);
        par->list_p[r]->list_p[2]=par;
    }
}

template<typename T, size_t Timer>
size_t Ex8_9<T,Timer>::packToArray(Node *node, std::vector<Node *> &vec, size_t i){
    if(!node) return i;
    i=packToArray(node->list_p[0], vec, i);
    vec[i++]=node;
    return packToArray(node->list_p[1], vec, i);
}

template<typename T, size_t Timer>
typename Ex8_9<T,Timer>::Node* Ex8_9<T,Timer>::buildBalanced(std::vector<Node *> &vec, size_t i, size_t ns){
    if(ns==0) return nullptr;
    size_t m=ns/2;
    vec[m+i]->size=1;
    for(int r=0; r<2; ++r){
        vec[m+i]->list_p[r]=buildBalanced(vec, r==0? i: i+m+1, r==1? m: ns-m-1);
        if(vec[m+i]->list_p[r]){
            vec[m+i]->list_p[r]->list_p[2]=vec[m+i];
            vec[i+m]->size+=vec[m+i]->list_p[r]->size;
        }
        vec[i+m]->range[r]=getRange(vec[i+m], r);
        vec[i+m]->timer=Timer;
    }
    return vec[i+m];
}


template<typename T, size_t Timer>
void Ex8_9<T,Timer>::add(const T& t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        if(prev){
            updateSize(prev, true);
            updateRange(prev);
            climbTree(prev, [this](Node* tmp){
                if(tmp) {
                    tmp->timer--;
                    if(tmp->timer<=0){
                        tmp->timer=Timer;
                        rebuildSubTree(tmp);
                    }
                }
                
            });
        }
        return;
    }
    delete node;
    node=nullptr;
}

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::updateRange(Node* node){
    bool stop{false};
    while (!stop && node) {
        stop=setRange(node);
        node=node->list_p[2];
    }
}

template<typename T, size_t Timer>
bool Ex8_9<T,Timer>::setRange(Node* node){
    T upRange=node->range[0];
    T lowRange=node->range[1];
    node->range[0]=node->list_p[0]? std::min(node->list_p[0]->range[0], lowRange): node->info;
    node->range[1]=node->list_p[1]? std::max(node->list_p[1]->range[1], upRange): node->info;
    if(node->range[0]==lowRange && node->range[1]==upRange) return true;
    return false;
}

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::updateSize(Node *node, bool increase){
    if(increase){
        climbTree(node, [this](Node* tmp){tmp->size++;});
    }else{
        climbTree(node, [this](Node* tmp){tmp->size--;});
    }
}

template<typename T, size_t Timer>
bool Ex8_9<T,Timer>::addChild(Node *prev, Node *node){
    if(prev && prev->info == node->info) return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info > node->info? 0:1]=node;
        node->list_p[2]=prev;
    }
    return true;
}

template<typename T, size_t Timer>
typename Ex8_9<T,Timer>::Node* Ex8_9<T,Timer>::findPrevNode(const T& t) const{
    searchFinger(t);
    Node* curr=root;
    if(!finger.isEmpty()){
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

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::searchFinger(const T& t) const{
    if(finger.isEmpty()) return;
    while (!finger.isEmpty()) {
        Node* node=finger.top();
        if(node->range[0]<=t && node->range[1]>=t) return;
        finger.pop();
    }
}

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::destroyTree(Node* node){
    if(node){
        destroyTree(node->list_p[0]);
        destroyTree(node->list_p[1]);
        delete node;
        node=nullptr;
    }
}

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::inorderTraversal() const{
    Node* node=root;
    auto diffNodeSize{[this](Node* tmp){
        return tmp->size-getSizeNode(tmp);
    }};
    auto diffRange{
        [this](Node* tmp, const int r){
            return tmp->range[r]-getRange(tmp, r);
        }
    };
    auto printNode{
        [diffNodeSize, diffRange](Node* node){
            std::cout<<"("<<node->info<<","<<diffNodeSize(node)<<","<<diffRange(node, 0)<<","<<diffRange(node, 1)<<")"<<std::setw(6);
        }
    };
    configurableTraverse(node, printNode);
    std::cout<<"\n";
}

template<typename T, size_t Timer>
const int Ex8_9<T,Timer>::getSizeNode(Node* node) const{
    int s=0;
    configurableTraverse(node, [&s](Node* tmp){
        if(tmp) s++;
    });
    return s;
}

template<typename T, size_t Timer>
const T Ex8_9<T,Timer>::getRange(Node *node, const int r) const{
    T rng=node->range[r];
    while (node->list_p[r]) {
        rng=node->range[r];
        node=node->list_p[r];
    }
    return rng;
}

template<typename T, size_t Timer>
void Ex8_9<T,Timer>::bfs() const{
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

#endif /* Ex8_9_h */
