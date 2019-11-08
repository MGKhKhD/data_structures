//
//  Ex4_8.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-12.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Ex4_8_h
#define Ex4_8_h

#include <stdexcept>
#include <cmath>
#include <iostream>

template<typename T>
class Ex4_8{
    struct Node{
        T info;
        Node** next;
        int height;
    };
    Node* root;
    int n;
    int h;
    Node** list;
    
    Node* newNode(const T& t, const int hv){
        Node* node=new Node;
        node->info=t;
        node->height=hv;
        node->next=new Node*[hv+1];
        return node;
    }
    
    int setHeight(){
        int z=std::rand();
        int k=0;
        int m=1;
        while((m & z)!=0){
            k++;
            m<<=1;
        }
        return k;
    }
    
    void deleteNode(Node* node){
        if(node){
            delete [] node->next;
            delete node;
            node=nullptr;
        }
    }
    
    void destroySet(){
        Node* curr=root->next[0];
        while(curr){
            Node* tmp=curr->next[0];
            deleteNode(curr);
            curr=tmp;
            n--;
        }
        delete [] root->next;
        memset(root->next, '\0', sizeof(Node*)*(root->height+1));
        delete [] list;
        delete root;
        root=nullptr;
        h=0;
    }
    
    Node* findNode(const T& t) const {
        Node* node=root;
        int r=h;
        Node* prev=nullptr;
        while(r>=0){
            while(node->next[r] && (!prev || (prev && prev!=node)) && node->next[r]->info<t){
                prev=node;
                node=node->next[r];
            }
            r--;
        }
        return node;
    }
    
    void swap(Ex4_8<T>& other) noexcept{
        std::swap(root, other.root);
        std::swap(n, other.n);
        std::swap(h, other.h);
    }
    
public:
    Ex4_8(): root{newNode(static_cast<T>(NULL), sizeof(int)*8)}, n{0}, h{0}{
        memset(root->next, '\0', sizeof(Node*)*(root->height+1));
        list=new Node*[root->height+1];
    }
    
    Ex4_8(const Ex4_8<T>& ss): Ex4_8(){
        for(Node* node=ss.root->next[0]; node; add(node->info), node=node->next[0]);
    }
    
    Ex4_8 operator=(const Ex4_8<T>& s){
        if(this!=&s){
            Ex4_8<T> c{s};
            swap(c);
        }
        return *this;
    }
    
    Ex4_8(Ex4_8<T>&& ss): root{ss.root}, h{ss.h}, n{ss.n}, list{ss.list} {
        ss.root=nullptr;
        ss.list=nullptr;
    }
    
    //not working corrctly
    Ex4_8<T>& operator=(Ex4_8<T>&& ss){
        Ex4_8<T> c{std::move(ss)};
        swap(c);
        return *this;
    }
    
    ~Ex4_8(){
        if(root){
            destroySet();
        }
    }
    
    void add(const T& t);
    bool find(const T& t) const;
    void remove(const T& t);
    void update(const T& oldVal, const T& newVal);
    

    friend std::ostream& operator<<(std::ostream & out, const Ex4_8<T>& ss){
        const size_t perLine{20};
        size_t num=0;
        
        for(typename Ex4_8<T>::Node* node=ss.root->next[0]; node; node=node->next[0]){
            out<<node->info<<std::setw(5);
            if(!(++num % perLine)) out<<"\n";
        }
        out<<"\n";
        return out;
    }
};

template<typename T>
void Ex4_8<T>::update(const T &oldVal, const T &newVal){
    Node* curr=findNode(oldVal);

    if(curr->next[0] && curr->next[0]->info==oldVal){
        if((curr!=root && curr->info< newVal) && (!curr->next[0]->next[0] || ( curr->next[0]->next[0] && curr->next[0]->next[0]->info>newVal)))
            {
                curr->next[0]->info=newVal;
                
            }else{
                remove(oldVal);
                add(newVal);
            }
    }

}

template<typename T>
void Ex4_8<T>::add(const T &t){
    Node* curr=root;
    int r=h;
    Node* prev=nullptr;
    while(r>=0){
        while (curr->next[r] && (!prev || (prev && prev!=curr)) && curr->next[r]->info<t) {
            prev=curr;
            curr=curr->next[r];
        }
        if(curr->next[r] && curr->next[r]->info==t) return;
        list[r--]=curr;
    }
    
    Node* node=newNode(t, setHeight());
    while(node->height>=h)
        list[++h]=root;
    
    for(int i=0; i<=node->height; ++i){
        node->next[i]=list[i]->next[i];
        list[i]->next[i]=node;
    }
    n++;
}

template<typename T>
bool Ex4_8<T>::find(const T &t) const{
    Node* node=findNode(t);
    
    if(node->next[0] && node->next[0]->info==t) return true;
    return false;
}

template<typename T>
void Ex4_8<T>::remove(const T &t){
    Node* curr=root;
    Node* del=nullptr;
    Node* prev=nullptr;
    bool found{false};
    int r=h;
    while(r>=0){
        while(curr->next[r] && (!prev || (prev && prev!=curr)) && curr->next[r]->info<t){
            prev=curr;
            curr=curr->next[r];
        }
        if(curr->next[r] && curr->next[r]->info==t){
            found=true;
            del=curr->next[r];
            curr->next[r]=del->next[r];
            if(curr==root && !curr->next[r])
                h--;
        }
        r--;
    }
    if(found){
        deleteNode(del);
        n--;
    }
}

#endif /* Ex4_8_h */
