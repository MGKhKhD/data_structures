//
//  Chapter4.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-11.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Chapter4_h
#define Chapter4_h

#include <iostream>
#include <string>

#include "SkipSet.hpp"
#include "SkipList.hpp"
#include "Ex4_8.hpp"
#include "Ex4_9.hpp"
#include "Ex4_10.hpp"
#include "Ex4_11.hpp"
#include "Ex4_12.hpp"


void testEx4_12(){
    Ex4_12<double> sl;
    for(int i=0; i<17; ++i){
        sl.add(i*2-5,i);
    }
    std::cout<<sl;
//    while (!sl.isEmpty()) {
//        std::cout<<sl.remove(0)<<std::setw(6);
//    }
//    std::cout<<"\n";
    
    Ex4_12<double> c;
    c.add(210, 0);
    c.add(220, 1);
    c.add(230, 2);
    sl.absorb(c);
    std::cout<<c;
    std::cout<<sl;
    
    
    sl.remove(0);
    sl.remove(10);
    sl.remove(14);
    sl.add(120, 14);
    std::cout<<sl;
}

void testEx4_11(){
    Ex4_11<double> sl;
    for(int i=0; i<17; ++i){
        sl.add(i*2-5,i);
    }
    std::cout<<sl;
    Ex4_11<double> c{sl.truncate(10)};
    std::cout<<sl;
    std::cout<<c;

}

void testSkipList(){
    SkipList<double> sl;
    
    for(int i=0; i<17; ++i){
        sl.add(i*2-5,i);
    }
    std::cout<<sl;
    SkipList<double> c;
    c=sl;
    
    std::cout<<sl.get(0)<<std::setw(5)<<sl.get(5)<<std::setw(5)<<sl.get(10)<<"\n";
    sl.set(0, 50);
    sl.set(10, 100);
    std::cout<<sl;
    sl.remove(0);
    sl.remove(9);
    std::cout<<sl;
    
    std::cout<<c;
}

void testEx4_10(){
    Ex4_10<double> ss;
}

void testEx4_8(){
    Ex4_8<double> ss;
    ss.add(20);
    ss.add(2);
    ss.add(17);
    ss.add(44);
    ss.add(40);
    ss.add(-2);
    ss.add(-2);
    ss.add(17);
    
    Ex4_8<double> ss1;
    ss1=ss;
    Ex4_8<double> c;
    c=std::move(ss1);
    c.add(111);
    
    std::cout<<ss;
//    ss.remove(17);
//    std::cout<<ss;
//    ss.add(222);
//    std::cout<<ss;
//    if(ss.find(44))
//        std::cout<<"\nfound\n";
    ss.update(17, 127.5);
    std::cout<<ss;
    std::cout<<c;
    
}


SkipSet<double> produceSkipSet(){
    SkipSet<double> ss;
    ss.add(20);
    ss.add(2);
    ss.add(17);
    ss.add(44);
    ss.add(40);
    ss.add(-2);
    ss.add(-2);
    ss.add(17);
    
    return ss;
}

Ex4_9<double> produceEx4_9(){
    Ex4_9<double> ss;
    ss.add(20);
    ss.add(2);
    ss.add(17);
    ss.add(44);
    ss.add(40);
    ss.add(-2);
    ss.add(-2);
    ss.add(17);
    
    return ss;
}

void testEx4_9(){
    Ex4_9<double> ss;
    
    ss.add(20);
    ss.add(2);
    ss.add(17);
    ss.add(44);
    ss.add(40);
    ss.add(-2);
    ss.add(-2);
    ss.add(17);
    
//    Ex4_9<double> c{ss};
    
//    Ex4_9<double> c;
//    c=ss;
    
    Ex4_9<double> c{produceEx4_9()};
//    Ex4_9<double> c;
//    c=produceEx4_9();
    c.add(300);
    c.add(111);
    c.removeItem(-2);
    c.add(-2222);
    
    ss.printList();
    std::cout<<ss.get(5)<<std::endl;
    ss.add(22);
    ss.add(80);
    ss.printList();
    std::cout<<ss.get(5)<<std::endl;
    
    ss.removeItem(17);
    ss.printList();
    std::cout<<ss.get(5)<<std::endl;
    ss.removeIndex(5);
    ss.printList();
    ss.set(4, 50);
    ss.printList();
    std::cout<<ss.get(5)<<std::endl;
    c.printList();
    
//    if(ss.find(220)){
//        std::cout<<"\nfound\n";
//    }
}

void testSkipSete(){
    
    SkipSet<double> ss;
    ss.add(20);
    ss.add(2);
    ss.add(17);
    ss.add(44);
    ss.add(40);
    ss.add(-2);
    ss.add(-2);
    ss.add(17);
    ss.printList();
    
    SkipSet<double> c{produceSkipSet()};
    c.add(222);


    ss.printList();
    ss.remove(17);
    ss.add(18.1);
    ss.printList();

    c.printList();
    
//    if(ss.find(40)){
//        std::cout<<"\nfound\n";
//    }
}


#endif /* Chapter4_h */
