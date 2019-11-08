//
//  Ex7_8.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-14.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex7_8_h
#define Ex7_8_h

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "ArrayImpl.hpp"

template<typename T>
class Ex7_8{
    struct Node{
        T info;
        Node* parent, *left, *right;
        int p;
        
        explicit Node(const T& t): info{t}, parent{nullptr}, left{nullptr}, right{nullptr}, p{std::rand()} {
        }
        
    };
    Node* root;

    Node* buildTreap(ArrayImpl<Node*> list, size_t i, size_t ns){
        if(ns==0) return nullptr;
        
        size_t m=ns/2;
        list[i+m]->left=buildTreap(list, i, m);
        if(list[i+m]->left){
            list[i+m]->left->parent=list[i+m];
        }
        
        list[i+m]->right=buildTreap(list, m+i+1, ns-m-1);
        if(list[i+m]->right){
            list[i+m]->right->parent=list[i+m];
        }
        
        return list[i+m];
    }
    
//    void bubbleUp(Node* node){
//        if(node->parent){
//            std::cout<<node->p - node->parent->p<<","<<node->info<<std::endl;
//        }
//        while(node->parent && node->parent->p > node->p){
//
//            if(node->parent->left==node){
//                rotateRight(node->parent);
//            }else{
//                rotateLeft(node->parent);
//            }
//        }
//
//        if(!node->parent)
//            root=node;
//    }
    
    void setNewParent(Node* w, Node* node){
        w->parent=node->parent;
        if(node->parent){
            if(node->parent->left==node){
                w->parent->left=w;
            }else{
                w->parent->right=w;
            }
        }
    }
    
    void rotateLeft(Node* node){
        Node* w=node->right;
        setNewParent(w, node);
        
        node->right=w->left;
        if(node->right)
            node->right->parent=node;
        node->parent=w;
        w->left=node;
        if(node==root){
            root=w;
            w->parent=nullptr;
        }
    }
    
    void rotateRight(Node* node){
        Node* w=node->left;
        setNewParent(w, node);
        
        node->left=w->right;
        if(node->left)
            node->left->parent=node;
        node->parent=w;
        w->right=node;
        if(node==root){
            root=w;
            w->parent=nullptr;
        }
    }
    
    void inorder(Node* node) const{
        if(node){
            int pri=0;
            if(node->parent)
                pri=node->parent->p;
            inorder(node->left);
            std::cout<<"("<<node->info<<","<<node->p - pri<<")"<<std::setw(6);
            inorder(node->right);
        }
    }
    
public:
    Ex7_8(): root{nullptr} { }
    Ex7_8(const ArrayImpl<T>& arr);
    
    void inorderTraversal() const{
        inorder(root);
        std::cout<<"\n";
    }
};

template<typename T>
Ex7_8<T>::Ex7_8(const ArrayImpl<T>& arr): Ex7_8(){
    ArrayImpl<Node*> list{arr.len};
    for(int i=0; i<arr.len; ++i){
        list[i]=new Node(arr[i]);
    }
    root=buildTreap(list, 0, arr.len-1);
    if(root) root->parent=nullptr;
}



#endif /* Ex7_8_h */
