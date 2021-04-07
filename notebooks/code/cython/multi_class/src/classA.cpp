
#include "classA.hpp"
#include "classB.hpp"


CppA::CppA(const std::string& name): _name(name), _ptr(nullptr) {}

void CppA::attach(CppB* ptr){
    this->_ptr = ptr;
    this->_ptr->attached();
}

std::string CppA::getName(){
    return this->_name; 
}

std::string CppA::getAttachedName(){
    if( this != nullptr ){
        return this->_ptr->getName();
    }
    else {
        return "NotAttached";
    }
}