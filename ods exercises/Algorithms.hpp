//
//  Algorithms.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-12.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Algorithms_h
#define Algorithms_h
#include <iostream>
#include <iomanip>
#include "ArrayImpl.hpp"

ArrayImpl<double> produceArray(const size_t num){
    ArrayImpl<double> a{num};
    for(int i=0; i<num; ++i)
    {
        a[i]=(i%2==0)? i : -i;
    }
    return a;
}

void testArray(){
    ArrayImpl<double> a{10};
    for(int i=0; i<10; ++i)
        a[i]=2*i+1;
    
    ArrayImpl<double> aa{a.len};
    aa=produceArray(a.len);
//    aa.reverse();
//    aa.fill(-1);
//    ArrayImpl<double> t{aa.copyOfRange(4, 7)};
    
    for(int i=0; i<aa.len; ++i)
        std::cout<<aa[i]<<std::setw(5);
}

int swap_first_last_digits(int n){
    if(n<0) return -swap_first_last_digits(-n);
    if(n<10) return n;
    constexpr int max_digits=std::numeric_limits<int>::digits10+1;
    uint8_t digits[max_digits];
    uint8_t* p=digits;
    while (n) {
        *(p++)=n%10;
        n=n/10;
    }
    std::swap(digits[0], *(p-1));
    while (p!=digits) {
        n*=10;
        n+=*(--p);
    }
    return n;
}


#endif /* Algorithms_h */
