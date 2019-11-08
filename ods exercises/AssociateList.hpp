//
//  AssociateList.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-25.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef AssociateList_h
#define AssociateList_h

#include "Dlink.hpp"
#include "ArrayImpl.hpp"

template<size_t len>
class AssociateList{
private:
    ArrayImpl<Dlink<size_t>> adjList{len};
    size_t n;
    
public:
    AssociateList(): n{len} {}
    AssociateList(const AssociateList& al): AssociateList(){
        for(size_t i=0; i<al.adjList.len; ++i){
            adjList[i]=al.adjList[i];
        }
        n=al.n;
    }
    ~AssociateList(){}
    
    size_t getSize() const{return n;}
    
    void addEdge(const size_t i, const size_t j){
        assert(i<n &&  j<n);
        if(!adjList[i].find(j))
            adjList[i].addFront(j);
    }
    void removeEdge(const size_t i, const size_t j){
        assert(i<n && j<n);
        adjList[i].remove(j);
    }
    
    Dlink<size_t> outEdges(const size_t i) const{
        assert(i>=0 && i<n);
        return adjList[i];
    }
    
    Dlink<size_t> inEdges(const size_t i) const{
        assert(i<n);
        Dlink<size_t> out;
        for(size_t j=0; j<adjList.len; ++j){
            if(adjList[j].find(i)) out.addBack(j);
        }
        return out;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const AssociateList& al){
        for(size_t i=0; i<al.getSize(); ++i){
            out<<"adj["<<i<<"]: "<<al.adjList[i];
        }
        return out;
    }
};



#endif /* AssociateList_h */
