#include "classA.hpp"
#include "classB.hpp"


CppB::CppB(const std::string& name): _name(name), _count(0) {}


std::string CppB::getName(){
    return this->_name; 
}


void CppB::attached(){
    this->_count++;
}

int CppB::isAttached(){
    return this->_count;
}
