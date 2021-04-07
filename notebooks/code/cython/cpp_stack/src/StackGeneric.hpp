#pragma once 

template<typename T>
class StackGeneric{
public:
    StackGeneric(const int& sz): _pos(0), _values(sz) {}

    void push(const T& x){
        this->_values[this->_pos] = x;
        this->_pos++;
    }

    T pop(){
        this->_pos--;
        return this->_values[this->_pos];
    }

    bool isEmpty(){
        return this->_pos == 0;
    }

    std::vector<T> getStorage(){
        return this->_values;
    }

protected:
    int _pos;
    std::vector<T> _values;
};