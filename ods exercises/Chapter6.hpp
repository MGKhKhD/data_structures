//
//  Chapter6.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-16.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Chapter6_h
#define Chapter6_h

#include "BinaryTree.hpp"

#include "BSTfinger.hpp"
#include "Ex6_6.hpp"

void testEx6_6(){
    Ex6_6<double> t;
    
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
    t.remove(7);
    t.inorderTraversal();
    
    if(t.isBalanced()){
        std::cout<<"balanced\n";
    }
}

void testBSTfinger(){
    BSTfinger<double> t;
    
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
    
    t.remove(3);
    t.inorderTraversal();
    
    
}

#endif /* Chapter6_h */
