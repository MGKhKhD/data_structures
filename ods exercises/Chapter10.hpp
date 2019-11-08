//
//  Chapter10.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-12.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Chapter10_h
#define Chapter10_h

#include "BinaryHeap.hpp"
#include "Algorithms.hpp"

void testBianryHeap(){
    ArrayImpl<double> a{produceArray(20)};
    BinaryHeap<double> bh{a};
    
    bh.printHeap();
    
}

#endif /* Chapter10_h */
