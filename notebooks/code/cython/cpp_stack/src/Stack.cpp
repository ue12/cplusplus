#include "Stack.h"

IntStack::IntStack(const int& sz): _pos(0), _values(sz) {}

void IntStack::push(const int& x){
    this->_values[this->_pos] = x;
    this->_pos++;
}

int IntStack::pop(){
    this->_pos--;
    return this->_values[this->_pos];
}

bool IntStack::isEmpty(){
    return this->_pos == 0;
}

std::vector<int> IntStack::getStorage(){
    return this->_values;
}

