//
//  Treap_A.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-14.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Treap_A_h
#define Treap_A_h

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "Queue.hpp"

template<typename T>
class Treap_A{
    struct Node{
        T info;
        Node** list_p;
        int p;
        
        explicit Node(const T& t): info{t}, list_p{new Node*[3]}, p{std::rand()} {
            for(int i=0; i<=2; ++i)
                list_p[i]=nullptr;
        }
        
    };
    Node* root;
    
    Node* findPrevNode(const T& t) const{
        Node* curr=root;
        Node* prev=nullptr;
        while(curr){
            if(curr->info==t){
                return curr;
            }else{
                prev=curr;
                curr=curr->list_p[curr->info>t? 0: 1];
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
            prev->list_p[prev->info>node->info? 0: 1]=node;
            node->list_p[2]=prev;
        }
        return true;
    }
    
    void bubbleUp(Node* node){
        while(node->list_p[2] && node->list_p[2]->p > node->p){
            rotate(node->list_p[2], node==node->list_p[2]->list_p[0]? 1: 0);//r=1: rotate right; r=0: rotate left
        }
        
        if(!node->list_p[2])
            root=node;
    }
    
    void rotate(Node* node, int r){
        int r_hat=(r+1)%2;
        Node* w=node->list_p[r_hat];
        w->list_p[2]=node->list_p[2];
        if(w->list_p[2])
            w->list_p[2]->list_p[node==node->list_p[2]->list_p[0]? 0: 1]=w;
        
        node->list_p[r_hat]=w->list_p[r];
        if(node->list_p[r_hat])
            node->list_p[r_hat]->list_p[2]=node;
        node->list_p[2]=w;
        w->list_p[r_hat]=node;
        if(node==root){
            root=w;
            w->list_p[2]=nullptr;
        }
    }
    
    void inorder(Node* node) const{
        if(node){
            int pri=0;
            if(node->list_p[2])
                pri=node->list_p[2]->p;
            inorder(node->list_p[0]);
            std::cout<<"("<<node->info<<","<<node->p - pri<<")"<<std::setw(6);
            inorder(node->list_p[1]);
        }
    }
    
    void splice(Node* node){
        Node* succ;
        succ=node->list_p[node->list_p[0]? 0: 1];
        
        Node* par=nullptr;
        if(node==root){
            root=succ;
        }else{
            par=node->list_p[2];
            par->list_p[par->list_p[0]==node? 0: 1]=succ;
        }
        if(succ)
            succ->list_p[2]=par;
            
    }
    
    void trickleDown(Node* node){
        while(node->list_p[0] || node->list_p[1]){
            
            if(!node->list_p[0]){
                rotate(node, 0);
            }else if(!node->list_p[1]){
                rotate(node, 1);
            }else{
                rotate(node, node->list_p[0]->p <node->list_p[1]->p? 1: 0);
            }
            
            if(node==root){
                root=node->list_p[2];
            }
        }
    }
    
    void deleteNode(Node* node){
        if(node){
            deleteNode(node->list_p[0]);
            deleteNode(node->list_p[1]);
            delete [] node->list_p;
            delete node;
            node=nullptr;
        }
    }
    
public:
    Treap_A(): root{nullptr} {}
    ~Treap_A(){
        deleteNode(root);
    }
    
    void add(const T& t);
    void remove(const T& t);
    
    void inorderTraversal() const{
        inorder(root);
        std::cout<<"\n";
    }
    
    void breathFirstPrint() const;
};

template<typename T>
void Treap_A<T>::remove(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        trickleDown(prev);
        splice(prev);
        delete [] prev->list_p;
        delete prev;
        prev=nullptr;
    }
}

template<typename T>
void Treap_A<T>::add(const T &t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        bubbleUp(node);
        return;
    }
    delete [] node->list_p;
    delete node;
    node=nullptr;
}

template<typename T>
void Treap_A<T>::breathFirstPrint() const{
    Node* curr=root;
    Queue<Node*> q;
    q.enqueue(curr);
    int s=q.getSize();
    while(!q.isEmpty()){
        if(s==0)
            std::cout<<"\n";
        s=q.getSize();
        while (s>0) {
            curr=q.dequeue();
            std::cout<<curr->info<<std::setw(6);
            if(curr->list_p[0]) q.enqueue(curr->list_p[0]);
            if(curr->list_p[1]) q.enqueue(curr->list_p[1]);
            s--;
        }
    }
    std::cout<<"\n";
}


#endif /* Treap_A_h */
