//
//  Chapter7.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-14.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Chapter7_h
#define Chapter7_h

#include "Treap.hpp"
#include "Treap_A.hpp"
#include "Ex7_7.hpp"
#include "Ex7_9.hpp"
#include "Ex7_10v1.hpp"
#include "Ex7_10v2.hpp"
#include "Ex7_12.hpp"
#include "Ex7_13.hpp"

void testEx7_13(){
    Ex7_13<double> t;
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    t.add(17);
    t.add(1);
    t.add(3.6);
    t.add(16);
    t.add(10.55);
    
//    Ex7_13<double> c=t;
//    c.inorderTraversal();
    
    t.inorderTraversal();
    
    Ex7_13<double> t1;
    t1.add(-10.1);
    t1.add(-10.2);
    t1.add(-8.8);
    t1.inorderTraversal();
    t.absorb(t1);
    t.inorderTraversal();
//    t1.inorderTraversal();
    
    Ex7_13<double> t2;
    t2.add(30.1);
    t2.add(30.2);
    t2.add(38.8);
    t2.inorderTraversal();
    t.absorb(t2);
    t.inorderTraversal();
    
    Ex7_13<double> t3;
    t3.add(3.1);
    t3.add(23.2);
    t3.add(10.8);
    t3.inorderTraversal();
    t.absorb(t3);
    t.inorderTraversal();
    
    t1.inorderTraversal();
    t2.inorderTraversal();
    t3.inorderTraversal();
    
//    t.remove(10);
//    t.inorderTraversal();
}

void testEx7_10(){
//    Ex7_10v1<double> t;
    Ex7_10v2<double> t;
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    t.add(17);
    t.add(1);
    t.add(3.6);
    t.add(16);
    t.add(10.55);
    
    t.inorderTraversal();
    
    t.removeLE(17);
//    t.add(-5);
    t.inorderTraversal();
    
//    t.remove(1);
//    t.inorderTraversal();
    
//    double ret=t.getLE(13);
//    if(ret){
//        std::cout<<ret<<std::setw(6);
//    }
//    std::cout<<"\n";
    
//    for(int i=0; i<t.getLen(); ++i){
//        double ret=t.getLE(i);
//        if(ret){
//            std::cout<<ret<<std::setw(6);
//        }
//
//    }
//    std::cout<<"\n";
}

void testEx7_12(){
    Ex7_12<double> t;
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    t.add(17);
    t.add(1);
    t.add(3.6);
    t.add(16);
    t.add(10.55);
    
    t.inorderTraversal();
    Ex7_12<double> tr{t.splite(10)};
    
    t.inorderTraversal();
    tr.inorderTraversal();

}

void testEx7_9(){
    Ex7_9<double> t;
    
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    t.add(17);
    t.add(1);
    t.add(3.6);
    t.add(16);
    t.add(10.55);
    
    t.inorderTraversal();
    t.breathFirstPrint();
    
    t.remove(3);
    t.inorderTraversal();
    t.breathFirstPrint();
    
}

void testEx7_7(){
    Ex7_7<double> t;
    
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    
    t.inorderTraversal();
    t.remove(11);
    t.inorderTraversal();
}

void testTreap_A(){
    Treap_A<double> t;
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    
    t.inorderTraversal();
    t.breathFirstPrint();
    
    t.remove(11);
    t.inorderTraversal();
    t.breathFirstPrint();
}

Treap<double> produceTreap(){
    Treap<double> t;
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    
    return t;
}


void testTreap(){
    Treap<double> t;
    t.add(10);
    t.add(11);
    t.add(8);
    t.add(5);
    t.add(29);
    t.add(3);
    t.add(5.5);
    t.add(8.77);
    t.add(7);
    
    
    
    Treap<double> c;
    c=produceTreap();
    c.add(11.79);
    c.add(8.771);
    t.add(24);
    
    t.inorderTraversal();
    t.breathFirstPrint();
    
    c.inorderTraversal();
    c.breathFirstPrint();
    
    
//    t.remove(29);
//    t.inorderTraversal();
//    t.breathFirstPrint();
}


#endif /* Chapter7_h */
