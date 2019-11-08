//
//  Ex6_6.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-28.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex6_6_h
#define Ex6_6_h

#include "Stack.hpp"
#include "Queue.hpp"
#include <iostream>
#include <iomanip>
#include <optional>

template<typename T>
class Ex6_6{
    struct Node{
        T info;
        int size;
        Node** list_p;
        T* range;
        
        explicit Node(const T& t): info{t}, size{1}, list_p{new Node*[3]}, range{new T[2]} {
            for(int i=0; i<3; list_p[i]=nullptr, ++i);
            range[0]=range[1]=t;
        }
    };
    Node* root;
    mutable Stack<Node*> finger;
    int n;
    
    template<typename Func>
    void configurableTravers(Node* node, Func func) const{
        Stack<Node*> s;
        while (!s.isEmpty() || node) {
            if(node){
                s.push(node);
                node=node->list_p[0];
            }else{
                node=s.top();
                s.pop();
                func(node);
                node=node->list_p[1];
            }
        }
    }
    
    void updateSize(Node* node, bool increase);
    void deleteNode(Node* node);
    void deleteTree(Node* node);
    void searchFinger(const T& t) const;
    Node* findPrevNode(const T& t) const;
    bool addChild(Node* prev, Node* node);
    const int getSize(Node* node) const;
    const T getRange(Node* node, const int r) const;
    void updateRange(Node* node);
    bool setRange(Node* node);
    void splice(Node* node);
    
public:
    Ex6_6(): root{nullptr}, n{0} {}
    ~Ex6_6(){
        while (!finger.isEmpty()) {
            finger.pop();
        }
        if(root) deleteTree(root);
    }
    
    void add(const T& t);
    void remove(const T& t);
    void inorderTraversal() const;
    bool isBalanced() const;
};

template<typename T>
bool Ex6_6<T>::isBalanced() const{
    int s=0;
    auto checkBalance{[&s](Node* node){
        if(node){
            int rightSize=node->list_p[1]? node->list_p[1]->size: 0;
            int leftSize=node->list_p[0]? node->list_p[0]->size: 0;
            if(std::abs(rightSize-leftSize)<=1) s++;
        }
    }};
    configurableTravers(root, checkBalance);
    return n==s;
}

template<typename T>
void Ex6_6<T>::remove(const T& t){
    Node* node=findPrevNode(t);
    if(node && node->info==t){
        Node* par=nullptr;
        if(!node->list_p[0] || !node->list_p[1]){
            par=node->list_p[2];
            splice(node);
            deleteNode(node);
        }else{
            Node* curr=node->list_p[1];
            while (curr->list_p[0]) {
                curr=curr->list_p[0];
            }
            node->info=curr->info;
            updateRange(node);
            par=curr->list_p[2];
            splice(curr);
            deleteNode(curr);
        }
        if(par){
            updateSize(par, false);
            updateRange(par);
        }
        n--;
    }
}

template<typename T>
void Ex6_6<T>::splice(Node* node){
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
bool Ex6_6<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info == node->info) return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info > node->info? 0:1]=node;
        node->list_p[2]=prev;
    }
    return true;
}

template<typename T>
void Ex6_6<T>::updateSize(Node* node, bool increase){
    if(increase){
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
void Ex6_6<T>::add(const T& t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        if(prev) {
            updateSize(prev, true);
            updateRange(prev);
        }
        n++;
        return;
    }
    deleteNode(node);
}

template<typename T>
bool Ex6_6<T>::setRange(Node* node){
    T upRange=node->range[0];
    T lowRang=node->range[1];
    node->range[0]=node->list_p[0]? std::min(node->list_p[0]->range[0], lowRang): node->info;
    node->range[1]=node->list_p[1]? std::max(node->list_p[1]->range[1], upRange): node->info;
    if(node->range[0]==lowRang && node->range[1]==upRange) return true;
    return false;
}

template<typename T>
void Ex6_6<T>::updateRange(Node* node){
    bool stop{false};
    while (node && !stop) {
        stop=setRange(node);
        node=node->list_p[2];
    }
}

template<typename T>
typename Ex6_6<T>::Node* Ex6_6<T>::findPrevNode(const T &t) const{
    Node* curr=root;
    searchFinger(t);
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

template<typename T>
void Ex6_6<T>::searchFinger(const T& t) const{
    if(!finger.isEmpty()){
        while (!finger.isEmpty()) {
            Node* node=finger.top();
            if(node->range[0]<=t && node->range[1]>=t)
                return;
            finger.pop();
        }
    }
}

template<typename T>
void Ex6_6<T>::deleteTree(Node* node){
    if(node){
        deleteTree(node->list_p[0]);
        deleteTree(node->list_p[1]);
        deleteNode(node);
    }
}

template<typename T>
void Ex6_6<T>::deleteNode(Node* node){
    if(node){
        delete [] node->list_p;
        delete [] node->range;
        delete node;
        node=nullptr;
    }
}

template<typename T>
const int Ex6_6<T>::getSize(Node* node) const{
    int s=0;
    configurableTravers(node, [&s](Node* tmp){if(tmp){
        s++;
    }});
    return s;
}

template<typename T>
const T Ex6_6<T>::getRange(Node *node, const int r) const{
    T rangeVal=node->range[r];
    while (node->list_p[r]) {
        node=node->list_p[r];
        rangeVal=node->range[r];
    }
    return rangeVal;
}

template<typename T>
void Ex6_6<T>::inorderTraversal() const{
    auto nodeSize{[this](Node* node){return getSize(node);}};
    auto nodeRange{[this](Node* node, const int r){return getRange(node, r);}};
    auto printNode{
        [nodeSize, nodeRange](Node* node){
            std::cout<<"("<<node->info<<","<<node->size-nodeSize(node)<<","<<node->range[0]-nodeRange(node, 0)<<","<<node->range[1]-nodeRange(node, 1)<<")"<<std::setw(6);
        }
    };
    configurableTravers(root, printNode);
    std::cout<<"\n";
}


#endif /* Ex6_6_h */
