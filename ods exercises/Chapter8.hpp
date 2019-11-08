//
//  Chapter8.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-20.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Chapter8_h
#define Chapter8_h

#include "ScapeGoat.hpp"
#include "SGfinger.hpp"
#include "Ex8_8.hpp"
#include "Ex8_9.hpp"
#include "Ex8_10.hpp"

void testEx8_9(){
    Ex8_9<double,6> sg;
    sg.add(7);
    sg.add(6);
    sg.add(8);
    sg.add(5);
    sg.add(9);
    sg.add(2);
    sg.add(1);
    sg.add(4);
    sg.add(0);
    sg.add(3);
    sg.add(3.5);
    sg.add(3);
    sg.add(4);
    
    sg.inorderTraversal();
    sg.bfs();
    
    sg.remove(7);
    sg.inorderTraversal();
    sg.bfs();
}

void testEx8_10(){
    Ex8_10<double> sg;
    sg.add(7);
    sg.add(6);
    sg.add(8);
    sg.add(5);
    sg.add(9);
    sg.add(2);
    sg.add(1);
    sg.add(4);
    sg.add(0);
    sg.add(3);
    sg.add(3.5);
    sg.add(3);
    sg.add(4);
    
    sg.inorderTraversal();
    sg.bfs();
    
    sg.remove(7);
    sg.inorderTraversal();
    sg.bfs();
    
}

void testSGfinger(){
    SGfinger<double> sg;
    sg.add(7);
    sg.add(6);
    sg.add(8);
    sg.add(5);
    sg.add(9);
    sg.add(2);
    sg.add(1);
    sg.add(4);
    sg.add(0);
    sg.add(3);
    sg.add(3.5);
    sg.add(3);
    sg.add(11);
    
    sg.inorderTraversal();
    sg.bfs();
//    if(sg.find(40)){
//        std::cout<<"\nfound\n";
//    }
    sg.remove(3);
    sg.inorderTraversal();
    sg.bfs();
}

void testEx8_8(){
    Ex8_8<double> sg;
    sg.add(7);
    sg.add(6);
    sg.add(8);
    sg.add(5);
    sg.add(9);
    sg.add(2);
    sg.add(1);
    sg.add(4);
    sg.add(0);
    sg.add(3);
    sg.add(3.5);
    
    sg.inorderTraversal();
    sg.bfs();
    
    sg.remove(3.5);
    sg.inorderTraversal();
    sg.bfs();
}


void testScapteGoat(){
    ScapeGoat<double> sg;
    sg.add(7);
    sg.add(6);
    sg.add(8);
    sg.add(5);
    sg.add(9);
    sg.add(2);
    sg.add(1);
    sg.add(4);
    sg.add(0);
    sg.add(3);
    sg.add(3.5);
    
    ScapeGoat<double> c;
    c=sg;
    c.add(11);
    
    sg.inorderTraversal();
    sg.bfs();
    c.inorderTraversal();
    c.bfs();
    
//    sg.remove(0);
//    sg.inorderTraversal();
//    sg.bfs();
    
//    if(sg.find(40)){
//        std::cout<<"\nfound\n";
//    }
}


#endif /* Chapter8_h */
