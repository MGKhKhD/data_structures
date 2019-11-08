//
//  main.cpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-10.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#include "Chapter4.hpp"
#include "Chapter6.hpp"
#include "Chapter7.hpp"
#include "Chapter8.hpp"
#include "Chapter10.hpp"
#include "Chapter12.hpp"

#include "Algorithms.hpp"

#include "Dlink.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "ArrayImpl.hpp"

void chapter4();
void chapter6();
void chapter7();
void chapter8();
void chapter10();
void chapter12();

void testStack();
void testQueue();
void testDlink();
void testAlgorithms();

template<typename T>
Stack<T> produceStack(){
    Stack<T> s;
    s.push(22);
    s.push(220);
    s.push(2200);
    
    return s;
}

template<typename T>
Queue<T> produceQueue(){
    Queue<T> q;
    q.enqueue(10);
    q.enqueue(188);
    q.enqueue(288);
    q.enqueue(488);
    q.enqueue(400);
    
    return q;
}

int main(int argc, const char * argv[]) {

//    chapter4();
//    chapter6();
    chapter7();
//    chapter8();
//    chapter12();
    
//    chapter10();
//    testArray();
    //    testStack();
//    testQueue();
//    testDlink();
    
//    testAlgorithms();
    
    return 0;
}

void testAlgorithms(){
    int num=13562;
    int n=swap_first_last_digits(num);
    std::cout<<n<<std::endl;
}

void chapter12(){
    testAssociteList();
}

void testDlink(){
    Dlink<double> dl;
    dl.addBack(-10);
    dl.addBack(-12);
    dl.addBack(-15);
    dl.addFront(10);
    dl.addFront(12);
    dl.addFront(15);
    
    Dlink<double> c;
    c=dl;
    
    dl.remove(15);
    if(dl.find(10)){
        std::cout<<"\nfound\n";
    }
    c.addBack(100);
    c.addFront(100);
    
    std::cout<<dl;
    while(!dl.isEmpty()){
        std::cout<<dl.removeBack()<<std::setw(6);
    }
    dl.addFront(10);
    dl.addBack(-10);
    std::cout<<dl;
    std::cout<<c;
    std::cout<<"\n";
}

void chapter6(){
//    testBSTfinger();
    testEx6_6();
    
}

void chapter8(){
//    testScapteGoat();
//    testSGfinger();
//    testEx8_8();
    testEx8_9();
//    testEx8_10();
}

void chapter7(){
//    testTreap();
//    testTreap_A();
//    testEx7_7();
//    testEx7_9();
//    testEx7_10();
//    testEx7_12();
    testEx7_13();
}

void chapter10(){
    testBianryHeap();
    
}

void chapter4(){
//    testSkipList();
    
//    testSkipSete();
//    testEx4_8();
//    testEx4_9();
//    testEx4_10();
//    testEx4_11();
    testEx4_12();
}

void testQueue(){
    Queue<double> q;
    q.enqueue(10);
    q.enqueue(188);
    q.enqueue(288);
    q.enqueue(488);
    q.enqueue(400);
    
    Queue<double> c;
    c=produceQueue<double>();
    c.enqueue(-2);
    q.enqueue(588);
    
    while(!q.isEmpty()){
        std::cout<<q.dequeue()<<std::setw(6);
    }
    std::cout<<"\n";
    
    while(!c.isEmpty()){
        std::cout<<c.dequeue()<<std::setw(6);
    }
    std::cout<<"\n";
}

void testStack(){
    Stack<double> s;
    s.push(10);
    s.push(24);
    s.push(11);
    s.pop();
    s.push(-23);
    
//    Stack<double> c{produceStack<double>()};
    Stack<double> c;
    c=produceStack<double>();
    c.push(100);
    s.push(-22);
    
    while (!c.isEmpty()) {
        std::cout<<c.top()<<std::setw(5);
        c.pop();
    }
    std::cout<<"\n";
    
    while(!s.isEmpty())
    {
        std::cout<<s.top()<<std::setw(5);
        s.pop();
    }
    std::cout<<"\n";
}
