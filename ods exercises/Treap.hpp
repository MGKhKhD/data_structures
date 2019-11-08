//
//  Treap.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-14.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Treap_h
#define Treap_h

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "Queue.hpp"

template<typename T>
class Treap{
    struct Node{
        T info;
        Node* parent, *left, *right;
        int p;
        
        explicit Node(const T& t): info{t}, parent{nullptr}, right{nullptr},left{nullptr}, p{std::rand()} {}
        Node(const T& t, const int pv): info{t}, parent{nullptr}, left{nullptr}, right{nullptr}, p{pv}{ }
    };
    Node* root;
    
    Node* findPrevNode(const T& t) const{
        Node* prev=nullptr;
        Node* curr=root;
        while(curr){
            if(curr->info == t){
                return curr;
            }else{
                prev=curr;
                if(curr->info > t){
                    curr=curr->left;
                }else{
                    curr=curr->right;
                }
            }
        }
        return prev;
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
    
    void destoryTree(Node* node){
        if(node){
            destoryTree(node->left);
            destoryTree(node->right);
            delete node;
            node=nullptr;
        }
    }
    
    bool addChild(Node* prev, Node* node){
        if(prev && prev->info==node->info)
            return false;
        if(!prev){
            root=node;
        }else{
            if(prev->info > node->info){
                prev->left=node;
            }else{
                prev->right=node;
            }
            node->parent=prev;
        }
        return true;
    }
    
    void setNewParent(Node* w, Node* node){
        w->parent=node->parent;
        if(node->parent){
            if(node->parent->left == node){
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
    
    void bubbleUp(Node* node){
        while(node->parent && node->parent->p> node->p){
            if(node->parent->left==node){
                rotateRight(node->parent);
            }else{
                rotateLeft(node->parent);
            }
        }
        if(!node->parent){
            root=node;
        }
    }
    
    void splice(Node* node){
        Node* succ;
        if(node->left){
            succ=node->left;
        }else{
            succ=node->right;
        }
        
        Node* par=nullptr;
        if(node==root){
            root=succ;
        }else{
            par=node->parent;
            if(par->left==node){
                par->left=succ;
            }else{
                par->right=succ;
            }
        }
        if(succ)
            succ->parent=par;
    }
    
    void trickleDown(Node* node){
        while(node->left || node->right){
            if(!node->left){
                rotateLeft(node);
            }else if(!node->right){
                rotateRight(node);
            }else if(node->left->p < node->right->p){
                rotateRight(node);
            }else{
                rotateLeft(node);
            }
            if(node==root){
                root=node->parent;
            }
        }
    }
    
    void copyTree(Node* &node, Node* ct){
        if(ct){
            node=new Node(ct->info, ct->p);
            copyTree(node->left, ct->left);
            copyTree(node->right, ct->right);
            if(node->left) node->left->parent=node;
            if(node->right) node->right->parent=node;
        }
    }
    
    void swap(Treap<T>& other) noexcept{
        std::swap(root, other.root);
    }
    
public:
    Treap(): root{nullptr} {}
    Treap(const Treap<T>& t);
    Treap<T>& operator=(const Treap<T>& t);
    Treap(Treap<T>&& t);
    Treap<T>& operator=(Treap<T>&& t);
    ~Treap(){destoryTree(root);}
    
    void add(const T& t);
    void remove(const T& t);
    
    void inorderTraversal() const{
        inorder(root);
        std::cout<<"\n";
    }
    void breathFirstPrint() const;
    
};

template<typename T>
Treap<T>& Treap<T>::operator=(Treap<T>&& t){
    Treap<T> c{std::move(t)};
    swap(c);
    return *this;
}

template<typename T>
Treap<T>::Treap(Treap<T>&& t): root{t.root}{
    t.root=nullptr;
}

template<typename T>
Treap<T>& Treap<T>::operator=(const Treap<T> &t){
    if(this!=&t){
        Treap<T> c{t};
        swap(c);
    }
    return *this;
}

template<typename T>
Treap<T>::Treap(const Treap<T>& t): Treap(){
    if(t.root){
        copyTree(root, t.root);
    }else{
        root=nullptr;
    }
}

template<typename T>
void Treap<T>::remove(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info == t){
        trickleDown(prev);
        splice(prev);
        delete prev;
        prev=nullptr;
    }
}

template<typename T>
void Treap<T>::add(const T &t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        bubbleUp(node);
        return;
    }
    delete node;
    node=nullptr;
}

template<typename T>
void Treap<T>::breathFirstPrint() const{
    Node* node=root;
    Queue<Node*> q;
    q.enqueue(node);
    int s=q.getSize();
    while(!q.isEmpty()){
        if(s==0)
            std::cout<<"\n";
        s=q.getSize();
        while (s>0) {
            node=q.dequeue();
            std::cout<<node->info<<std::setw(6);
            if(node->left) q.enqueue(node->left);
            if(node->right) q.enqueue(node->right);
            s--;
        }
    }
    std::cout<<"\n";
}

#endif /* Treap_h */
