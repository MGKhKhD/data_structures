//
//  Ex7_10v2.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-22.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex7_10v2_h
#define Ex7_10v2_h

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Stack.hpp"
#include "Queue.hpp"

template<typename T>
class Ex7_10v2{
    struct Node{
        T info;
        int p;
        int size;
        Node** list_p;
        
        explicit Node(const T& t): info{t}, p{std::rand()}, size{1}, list_p{new Node*[3]} {
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
    
    void destroyTree(Node* node);
    Node* findPrevNode(const T& t) const;
    bool addChild(Node* prev, Node* node);
    void updateSize(Node* node, bool decrease);
    const int getSize(Node* node) const;
    void trickleDown(Node* node);
    void bubbleUp(Node* node);
    void rotate(Node* node, int direction);
    void splice(Node* node);
    
public:
    Ex7_10v2(): root{nullptr}, n{0} {}
    ~Ex7_10v2(){
        if(root)
            destroyTree(root);
    }
    
    const int getLen() const{return n;}
    void add(const T& t);
    void remove(const T& t);
    const T getLE(const int i) const;
    T removeLE(const int i);
    void inorderTraversal() const;
};

template<typename T>
T Ex7_10v2<T>::removeLE(const int ind){
    assert(ind>=0 && ind<n);
    
    int i=ind;
    T res=static_cast<T>(NULL);
    Node* node=root;
    while (node) {
        int ind_n=0;
        if(node->list_p[0]) ind_n=node->list_p[0]->size;
        if(i==ind_n){
            res=node->info;
            trickleDown(node);
            Node* prev=node->list_p[2];
            splice(node);
            deleteNode(node);
            n--;
            if(prev) updateSize(prev, true);
            return res;
        }else if(i<ind_n){
            node=node->list_p[0];
        }else{
            i-=ind_n+1;
            node=node->list_p[1];
        }
    }
    return res;
}

template<typename T>
const T Ex7_10v2<T>::getLE(const int ind) const{
    assert(ind>=0 && ind<n);
    
    int i=ind;
    T res=static_cast<T>(NULL);
    Node* node=root;
    while (node) {
        int ind_n=0;
        if(node->list_p[0]) ind_n=node->list_p[0]->size;
        if(i==ind_n){
            res=node->info;
            return res;
        }else if(i<ind_n){
            node=node->list_p[0];
        }else{
            i-=ind_n+1;
            node=node->list_p[1];
        }
    }

    return res;
}

template<typename T>
void Ex7_10v2<T>::trickleDown(Node *node){
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
void Ex7_10v2<T>::splice(Node* node){
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
void Ex7_10v2<T>::remove(const T& t){
    Node* node=findPrevNode(t);
    if(node && node->info==t){
        trickleDown(node);
        Node* prev=node->list_p[2];
        splice(node);
        deleteNode(node);
        n--;
        if(prev) updateSize(prev, true);
    }
}

template<typename T>
void Ex7_10v2<T>::bubbleUp(Node* node){
    while (node->list_p[2] && node->list_p[2]->p>node->p) {
        rotate(node->list_p[2], node->list_p[2]->list_p[0]==node? 0:1);
    }
    if(!node->list_p[2])
        root=node;
}

template<typename T>
void Ex7_10v2<T>::rotate(Node *node, int dir){
    Node* w=node->list_p[dir];
    w->list_p[2]=node->list_p[2];
    if(w->list_p[2])
        w->list_p[2]->list_p[node->list_p[2]->list_p[0]==node? 0:1]=w;
    int r=(dir+1)%2;
    node->list_p[dir]=w->list_p[r];
    node->size-=w->size;
    if(node->list_p[dir])
    {
        node->list_p[dir]->list_p[2]=node;
        node->size+=node->list_p[dir]->size;
        w->size-=node->list_p[dir]->size;
    }
    node->list_p[2]=w;
    w->list_p[r]=node;
    w->size+=node->size;
    if(node==root){
        root=w;
        w->list_p[2]=nullptr;
    }
}

template<typename T>
void Ex7_10v2<T>::inorderTraversal() const{
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
                c=node->list_p[2]->p>node->p? '-':'+';
            std::cout<<"("<<node->info<<","<<node->size-getSize(node)<<","<<c<<")"<<std::setw(6);
            node=node->list_p[1];
        }
    }
    std::cout<<"\n";
}

template<typename T>
const int Ex7_10v2<T>::getSize(Node* node) const{
    Stack<Node*> stack;
    int s=0;
    while(node || !stack.isEmpty()){
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
void Ex7_10v2<T>::updateSize(Node* node, bool decrease){
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
bool Ex7_10v2<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info == node->info) return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info > node->info? 0:1]=node;
        node->list_p[2]=prev;
        updateSize(prev, false);
    }
    return true;
}

template<typename T>
void Ex7_10v2<T>::add(const T& t){
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
typename Ex7_10v2<T>::Node* Ex7_10v2<T>::findPrevNode(const T &t) const{
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
void Ex7_10v2<T>::destroyTree(Node *node){
    if(node){
        destroyTree(node->list_p[0]);
        destroyTree(node->list_p[1]);
        deleteNode(node);
        n--;
    }
}

#endif /* Ex7_10v2_h */
