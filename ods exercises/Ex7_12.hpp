//
//  Ex7_12.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-17.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex7_12_h
#define Ex7_12_h

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Stack.hpp"

template<typename T>
class Ex7_12{
    struct Node{
        T info;
        Node** list_p;
        int p;
        
        explicit Node(const T& t): info{t}, list_p{new Node*[3]}, p{std::rand()} {
            for(int i=0; i<3; ++i)
                list_p[i]=nullptr;
        }
        
        Node(const T& t, int pv): info{t}, list_p{new Node*[3]}, p{pv} {
            for(int i=0; i<3; ++i)
                list_p[i]=nullptr;
        }
    };
    Node* root;
    
    void deleteNode(Node* node){
        delete [] node->list_p;
        delete node;
        node=nullptr;
    }
    
    void copyTree(Node* &node, Node* otherNode){
        if(otherNode){
            node=new Node(otherNode->info, otherNode->p);
            copyTree(node->list_p[0], otherNode->list_p[0]);
            copyTree(node->list_p[1], otherNode->list_p[1]);
            if(node->list_p[0]) node->list_p[0]->list_p[2]=node;
            if(node->list_p[1]) node->list_p[1]->list_p[2]=node;
        }else{
            node=nullptr;
        }
    }
    
    
    Node* findPrevNode(const T& t) const{
        Node* curr=root;
        Node* prev=nullptr;
        while (curr) {
            if(curr->info==t){
                return curr;
            }else{
                prev=curr;
                curr=curr->list_p[curr->info > t? 0:1];
            }
        }
        return prev;
    }
    
    bool addChild(Node* prev, Node* node){
        if(prev && prev->info == node->info)
            return false;
        if(!prev){
            root=node;
        }else{
            prev->list_p[prev->info>node->info? 0:1]=node;
            node->list_p[2]=prev;
        }
        return true;
    }
    
    void inorder(Node* node) const{
        if(node){
            char c='+';
            if(node->list_p[2]){
                c=node->list_p[2]->p < node->p? '+': '-';
            }
            inorder(node->list_p[0]);
            std::cout<<"("<<node->info<<","<<c<<")"<<std::setw(6);
            inorder(node->list_p[1]);
        }
    }
    
    void deleteTree(Node* node);
    void bubbleUP(Node* node);
    void rotate(Node* node, int r);//r=0 rotate right; r=1: rotate left
    void trickleDown(Node* node);
    void splice(Node* node);
    
public:
    Ex7_12(): root{nullptr} {}
    Ex7_12(const Ex7_12<T>& other): Ex7_12(){
        if(other.root){
            copyTree(root, other.root);
        }else{
            root=nullptr;
        }
    }
    ~Ex7_12(){
        if(root)
            deleteTree(root);
    }
    
    void add(const T& t);
    void remove(const T& t);
    Ex7_12<T> splite(const T& t);
    
    void inorderTraversal() const{
        inorder(root);
        std::cout<<"\n";
    }
};

template<typename T>
void Ex7_12<T>::remove(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        trickleDown(prev);
        splice(prev);
        deleteNode(prev);
    }
}

template<typename T>
void Ex7_12<T>::trickleDown(Node *node){
    while (node->list_p[0] || node->list_p[1]) {
        if(!node->list_p[0]){
            rotate(node, 1);
        }else if(!node->list_p[1]){
            rotate(node, 0);
        }else{
            rotate(node, node->list_p[0]->p < node->list_p[1]->p? 1: 0);
        }
        if(node==root)
            root=node->list_p[2];
    }
}


template<typename T>
void Ex7_12<T>::splice(Node *node){
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
void Ex7_12<T>::add(const T& t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        bubbleUP(node);
        return;
    }
    deleteNode(node);
}

template<typename T>
void Ex7_12<T>::bubbleUP(Node *node){
    while (node->list_p[2] && node->list_p[2]->p > node->p) {
        rotate(node->list_p[2], node->list_p[2]->list_p[0]==node? 0:1);
    }
    if(!node->list_p[2])
        root=node;
}

template<typename T>
void Ex7_12<T>::rotate(Node *node, int r){
    Node* w=node->list_p[r];
    w->list_p[2]=node->list_p[2];
    if(node->list_p[2])
        w->list_p[2]->list_p[node==node->list_p[2]->list_p[0]? 0:1]=w;
    int r_hat=(r+1)%2;
    node->list_p[r]=w->list_p[r_hat];
    if(node->list_p[r])
        node->list_p[r]->list_p[2]=node;
    node->list_p[2]=w;
    w->list_p[r_hat]=node;
    if(node==root){
        root=w;
        w->list_p[2]=nullptr;
    }
}


template<typename T>
void Ex7_12<T>:: deleteTree(Node* node){
    if(node){
        deleteTree(node->list_p[0]);
        deleteTree(node->list_p[1]);
        deleteNode(node);
    }
}

template<typename T>
Ex7_12<T> Ex7_12<T>::splite(const T &t){
    Node* node=root;
    Stack<Node*> s;
    Ex7_12 tr;
    while(node || !s.isEmpty()){
        if(node){
            s.push(node);
            node=node->list_p[0];
        }else{
            node=s.top();
            Node* tmp=node;
            node=node->list_p[1];
            s.pop();
            if(tmp->info >=t)
            {
                tr.add(tmp->info);
                remove(tmp->info);
            }
            
        }
    }
    return tr;
}


#endif /* Ex7_12_h */
