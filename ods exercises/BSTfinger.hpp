//
//  BSTfinger.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-16.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef BSTfinger_h
#define BSTfinger_h

#include <iostream>
#include <iomanip>
#include "Stack.hpp"

template<typename T>
class BSTfinger{
    struct Node{
        T info;
        Node** list_p;
        T* range;
        
        explicit Node(const T& t): info{t}, list_p{new Node*[3]}, range{new T[2]} {
            for(int i=0; i<3; ++i){
                if(i<2){
                    range[i]=t;
                }
                list_p[i]=nullptr;
            }
        }
    };
    Node* root;
    Stack<Node*> finger;
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->list_p;
            delete [] node->range;
            delete node;
            node=nullptr;
        }
    }
    
    void destroyTree(Node* node){
        if(node){
            destroyTree(node->list_p[0]);
            destroyTree(node->list_p[1]);
            deleteNode(node);
        }
    }
    
    void inorder(Node* node) const{
        if(node){
            inorder(node->list_p[0]);
            std::cout<<"("<<node->range[0]- rangeOfNode(node, 0)<<","<<node->info<<","<<node->range[1]- rangeOfNode(node, 1)<<")"<<std::setw(6);
            inorder(node->list_p[1]);
        }
    }
    
    Node* findPrevNode(const T& t){
        
        searchFinger(t);
        Node* curr=nullptr;
        if(finger.isEmpty()){
            curr=root;
        }else{
            curr=finger.top();
            finger.pop();
        }

        
        Node* prev=nullptr;
        while(curr){
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
    
    bool addChild(Node* prev, Node* node){
        if(prev && prev->info==node->info)
            return false;
        if(!prev){
            root=node;
        }else{
            prev->list_p[prev->info > node->info ? 0:1]=node;
            node->list_p[2]=prev;
        }
        return true;
    }
    
    T rangeOfNode(Node* node, const int r) const;
    void updateRange(Node* prev, Node* node);
    void updateRangeAfterDeletion(Node* prev, Node* node);
    void searchFinger(const T& t);
    void splice(Node* node);
    
public:
    BSTfinger(): root{nullptr} {}
    ~BSTfinger(){
        destroyTree(root);
    }
    
    void add(const T& t);
    bool find(const T& t);
    void remove(const T& t);
    
    void inorderTraversal() const{
        inorder(root);
        std::cout<<"\n";
    }
    
};

template<typename T>
void BSTfinger<T>::splice(Node *node){
    Node* succ=node->list_p[0]? node->list_p[0]: node->list_p[1];
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
void BSTfinger<T>::updateRangeAfterDeletion(Node *node, Node *del){
    int i=node->list_p[0]==del? 0:1;
    if(del->info==del->range[i]){
        node->range[i]=node->info;
    }else{
        node->range[i]=i==0? std::min(node->info, del->range[i]): std::max(node->info, del->range[i]);
    }
    Node* par=node->list_p[2];
    if(par){
        updateRange(par, node);
    }
}

template<typename T>
void BSTfinger<T>::remove(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        if(!prev->list_p[0] || !prev->list_p[1]){
            Node* node=prev->list_p[2];
            if(node)
                updateRangeAfterDeletion(node, prev);
 
            splice(prev);
            deleteNode(prev);
        }else{
            Node* curr=prev->list_p[1];
            while(curr->list_p[0]){
                curr=curr->list_p[0];
            }
            prev->info=curr->info;
            Node* par=prev->list_p[2];
            if(par)
                updateRange(par, prev);
            
            Node* node=curr->list_p[2];
            if(node)
                updateRangeAfterDeletion(node, curr);
            
            splice(curr);
            deleteNode(curr);
        } 
    }
}

template<typename T>
bool BSTfinger<T>::find(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        return true;
    }
    return false;
}

template<typename T>
void BSTfinger<T>::searchFinger(const T &t){
    while(!finger.isEmpty() ){
        Node* node=finger.top();
        if(node->range[0]<t && node->range[1]>t){
            return;
        }else{
            finger.pop();
        }
    }
}

template<typename T>
void BSTfinger<T>::updateRange(Node *prev, Node *node){
    bool stop{false};
    while (prev && !stop) {
        int i=prev->list_p[0]==node? 0:1;
        T oldV=prev->range[i];
        if(!i){
            prev->range[i]=std::min(prev->info, node->range[0]);
        }else{
            prev->range[i]=std::max(prev->info, node->range[1]);
        }
        if(oldV==prev->range[i]){
            stop=true;
        }else{
            node=prev;
            prev=prev->list_p[2];
        }
    }
}

template<typename T>
void BSTfinger<T>::add(const T &t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        if(prev){
            updateRange(prev, node);
        }
        return;
    }
    deleteNode(node);
}

template<typename T>
T BSTfinger<T>::rangeOfNode(Node *node, const int r) const{
    T res=node->info;
    while(node->list_p[r]){
        node=node->list_p[r];
        res=node->info;
    }
    return res;
}

#endif /* BSTfinger_h */
