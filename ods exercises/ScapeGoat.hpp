//
//  ScapeGoat.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-20.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef ScapeGoat_h
#define ScapeGoat_h

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include "Stack.hpp"
#include "Queue.hpp"

template<typename T>
class ScapeGoat{
    struct Node{
        T info;
        Node** list_p;
        int size;
        int depth;
        
        explicit Node(const T& t): info{t}, list_p{new Node*[3]}, size{1}, depth{0} {
            for(int i=0; i<3; ++i)
                list_p[i]=nullptr;
        }
        Node(const T& t, const int s, const int d): info{t}, list_p{new Node*[3]}, size{s}, depth{d} {
            for(int i=0; i<3; ++i)
                list_p[i]=nullptr;
        }
    };
    Node* root;
    int n;
    int q;
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->list_p;
//            for(int i=0; i<3; ++i)
//                node->list_p[i]=nullptr;
            delete node;
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
                curr=curr->list_p[curr->info>t? 0:1];
            }
        }
        return prev;
    }
    
    static int log32(const int qv){
        static double log23=2.4663034623764317;
        return static_cast<int>(std::ceil(std::log(qv)*log23));
    }
    
    bool addChild(Node* prev, Node* node);
    void destroyTree(Node* node);
    void updateSize(Node* node, bool decrease);
    void updateDepth(Node* node);
    const int getSize(Node* node) const;
    const int getDepth(Node* node) const;
    void rebuildSubTree(Node* node);
    int packToArray(Node* node, Node** arr, int i);
    Node* buildBalanced(Node** node, int i, int ns, int parDepth);
    void splice(Node* node);
    void copyTree(Node* &node, Node* otherNode);
    void swap(ScapeGoat& other) noexcept;
    
public:
    ScapeGoat(): root{nullptr}, n{0}, q{0} {}
    ScapeGoat(const ScapeGoat<T>& sg);
    ScapeGoat& operator=(const ScapeGoat& sg);
    ~ScapeGoat(){
        if(root) destroyTree(root);
        q=0;
    }
    
    void add(const T& t);
    bool find(const T& t) const;
    void remove(const T& t);
    
    void inorderTraversal() const;
    void bfs() const;
};

template<typename T>
void ScapeGoat<T>::swap(ScapeGoat<T> &other) noexcept{
    std::swap(root, other.root);
    std::swap(n,other.n);
    std::swap(q, other.q);
}

template<typename T>
ScapeGoat<T>& ScapeGoat<T>::operator=(const ScapeGoat<T> &sg){
    if(this!=&sg){
        ScapeGoat<T> c{sg};
        swap(c);
    }
    return *this;
}

template<typename T>
ScapeGoat<T>::ScapeGoat(const ScapeGoat<T>& sg): ScapeGoat(){
    if(sg.root){
        copyTree(root, sg.root);
    }else{
        root=nullptr;
    }
}

template<typename T>
void ScapeGoat<T>::copyTree(Node *&node, Node *otherNode){
    if(otherNode){
        node=new Node(otherNode->info, otherNode->size, otherNode->depth);
        for(int r=0; r<2; ++r){
            copyTree(node->list_p[r], otherNode->list_p[r]);
            if(node->list_p[r])
                node->list_p[r]->list_p[2]=node;
        }
        n++;
        q++;
    }else{
        node=nullptr;
    }
}

template<typename T>
void ScapeGoat<T>::updateDepth(Node *node){
    Stack<Node*> s;
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[0];
        }else{
            node=s.top();
            s.pop();
            node->depth--;
            node=node->list_p[1];
        }
    }
}

template<typename T>
void ScapeGoat<T>::splice(Node *node){
    Node* succ=node->list_p[node->list_p[0]? 0: 1];
    Node* par=nullptr;
    if(node==root){
        root=succ;
    }else{
        par=node->list_p[2];
        par->list_p[node==par->list_p[0]? 0:1]=succ;
    }
    if(succ)
    {
        succ->list_p[2]=par;
        updateDepth(succ);
    }
}

template<typename T>
void ScapeGoat<T>::remove(const T &t){
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t){
        Node* par=nullptr;
        if(!prev->list_p[0] || !prev->list_p[1]){
            par=prev->list_p[2];
            splice(prev);
            deleteNode(prev);
        }else{
            Node* curr=prev->list_p[1];
            while (curr->list_p[0]) {
                curr=curr->list_p[0];
            }
            prev->info=curr->info;
            par=curr->list_p[2];
            splice(curr);
            deleteNode(curr);
        }
        if(par) updateSize(par, true);
        n--;
        if(2*n < q){
            rebuildSubTree(root);
            q=n;
        }
    }
}

template<typename T>
bool ScapeGoat<T>::find(const T &t) const{
    Node* prev=findPrevNode(t);
    if(prev && prev->info==t)
        return true;
    return false;
}

template<typename T>
typename ScapeGoat<T>::Node* ScapeGoat<T>::buildBalanced(Node **arr, int i, int ns, int parDepth){
    if(ns==0) return nullptr;
    int m=ns/2;
    arr[i+m]->size=1;
    arr[i+m]->depth=parDepth+1;
    arr[i+m]->list_p[0]=buildBalanced(arr, i, m, arr[i+m]->depth);
    arr[i+m]->list_p[1]=buildBalanced(arr, i+m+1, ns-m-1, arr[i+m]->depth);
    for(int r=0; r<2; ++r){
        if(arr[i+m]->list_p[r]){
            arr[i+m]->list_p[r]->list_p[2]=arr[i+m];
            arr[i+m]->size+=arr[i+m]->list_p[r]->size;
        }
    }
    return arr[i+m];
}

template<typename T>
void ScapeGoat<T>::rebuildSubTree(Node *node){
    int ns=node->size;
    Node** arr=new Node*[ns];
    packToArray(node, arr, 0);
    Node* par=node->list_p[2];
    if(!par){
        root=buildBalanced(arr, 0, ns, 0);
        root->list_p[2]=nullptr;
    }else{
        int i=par->list_p[0]==node? 0:1;
        par->list_p[i]=buildBalanced(arr, 0, ns, par->depth);
        par->list_p[i]->list_p[2]=par;
    }
    
    delete [] arr;
}

template<typename T>
int ScapeGoat<T>::packToArray(Node *node, Node **arr, int i){
    if(!node) return i;
    i=packToArray(node->list_p[0], arr, i);
    arr[i++]=node;
    return packToArray(node->list_p[1], arr, i);
}

template<typename T>
void ScapeGoat<T>::add(const T &t){
    Node* node=new Node(t);
    Node* prev=findPrevNode(t);
    if(addChild(prev, node)){
        if(node->depth >0 && node->depth >= log32(q)){
            Node* par=prev->list_p[2];
            while (3*prev->size <=2*par->size) {
                prev=par;
                par=prev->list_p[2];
            }
            rebuildSubTree(par);
        }
        return;
    }
    deleteNode(node);
}

template<typename T>
void ScapeGoat<T>::updateSize(Node *node, bool decrease){
    while(node){
        node->size=decrease? node->size-1: node->size+1;
        node=node->list_p[2];
    }
}

template<typename T>
bool ScapeGoat<T>::addChild(Node *prev, Node *node){
    if(prev && prev->info==node->info) return false;
    if(!prev){
        root=node;
    }else{
        prev->list_p[prev->info>node->info? 0:1]=node;
        node->list_p[2]=prev;
        node->depth=prev->depth+1;
        updateSize(prev, false);
    }
    n++;
    q++;
    return true;
}

template<typename T>
void ScapeGoat<T>::destroyTree(Node *node){
    if(node){
        destroyTree(node->list_p[0]);
        destroyTree(node->list_p[1]);
        deleteNode(node);
        n--;
    }
}

template<typename T>
const int ScapeGoat<T>::getSize(Node *node) const{
    Stack<Node*> s;
    int res=0;
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[0];
        }else{
            node=s.top();
            s.pop();
            res++;
            node=node->list_p[1];
        }
    }
    return res;
}

template<typename T>
const int ScapeGoat<T>::getDepth(Node *node) const{
    int d=0;
    while(node!=root){
        node=node->list_p[2];
        d++;
    }
    return d;
}

template<typename T>
void ScapeGoat<T>::inorderTraversal() const{
    Node* node=root;
    Stack<Node*> s;
    while (node || !s.isEmpty()) {
        if(node){
            s.push(node);
            node=node->list_p[0];
        }else{
            node=s.top();
            s.pop();
            std::cout<<"("<<node->info<<","<<node->size-getSize(node)<<","<<node->depth-getDepth(node)<<")"<<std::setw(6);
            node=node->list_p[1];
        }
    }
    std::cout<<"\n";
}

template<typename T>
void ScapeGoat<T>::bfs() const{
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

#endif /* ScapeGoat_h */
