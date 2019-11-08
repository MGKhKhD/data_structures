//
//  Ex7_13.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-31.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex7_13_h
#define Ex7_13_h

#include <array>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Stack.hpp"
#include "Queue.hpp"

template<typename T>
class Ex7_13{
    struct Node{
        T info;
        std::array<Node*, 3> list_p;
        std::array<T, 2> range;
        size_t size;
        int p;
        
        explicit Node(const T& t): info{t}, size{1}, p{std::rand()} {
            range[0]=range[1]=t;
            list_p[0]=list_p[1]=list_p[2]=nullptr;
        }
        
        Node(const T& t, const int pv): info{t}, size{1}, p{pv} {
            range[0]=range[1]=t;
            list_p[0]=list_p[1]=list_p[2]=nullptr;
        }
        
    };
    Node* root;
    mutable Stack<Node*> finger;
    size_t n;
    
    template<typename Func>
    void configurableTravers(Node* node, Func f) const{
        Stack<Node*> stack;
        while (node || !stack.isEmpty()) {
            if(node){
                stack.push(node);
                node=node->list_p[0];
            }else{
                node=stack.top();
                stack.pop();
                Node* tmp=node->list_p[1];
                f(node);
                node=tmp;
            }
        }
    }
    
    template<typename Func>
    void climbSubTree(Node* node, Func f, bool stop){
        while (node && !stop) {
            stop=f(node);
            node=node->list_p[2];
        }
    }
    
    const T getMinMax(Node* node, const int r){
        T val=node->info;
        while (node->list_p[r]) {
            val=node->info;
            node=node->list_p[r];
        }
        return val;
    }
    
    void destroyTree(Node* node);
    void searchFinger(const T& t) const;
    Node* findPrevNode(const T& t) const;
    bool addChild(Node* prev, Node* node);
    void rotate(Node* node, const int r);
    void bubbleUP(Node* node);
    const size_t getSize(Node* node) const;
    const T getRange(Node* node, const int r) const;
    bool setRange(Node* node);
    void updateSize(Node* node, const bool increase);
    void updateRange(Node* node);
    void splice(Node* node);
    void trickleDown(Node* node);
    void copyTree(Node* &node, Node* otherNode);
    bool absorbSubTree(const Ex7_13& other);
    
public:
    Ex7_13(): root{nullptr}, n{0} {}
    Ex7_13(const Ex7_13& other){
        if(other.root){
            copyTree(root, other.root);
        }else{
            root=nullptr;
        }
    }
    ~Ex7_13(){
        finger.~Stack();
        if(root){
            destroyTree(root);
        }
    }
    
    void add(const T& t);
    void remove(const T& t);
    void absorb(const Ex7_13& tr);
    void inorderTraversal() const;
    const size_t sizeTree() const{ return n;}
};

template<typename T>
void Ex7_13<T>::copyTree(Node* &node, Node* otherNode){
    if(otherNode){
        node=new Node(otherNode->info, otherNode->p);
        node->range[0]=otherNode->range[0];
        node->range[1]=otherNode->range[1];
        node->size=otherNode->size;
        copyTree(node->list_p[0], otherNode->list_p[0]);
        copyTree(node->list_p[1], otherNode->list_p[1]);
        if(node->list_p[0]){
            node->list_p[0]->list_p[2]=node;
        }
        if(node->list_p[1]){
            node->list_p[1]->list_p[2]=node;
        }
        n++;
    }else{
        node=nullptr;
    }
}

template<typename T>
bool Ex7_13<T>::absorbSubTree(const Ex7_13<T> &tr){
    T maxVal{getMinMax(tr.root, 1)};
    Node* node=root;
    while (node->list_p[0]) {
        node=node->list_p[0];
    }
    Node* tmp=node;
    bool absorbed{false};
    if(node->info > maxVal){
        copyTree(node->list_p[0], tr.root);
        absorbed=true;
    }else{
        T minVal{getMinMax(tr.root, 0)};
        node=root;
        while (node->list_p[1]) {
            node=node->list_p[1];
        }
        tmp=node;
        if(node->info < minVal){
            copyTree(node->list_p[1], tr.root);
            absorbed=true;
        }
    }
    if(absorbed){
        const size_t s=tr.sizeTree();
        climbSubTree(tmp, [s](Node* nodeTmp){
            if(nodeTmp){
                nodeTmp->size +=s;
            }
            return false;
        }, false);
        updateRange(tmp);
    }
    return absorbed;
}

template<typename T>
void Ex7_13<T>::absorb(const Ex7_13<T> &tr){
    if(tr.root){
        if(!absorbSubTree(tr)){
            configurableTravers(tr.root, [this](Node* node){
                if(node){
                    const T val=node->info;
                    add(val);
                }
            });
        }
    }
}

template<typename T>
void Ex7_13<T>::trickleDown(Node* node){
    while (node->list_p[0] || node->list_p[1]) {
        if(!node->list_p[0]){
            rotate(node, 1);
        }else if(!node->list_p[1]){
            rotate(node, 0);
        }else{
            rotate(node, node->list_p[0]->p <node->list_p[1]->p? 0:1);
        }
        if(node==root){
            root=node->list_p[2];
        }
    }
}

template<typename T>
void Ex7_13<T>::splice(Node* node){
    Node* succ=node->list_p[node->list_p[0]? 0:1];
    Node* par=nullptr;
    if(node==root){
        root=succ;
    }else{
        par=node->list_p[2];
        par->list_p[node==par->list_p[0]? 0:1]=succ;
    }
    if(succ){
        succ->list_p[2]=succ;
    }
}

template<typename T>
void Ex7_13<T>::remove(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        trickleDown(prev);
        Node* node=prev->list_p[2];
        if(prev->list_p[0]){
            setRange(prev->list_p[0]);
        }else if(prev->list_p[1]){
            setRange(prev->list_p[1]);
        }
        splice(prev);
        delete prev;
        if(node){
            updateSize(node, false);
            updateRange(node);
        }
        n--;
    }
}

template<typename T>
void Ex7_13<T>::rotate(Node *node, const int r){
    Node* w=node->list_p[r];
    w->list_p[2]=node->list_p[2];
    if(w->list_p[2]){
        w->list_p[2]->list_p[node->list_p[2]->list_p[0]==node? 0:1]=w;
    }
    
    int r1 = (r+1)%2;
    node->list_p[r] = w->list_p[r1];
    node->size -= w->size;
    
    if( node->list_p[r] ){
        node->list_p[r]->list_p[2] = node;
        node->size += node->list_p[r]->size;
        w->size -= node->list_p[r]->size;
    }
    
    setRange(node);
    node->list_p[2]=w;
    w->list_p[r1]=node;
    w->size+=node->size;
    updateRange(w);
    if(node==root){
        root=w;
        w->list_p[2]=nullptr;
    }
    
}

template<typename T>
void Ex7_13<T>::bubbleUP(Node* node){
    while (node->list_p[2] && node->list_p[2]->p > node->p) {
        rotate(node->list_p[2], node->list_p[2]->list_p[0]==node? 0:1);
    }
    if(!node->list_p[2]){
        root=node;
    }
}

template<typename T>
void Ex7_13<T>::add(const T& t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        if(prev){
            updateSize(prev, true);
            updateRange(prev);
        }
        bubbleUP(node);
        n++;
        return;
    }
    delete node;
    node=nullptr;
}

template<typename T>
bool Ex7_13<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info==node->info) return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info>node->info? 0:1]=node;
        node->list_p[2]=prev;
    }
    return true;
}

template<typename T>
void Ex7_13<T>::searchFinger(const T& t) const{
    if(finger.isEmpty()) return;
    while (!finger.isEmpty()) {
        Node* node=finger.top();
        if(node->range[0]<=t && node->range[1]>=t) return;
        finger.pop();
    }
}

template<typename T>
typename Ex7_13<T>::Node* Ex7_13<T>::findPrevNode(const T& t) const{
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
void Ex7_13<T>::destroyTree(Node* node){
    if(node){
        destroyTree(node->list_p[0]);
        destroyTree(node->list_p[1]);
        delete node;
        node=nullptr;
    }
}

template<typename T>
void Ex7_13<T>::inorderTraversal() const{
    auto nodeSize{[this](Node* node){
        return node->size==getSize(node);
    }};
    auto nodeRange{[this](Node* node, const int r){
        return node->range[r]==getRange(node, r);
    }};
    std::cout<<std::boolalpha;
    auto printNode{[nodeSize, nodeRange](Node* node){
        std::cout<<"("<<node->info<<","<<(node->list_p[2]? (node->p - node->list_p[2]->p>0? '+': '-' ) : '+' )<<","<<nodeSize(node)<<","<<nodeRange(node,0)<<","<<nodeRange(node,1)<<")"<<std::setw(6);
    }};
    configurableTravers(root, printNode);
    std::cout<<"\n";
}

template<typename T>
const size_t Ex7_13<T>::getSize(Node* node) const{
    size_t s=0;
    configurableTravers(node, [&s](Node* tmp){
        if(tmp) s++;
    });
    return s;
}

template<typename T>
void Ex7_13<T>::updateSize(Node* node, const bool increase){
    if(increase){
        climbSubTree(node, [](Node* tmp){
            if(tmp) tmp->size++;
            return false;
        }, false);
        return;
    }
    climbSubTree(node, [](Node* tmp){
        if(tmp) tmp->size--;
        return false;
    }, false);
}

template<typename T>
bool Ex7_13<T>::setRange(Node* node){
    const T lowRange=node->range[0];
    const T upRange=node->range[1];
    node->range[0]=node->list_p[0]? std::min(node->list_p[0]->range[0], lowRange):node->info;
    node->range[1]=node->list_p[1]? std::max(node->list_p[1]->range[1], upRange):node->info;
    if(node->range[0]==lowRange && node->range[1]==upRange) return true;
    return false;
}

template<typename T>
void Ex7_13<T>::updateRange(Node* node){
    climbSubTree(node, [this](Node* tmp){
        return setRange(tmp);
    }, false);
}

template<typename T>
const T Ex7_13<T>::getRange(Node *node, const int r) const{
    T range=node->range[r];
    while (node->list_p[r]) {
        range=node->range[r];
        node=node->list_p[r];
    }
    return range;
}

#endif /* Ex7_13_h */
