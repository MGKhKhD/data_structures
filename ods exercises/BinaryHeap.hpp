//
//  BinaryHeap.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-12.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef BinaryHeap_h
#define BinaryHeap_h

#include <iostream>
#include <iomanip>

#include "ArrayImpl.hpp"

template<typename T>
class BinaryHeap{
    ArrayImpl<T> a;
    size_t n;
    
public:
    BinaryHeap(const ArrayImpl<T>& arr):a{arr}, n{arr.len} {}
    ~BinaryHeap(){
    }
    
    void printHeap() const;
};

template<typename T>
void BinaryHeap<T>::printHeap() const{
    const size_t perLine{20};
    size_t num{0};
    for(int i=0; i<n; ++i){
        std::cout<<a[i]<<std::setw(5);
        if(!(++num % perLine)) std::cout<<std::endl;
    }
    std::cout<<std::endl;
}


#endif /* BinaryHeap_h */
