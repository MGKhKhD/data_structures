//
//  Ex7_7.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-15.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex7_7_h
#define Ex7_7_h

#include <algorithm>
#include <iostream>
#include <iomanip>

template<typename T>
class Ex7_7{
    struct Node{
        T info;
        int p;
        int size;
        int height;
        int depth;
        Node** list_p;//list_p[0]=left, list_p[1]=right, list_p[2]=parent
        
        explicit Node(const T& t):info{t}, p{std::rand()}, size{1}, height{0}, depth{0}, list_p{new Node*[3]} {
            for(int i=0; i<3; ++i)
                list_p[i]=nullptr;
        }
    };
    Node* root;
    
    const int getSizeNode(Node* node) const{
        if(!node) return 0;
        return 1+getSizeNode(node->list_p[0])+getSizeNode(node->list_p[1]);
    }
    
    const int getHeightNode(Node* node) const{
        if(!node) return -1;
        return 1+std::max(getHeightNode(node->list_p[1]), getHeightNode(node->list_p[0]));
    }
    
    const int getDepthNode(Node* node) const{
        int d=0;
        while(node!=root){
            d++;
            node=node->list_p[2];
        }
        return d;
    }
    
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
            prev->list_p[prev->info > node->info? 0:1]=node;
            node->list_p[2]=prev;
        }
        return true;
    }
    
    void destroyTree(Node* node){
        if(node){
            destroyTree(node->list_p[0]);
            destroyTree(node->list_p[1]);
            delete [] node->list_p;
            delete node;
            node=nullptr;
        }
    }
    
    void inorder(Node* node) const;
    
    void updateParamsAfterAddition(Node* prev, Node* node);
    void updateParamsAfterDeletion(Node* prev);
    void updateDepth(Node* node, bool increase);
    void updateHeight(Node* node);
    void rotate(Node* node, int r);//r=1 rotate left; r=0 rotate right
    void bubbleUp(Node* node);
    void splice(Node* node);
    void trickleDown(Node* node);
    
public:
    Ex7_7(): root{nullptr} {}
    ~Ex7_7(){
        if(root) destroyTree(root);
    }
    
    void add(const T& t);
    void remove(const T& t);
    void inorderTraversal() const{
        inorder(root);
        std::cout<<"\n";
    }
};

template<typename T>
void Ex7_7<T>::remove(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        trickleDown(prev);
        splice(prev);
        Node* node=prev->list_p[2];
        delete [] prev->list_p;
        delete prev;
        prev=nullptr;
        
        if(node)
            updateParamsAfterDeletion(node);
    }
}

template<typename T>
void Ex7_7<T>::splice(Node *node){
    Node* succ=node->list_p[node->list_p[0]!=nullptr? 0:1];
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
void Ex7_7<T>::trickleDown(Node *node){
    while(node->list_p[0] || node->list_p[1]){
        if(!node->list_p[0]){
            rotate(node, 1);
        }else if(!node->list_p[1]){
            rotate(node, 0);
        }else{
            rotate(node, node->list_p[0]->p < node->list_p[1]->p? 0 : 1);
        }
        if(node==root){
            root=node->list_p[2];
        }
    }
}

template<typename T>
void Ex7_7<T>::add(const T &t){
    Node* prev=findPrevNode(t);
    Node* node=new Node(t);
    if(addChild(prev, node)){
        if(prev){
            updateParamsAfterAddition(prev, node);
        }
        bubbleUp(node);
        return;
    }
    delete [] node->list_p;
    delete node;
    node=nullptr;
}

template<typename T>
void Ex7_7<T>::updateParamsAfterAddition(Node *prev, Node *node){
    node->depth=prev->depth+1;
    
    bool increaseHeight{true};
    while(prev){
        prev->size++;
        if(increaseHeight){
            int oldH=prev->height;
            updateHeight(prev);
            if(prev->height==oldH)
                increaseHeight=false;
        }
        
        prev=prev->list_p[2];
    }
}

template<typename T>
void Ex7_7<T>::updateParamsAfterDeletion(Node *prev){
    
    bool reduceHeight{true};
    while (prev) {
        prev->size--;
        if(reduceHeight){
            int oldH=prev->height;
            updateHeight(prev);
            if(oldH == prev->height)
                reduceHeight=false;
        }
        
        prev=prev->list_p[2];
    }
}

template<typename T>
void Ex7_7<T>::updateHeight(Node *prev){
    if(!prev->list_p[0] && !prev->list_p[1]){
        prev->height=0;
    }
    else if(prev->list_p[0] && !prev->list_p[1]){
        prev->height=1+prev->list_p[0]->height;
    }else if(prev->list_p[1] && !prev->list_p[0]){
        prev->height=1+prev->list_p[1]->height;
    }else{
        prev->height=1+std::max(prev->list_p[0]->height, prev->list_p[1]->height);
    }
}

template<typename T>
void Ex7_7<T>::bubbleUp(Node *node){
    while(node->list_p[2] && node->list_p[2]->p > node->p){
        rotate(node->list_p[2], node->list_p[2]->list_p[0]==node? 0:1);
    }
    if(!node->list_p[2])
        root=node;
}

template<typename T>
void Ex7_7<T>::rotate(Node *node, int r){
    assert(r==0 || r==1);//r=0 rotate right; r=1 rotate left
    Node* w=node->list_p[r];
    w->list_p[2]=node->list_p[2];
    if(node->list_p[2])
        w->list_p[2]->list_p[node->list_p[2]->list_p[0]==node? 0: 1]=w;
    
    int r_hat=(r+1)%2;//sibling of r
    
    node->depth++;
    w->depth--;
    if(w->list_p[r]){
        updateDepth(w->list_p[r], false);//reduce depth
    }
    if(node->list_p[r_hat])
        updateDepth(node->list_p[r_hat], true);//increase depth
    
    node->list_p[r]=w->list_p[r_hat];
    node->size-=w->size;
   
    if(node->list_p[r]){
        node->list_p[r]->list_p[2]=node;
        node->size+=node->list_p[r]->size;
        w->size-=node->list_p[r]->size;
    }
    node->list_p[2]=w;
    w->list_p[r_hat]=node;
    
    w->size+=node->size;
    updateHeight(node);
    updateHeight(w);
    
    if(node==root){
        root=w;
        w->list_p[2]=nullptr;
    }
}

template<typename T>
void Ex7_7<T>::updateDepth(Node *node, bool increase){
    if(node){
        updateDepth(node->list_p[0], increase);
        updateDepth(node->list_p[1], increase);
        if(increase){
            node->depth++;
        }else{
            node->depth--;
        }
    }
}

template<typename T>
void Ex7_7<T>::inorder(Node *node) const {
    if(node){
        int pri=0;
        if(node->list_p[2])
            pri=node->list_p[2]->p;
        char c=node->p - pri>0? '+':'-';
        inorder(node->list_p[0]);
        std::cout<<"("<<node->info<<","<<node->size-getSizeNode(node)<<","<<node->height-getHeightNode(node)<<","<<node->depth-getDepthNode(node)<<","<<c<<")"<<std::setw(5);
        inorder(node->list_p[1]);
    }
}

#endif /* Ex7_7_h */
