//
//  Chapter12.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-25.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef Chapter12_h
#define Chapter12_h

#include "AssociateList.hpp"

void testAssociteList(){
    AssociateList<4> al;
    
    al.addEdge(0, 1);
    al.addEdge(0, 3);
    al.addEdge(1, 2);
    al.addEdge(1, 2);
    al.addEdge(1, 3);
    al.addEdge(2,0);
    al.addEdge(3, 2);
    AssociateList<4> c{al};
    al.removeEdge(0, 3);
    
    std::cout<<al;
    std::cout<<c;
    
//    Dlink<size_t> list{al.outEdges(1)};
//    std::cout<<list;
//    
//    Dlink<size_t> in{al.inEdges(2)};
//    std::cout<<in;
}


#endif /* Chapter12_h */
