//
//  BinaryTree.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-09-01.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef BinaryTree_h
#define BinaryTree_h

#include <array>
#include <iostream>
#include <iomanip>

template<typename N>
class BTNode{
public:
    std::array<N*, 3> list_p;
    
    BTNode(){
        for(int i=0; i<3; list_p[i]=nullptr, ++i);
    }
};

template<typename Node>
class BinaryTree{
protected:
    Node* root;
    
    virtual const size_t heightNode(Node* node) const;
    virtual const size_t sizeNode(Node* node) const;
    virtual const size_t depthNode(Node* node) const;
    
public:
    BinaryTree(): root{nullptr} {}
    virtual ~BinaryTree(){
        destroyTree();
    }
    
    virtual const size_t size() const;
    virtual const size_t height() const;
    virtual const size_t depth() const;
    virtual void destroyTree();
    virtual const size_t sizeNonRecursive() const;
    virtual const size_t sizeWithStack() const;
    virtual void inorderTraversal() const;
    virtual void bfs() const;
};

class BTNode1: public BTNode<BTNode1> {};

template<typename Node>
const size_t BinaryTree<Node>::size() const{
    return sizeNode(root);
}

template<typename Node>
const size_t BinaryTree<Node>::sizeNonRecursive() const{
    if(!root) return 0;
    Node* prev=nullptr;
    Node* next=nullptr;
    Node* curr=root;
    while (curr) {
        if(prev==curr->list_p[2]){
            if(curr->list_p[0]){
                next=curr->list_p[0];
            }else if(curr->list_p[1]){
                next=curr->list_p[1];
            }else{
                next=curr->list_p[2];
            }
        }
    }
}

#endif /* BinaryTree_h */
