//
//  ArrayImpl.hpp
//  ods exercises
//
//  Created by Mohammad on 2019-08-12.
//  Copyright © 2019 Mohammad. All rights reserved.
//

#ifndef ArrayImpl_h
#define ArrayImpl_h

#include <stdexcept>

template<typename T>
class ArrayImpl{
    T* elems;
    
    void swap(ArrayImpl<T>& other) noexcept{
        std::swap(other.elems, elems);
        std::swap(other.len, len);
    }
    
public:
    size_t len;
    ArrayImpl(const size_t s): len{s}, elems{new T[s]} {}
    ArrayImpl(const size_t s, const T& init): ArrayImpl(s){
        for(int i=0; i<len; ++i)
            elems[i]=init;
    }
    ArrayImpl(const ArrayImpl<T>& a): ArrayImpl(a.len){
        for(int i=0; i<a.len; ++i)
            elems[i]=a.elems[i];
    }
    ArrayImpl<T>& operator=(const ArrayImpl<T>& a){
        if(this != & a){
            ArrayImpl<T> c{a};
            swap(c);
        }
        return *this;
    }
    ArrayImpl(ArrayImpl<T>&& a): elems{a.elems}, len{a.len}{
        a.elems=nullptr;
    }
    ArrayImpl<T>& operator=(ArrayImpl<T>&& a){
        ArrayImpl<T> c{std::move(a)};
        swap(c);
        return *this;
    }
    
    ~ArrayImpl(){delete [] elems;}

    void swap(const size_t i, const size_t j){
        assert(i<len && j<len);
        T tmp=elems[i];
        elems[i]=elems[j];
        elems[j]=tmp;
    }
    
    const T& operator[](const size_t i) const{
        assert(i<len);
        return elems[i];
    }
    
    T& operator[](const size_t i){
        return const_cast<T&>(static_cast<const ArrayImpl<T>&>(*this)[i]);
    }
    
    void reverse(){
        for(int i=0; i<len/2; ++i)
            swap(i, len-1-i);
    }
    
    void fill(const T& t){
        std::fill(elems, elems+len, t);
    }
    
    ArrayImpl<T> copyOfRange(const size_t i, const size_t j){
        assert(j<len && i<len && i<j);
        ArrayImpl<T> a{j-i};
        std::copy(elems, elems+j-i, a.elems);
        return a;
    }
    
};


#endif /* ArrayImpl_h */
